#include "EnergyLocalStd.h"
#define PI 3.1415926535

EnergyLocalStd* EnergyLocalStd::mSingleton = NULL;

void EnergyLocalStd::AlgrithomProcessor(Mat& srcImg, Mat& desImg)
{
	unsigned char* cvtImg =  mat2GrayImgPointer(srcImg, desImg);
	localStd( cvtImg, desImg.rows, desImg.cols, wKsize);
	Mat outimage(desImg.rows, desImg.cols,  CV_8UC1, cvtImg);
	desImg = outimage;
}

void EnergyLocalStd::localStd( unsigned char* image, int height, int width, int Ksize )
{
	int x, y, i, j;
	int LineBytes = ((width * 8) + 31) / 32 * 4;  //一行所占的字节数
	int k2 = (Ksize - 1) / 2;
	unsigned char * NewImage = new unsigned char[height*LineBytes];
	memcpy(NewImage, image, LineBytes*height);

	unsigned char *MeanImage = new unsigned char[height*LineBytes];
	memcpy(MeanImage, image, LineBytes*height);//复制原始图像，用于存放像素在3*3区域内的均值

	float *gg = new float[height*LineBytes];
	memset(gg, 0, height*LineBytes*sizeof(float));
	float *EE = new float[height*LineBytes];
	memset(EE, 0, height*LineBytes*sizeof(float));

	float *kernel = new float[Ksize*Ksize];

	//求模板算子,采用的是单峰高斯分布来确定抑制系数
	for (j = -k2; j <= k2; j++)
	{
		for (i = -k2; i <= k2; i++)
		{
			float dx = abs(j);
			float dy = abs(i);
			float sigma = 10;
			float d = sqrt(dx*dx + dy*dy);
			float c = sqrt(2 * PI);
			float beta = 20;
			kernel[(j + k2)*Ksize + (i + k2)] = -1 / (beta*c*sigma)*exp(-d*d / (2 * sigma*sigma));
			kernel[0] = 0;

		}
	}
	//改进的3邻域模板

	for (y = k2; y<height - k2; y++)
	{
		for (x = k2; x<width - k2; x++)
		{
			//水平模板
			float g1 = 0, g2 = 0;
			for (j = -k2; j <= k2; j++)
			{
				g1 += NewImage[(y + j)*LineBytes + x + 1];
				g2 += NewImage[(y + j)*LineBytes + x - 1];
			}
			g1 = g1 / 3;
			g2 = g2 / 3;

			//垂直模板
			float g3 = 0, g4 = 0;
			for (i = -k2; i <= k2; i++)
			{
				g3 += NewImage[(y + 1)*LineBytes + x + i];
				g4 += NewImage[(y - 1)*LineBytes + x - i];
			}
			g3 = g3 / 3;
			g4 = g4 / 3;


			//左斜模板45
			float g5 = 0, g6 = 0;
			g5 = (NewImage[(y - 1)*LineBytes + x - 1] + NewImage[(y - 1)*LineBytes + x] + NewImage[y*LineBytes + x - 1]) / 3;//左上角三像素均值
			g6 = (NewImage[(y + 1)*LineBytes + x] + NewImage[(y + 1)*LineBytes + x + 1] + NewImage[y*LineBytes + x + 1]) / 3;//右下角三像素均值

			//右斜模板
			float g7 = 0, g8 = 0;
			g7 = (NewImage[y*LineBytes + x - 1] + NewImage[(y + 1)*LineBytes + x - 1] + NewImage[(y + 1)*LineBytes + x]) / 3;//右上角三像素均值
			g8 = (NewImage[(y - 1)*LineBytes + x] + NewImage[(y - 1)*LineBytes + x + 1] + NewImage[y*LineBytes + x + 1]) / 3;//右上角三像素均值


			//找出四个方向的最大值为gmax
			unsigned char g[4];

			g[0] = abs(g1 - g2);
			g[1] = abs(g3 - g4);
			g[2] = abs(g5 - g6);
			g[3] = abs(g7 - g8);
			for (int k = 0; k<4; k++)
			{
				if (g[k]>gg[y*LineBytes + x])
					gg[y*LineBytes + x] = g[k];
			}

		}
	}
	//找出gg[]中最大的一个
	unsigned char max;
	max = 0;
	for (y = k2; y<height - k2; y++)
	{
		for (x = k2; x<width - k2; x++)
		{
			if (gg[y*LineBytes + x]>max)
				max = gg[y*LineBytes + x];
		}
	}

	//计算E[y*LineBytes+x]
	for (y = k2; y<height - k2; y++)
	{
		for (x = k2; x<width - k2; x++)
		{
			EE[y*LineBytes + x] = gg[y*LineBytes + x] / max;
		}
	}

	//带入公式
	for (y = k2; y<height - k2; y++)
	{
		for (x = k2; x<width - k2; x++)
		{
			float sum = 0;
			float sum1 = 0;

			for (j = -k2; j <= k2; j++)
			{
				for (i = -k2; i <= k2; i++)
				{
					sum += MeanImage[(y + j)*LineBytes + x + i];
					sum1 += NewImage[(y + j)*LineBytes + x + i] * kernel[(j + k2)*Ksize + (i + k2)];

				}
			}
			MeanImage[y*LineBytes + x] = (int)sum / (Ksize*Ksize);
			image[y*LineBytes + x] = MeanImage[y*LineBytes + x] + sum1 + EE[y*LineBytes + x] * (MeanImage[y*LineBytes + x] - NewImage[y*LineBytes + x]);
		}
	}

	//处理边界 
	for (y = 0; y <= height - 1; y++)
	{
		for (x = 0; x <= width - 1; x++)
		{
			//处理上边界
			if (y == 0 || y == 1)
			{
				for (int x1 = 0; x1<width - 1; x1++)
				{
					image[y*LineBytes + x1] = image[2 * LineBytes + x1];
				}
			}
			//处理下边界
			if (y == height - 1 || y == height - 2)
			{
				for (int x1 = 0; x1<width - 1; x1++)
				{
					image[y*LineBytes + x1] = image[(height - 3)*LineBytes + x1];
				}
			}
			//处理左边界
			if (x == 0 || x == 1)
			{
				for (int y1 = 1; y1<height - 2; y1++)
				{
					image[y1*LineBytes + x] = image[y1*LineBytes + 2];
				}
			}
			//处理右边界
			if (x == width - 1 || x == width - 2)
			{
				for (int y1 = 1; y1<height - 2; y1++)
				{
					image[y1*LineBytes + x] = image[y1*LineBytes + width - 3];
				}
			}
		}
	}

	int Min = 255;
	int Max = 1;

	for (y = 0; y <= height - 1; y++)
	{
		for (x = 0; x <= width - 1; x++)
		{
			if (image[y*LineBytes + x]>Max)
				Max = image[y*LineBytes + x];
			if (image[y*LineBytes + x]<Min)
				Min = image[y*LineBytes + x];
		}
	}

	for (y = 0; y <= height - 1; y++)
	{
		for (x = 0; x <= width - 1; x++)
		{
			float s = float(image[y*LineBytes + x] - Min);
			float u = float(Max - Min);
			image[y*LineBytes + x] = int(s * 255 / u);
		}
	}
	delete[] gg;
	delete[] EE;
	delete[] NewImage;
	delete[] MeanImage;
}