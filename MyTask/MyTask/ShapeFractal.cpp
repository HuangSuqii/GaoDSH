#include "ShapeFractal.h"

ShaprFractal* ShaprFractal::mSingleton = NULL;

void ShaprFractal::AlgrithomProcessor(Mat& srcImg, Mat& desImg)
{
	unsigned char* cvtImg = mat2GrayImgPointer(srcImg, desImg);
	fd(cvtImg, desImg.rows, desImg.cols);
	Mat outimage(desImg.rows, desImg.cols, CV_8UC1, cvtImg);
	desImg = outimage;
}

void ShaprFractal::fd(unsigned char* image, int height, int width)
{
	int x, y;									//图像坐标的循环变量
	int Emax = 2;								//尺度的最大循环次数
	int linebyte = (width + 3) / 4 * 4;				//使图像的列宽可以被四整除

	double *U1 = new double[linebyte*height];  //地毯法的上曲面 
	double *B1 = new double[linebyte*height];	//地毯法的下曲面

	double *U2 = new double[linebyte*height];  //地毯法的上曲面
	double *B2 = new double[linebyte*height];  //地毯法的下曲面

	double*VV = new double[linebyte*height];   //地毯体积
	double*AA = new double[linebyte*height];	//地毯表面积

	//赋初值
	memset(U1, 0, linebyte*height*sizeof(double));
	memset(B1, 0, linebyte*height*sizeof(double));

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			int k = linebyte*y + x;
			U1[k] = image[k];
			B1[k] = image[k];
		}
	}

	//初始化
	memset(U2, 0, linebyte*height*sizeof(double));
	memset(B2, 0, linebyte*height*sizeof(double));

	memset(VV, 0, linebyte*height*sizeof(double));
	memset(AA, 0, linebyte*height*sizeof(double));

	//循环，计算每一次的上下曲面的体积
	for (int e = 1; e < Emax; e++)
	{
		for (y = 1; y < height - 1; y++)
		{
			for (x = 1; x < width - 1; x++)
			{
				int k = linebyte*y + x;
				double tempu2 = U1[k] + 1;
				double tempb2 = B1[k] - 1;
				//八领域搜索最小和最大值
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						if (tempu2 < U1[linebyte*(y + i) + x + j])
							tempu2 = U1[linebyte*(y + i) + x + j];

						if (tempb2 > B1[linebyte*(y + i) + x + j])
							tempb2 = B1[linebyte*(y + i) + x + j];
					}
				}
				U2[k] = tempu2;
				B2[k] = tempb2;
			}
		}

		memcpy(U1, U2, linebyte*height*sizeof(double));
		memcpy(B1, B2, linebyte*height*sizeof(double));
	}

	//计算体积V (是-E到E 领域的差值和)
	for (y = Emax; y < height - Emax; y++)
	{
		for (x = Emax; x < width - Emax; x++)
		{
			for (int i = -Emax; i <= Emax; i++)
			{
				for (int j = -Emax; j <= Emax; j++)
				{
					VV[linebyte*y + x] += U1[linebyte*(y + i) + x + j] - B1[linebyte*(y + i) + x + j];
				}
			}
			AA[linebyte*y + x] = (double)((double)VV[linebyte*y + x] / (2 * Emax));  //计算表面积
		}
	}

	//将表面积归一化，使其在0~255显示。
	double maxT = 0.000001;
	for (y = 4; y < height - 4; y++)
	{
		for (x = 4; x < width - 4; x++)
		{
			int k = linebyte*y + x;
			if (maxT < AA[k])
				maxT = AA[k];
		}
	}

	memset(image, 0, linebyte*height*sizeof(unsigned char));
	for (y = 4; y < height - 4; y++)
	{
		for (x = 4; x < width - 4; x++)
		{
			int k = linebyte*y + x;
			int temp = (int)(AA[k] * (double)255.0 / (double)maxT);
			temp = temp > 255 ? 255 : temp;
			image[k] = temp;
		}
	}

	delete[] B1;
	delete[] B2;
	delete[] U1;
	delete[] U2;
	delete[] VV;
	delete[] AA;
}