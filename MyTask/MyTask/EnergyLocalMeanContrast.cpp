#include "EnergyLocalMeanContrast.h"

EnergyLocalMeanContrast* EnergyLocalMeanContrast::mSingleton = NULL;

void EnergyLocalMeanContrast::AlgrithomProcessor(Mat& srcImg, Mat& desImg)
{
	unsigned char* cvtImg =  mat2GrayImgPointer(srcImg, desImg);

	localmeancontrast( cvtImg, desImg.rows, desImg.cols, mswKsize, mbwKsize);
	Mat outimage(desImg.rows, desImg.cols,  CV_8UC1, cvtImg);
	desImg = outimage;
}

void EnergyLocalMeanContrast::localmeancontrast( unsigned char* image, int height, int width, int sksize, int bksize )
{
	int i, j, k, x, y; // 用于循环
	int lLBytes = ((width * 8) + 31) / 32 * 4;
	float *OutImage = new float[lLBytes*height];
	memset(OutImage, 0, sizeof(float)*lLBytes*height);
	//1.求图像的均值(为了只处理大于均值的点减少计算量)
	float avg = 0;
	float Mean_ll;
	float Mean_LL;
	int S_ll = (2 * sksize + 1)*(2 * sksize + 1);
	int S_LL = (2 * bksize + 1)*(2 * bksize + 1);
	for (j = 0; j<height; j++)
	{
		for (i = 0; i<width; i++)
			avg += float(image[j*lLBytes + i]);
	}
	avg = avg / (width*height);
	for (j = bksize; j<height - bksize; j++)
	{
		for (i = bksize; i<width - bksize; i++)
		{
			k = j*lLBytes + i;
			if (image[k]>avg) //小于均值的点不处理
			{
				//2.求局部内窗的均值
				Mean_ll = 0;
				for (y = j - sksize; y <= j + sksize; y++)
				{
					for (x = i - sksize; x <= i + sksize; x++)
						Mean_ll += float(image[y*lLBytes + x]);
				}
				Mean_ll = Mean_ll / S_ll;
				if (Mean_ll>avg) //小于均值的点不处理
				{
					//3.求局部外窗的均值
					Mean_LL = 0;
					for (y = j - bksize; y <= j + bksize; y++)
					{
						for (x = i - bksize; x <= i + bksize; x++)
							Mean_LL += float(image[y*lLBytes + x]);
					}
					Mean_LL = Mean_LL / S_LL;
					//4.求均值对比度
					if (Mean_ll>Mean_LL)
						OutImage[k] = (Mean_ll - Mean_LL) / Mean_ll;
				}
			}
		}
	}

	float maxT = 0;
	//对结果图像进行归一化，扩大显示的对比度
	for (y = 0; y<height; y++)
	{
		for (x = 0; x<width; x++)
		{
			if (OutImage[y*lLBytes + x]>maxT)
				maxT = OutImage[y*lLBytes + x];       // 计算图像的最高像素值
		}
	}

	for (y = 0; y<height; y++)
	{
		for (x = 0; x<width; x++)
		{
			image[y*lLBytes + x] = (unsigned char)(OutImage[y*lLBytes + x] * 255 / maxT);//对差值图像进行归一化
		}
	}
	delete[] OutImage;
	// 	int a ;
	// 	int b ;
	// 	RegionGrow(image,width,height,a,b,4);
}