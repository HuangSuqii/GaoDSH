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
	int x, y;									//ͼ�������ѭ������
	int Emax = 2;								//�߶ȵ����ѭ������
	int linebyte = (width + 3) / 4 * 4;				//ʹͼ����п���Ա�������

	double *U1 = new double[linebyte*height];  //��̺���������� 
	double *B1 = new double[linebyte*height];	//��̺����������

	double *U2 = new double[linebyte*height];  //��̺����������
	double *B2 = new double[linebyte*height];  //��̺����������

	double*VV = new double[linebyte*height];   //��̺���
	double*AA = new double[linebyte*height];	//��̺�����

	//����ֵ
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

	//��ʼ��
	memset(U2, 0, linebyte*height*sizeof(double));
	memset(B2, 0, linebyte*height*sizeof(double));

	memset(VV, 0, linebyte*height*sizeof(double));
	memset(AA, 0, linebyte*height*sizeof(double));

	//ѭ��������ÿһ�ε�������������
	for (int e = 1; e < Emax; e++)
	{
		for (y = 1; y < height - 1; y++)
		{
			for (x = 1; x < width - 1; x++)
			{
				int k = linebyte*y + x;
				double tempu2 = U1[k] + 1;
				double tempb2 = B1[k] - 1;
				//������������С�����ֵ
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

	//�������V (��-E��E ����Ĳ�ֵ��)
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
			AA[linebyte*y + x] = (double)((double)VV[linebyte*y + x] / (2 * Emax));  //��������
		}
	}

	//���������һ����ʹ����0~255��ʾ��
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