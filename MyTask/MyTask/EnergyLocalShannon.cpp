#include "EnergyLocalShannon.h"
#include "math.h"
#include "string.h"

//�ྲ̬����������.cpp�ļ�������
EnergyLocalShannon*	EnergyLocalShannon::mSingleton = NULL;

void EnergyLocalShannon::AlgrithomProcessor(Mat& srcImg, Mat& desImg)
{
	unsigned char* cvtImg =  mat2GrayImgPointer(srcImg, desImg);

	localshannon( cvtImg, desImg.rows, desImg.cols, mwKsize);
	Mat outimage(desImg.rows, desImg.cols,  CV_8UC1, cvtImg);
	desImg = outimage;
}

void EnergyLocalShannon::localshannon(unsigned char *image, int height, int width, int Ksize)
{
	int i, j, k, x, y; // ����ѭ��
	int lLBytes = ((width * 8) + 31) / 32 * 4;
	float *OutImage = new float[lLBytes*height];
	memset(OutImage, 0, sizeof(float)*lLBytes*height);
	int *histArray_1D = new int[256];
	//1.��ͼ��ľ�ֵ(Ϊ��ֻ������ھ�ֵ�ĵ���ټ�����)
	float avg = 0;
	int S_LL = (2 * Ksize + 1)*(2 * Ksize + 1);
	float Shannnon;
	for (j = 0; j<height; j++)
	{
		for (i = 0; i<width; i++)
			avg += float(image[j*lLBytes + i]);
	}
	avg = avg / (width*height);
	for (j = Ksize; j<height - Ksize; j++)
	{
		for (i = Ksize; i<width - Ksize; i++)
		{
			k = j*lLBytes + i;
			if (image[k]>avg) //С�ھ�ֵ�ĵ㲻����
			{
				memset(histArray_1D, 0, sizeof(int) * 256);
				//2.��ֲ�����ֱ��ͼ
				for (y = j - Ksize; y <= j + Ksize; y++)
				{
					for (x = i - Ksize; x <= i + Ksize; x++)
					{
						int index = image[y*lLBytes + x];
						histArray_1D[index]++;
					}
				}
				//3.��ֲ�Shannon
				Shannnon = 0;
				for (int len = 0; len<256; len++)
				{
					float p = float(histArray_1D[len]) / float(S_LL);
					if (p<0.00000001)
						continue;
					Shannnon += -float(p*log10(p));
				}
				OutImage[k] = Shannnon;
			}
		}
	}
	float maxT = 0;
	//�Խ��ͼ����й�һ����������ʾ�ĶԱȶ�
	for (y = 0; y<height; y++)
	{
		for (x = 0; x<width; x++)
		{
			if (OutImage[y*lLBytes + x]>maxT)
				maxT = OutImage[y*lLBytes + x];       // ����ͼ����������ֵ
		}
	}

	for (y = 0; y<height; y++)
	{
		for (x = 0; x<width; x++)
		{
			image[y*lLBytes + x] = (unsigned char)(OutImage[y*lLBytes + x] * 255 / maxT);//�Բ�ֵͼ����й�һ��
		}
	}
	delete[] OutImage;
	delete[] histArray_1D;
	// 	int a ;
	// 	int b ;
	// 	RegionGrow(image,width,height,a,b,4);
}

