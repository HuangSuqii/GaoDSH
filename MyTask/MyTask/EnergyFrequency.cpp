#include "EnergyFrequency.h"

EnergyFrequency* EnergyFrequency::mSingleton = NULL;

void EnergyFrequency::AlgrithomProcessor(Mat& srcImg, Mat& desImg)
{
	unsigned char* cvtImg = mat2GrayImgPointer(srcImg, desImg);

	int nr = desImg.rows; int nc = desImg.cols;
	uchar* spectual = new uchar[nr * nc];
	memset(spectual, 0, nr*nc*sizeof(uchar));
	uchar* spectual_shift = new uchar[nr * nc];
	memset(spectual_shift, 0, nr*nc*sizeof(uchar));

	FFTShow(cvtImg, spectual, nc, nr);
	FFTShift(spectual, spectual_shift, nc, nr);
	Mat outimage(desImg.rows, desImg.cols, CV_8UC1, spectual_shift);
	desImg = outimage;
	delete spectual;	
	//delete spectual_shift;	// ���ﲻ��ɾ��ɾ�˺�desImg Ҳ��Ӱ�졣
}

void EnergyFrequency::FFTShow(uchar* img_in, uchar* spectual, int lWidth, int lHeight)
{
	fftw_complex *in, *out;
	fftw_plan p;
	int i = 0, j = 0;

	double * temp = new double[lHeight*lWidth];
	memset(temp, 0, lHeight*lWidth*sizeof(double));

	in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) *lHeight*lWidth);    //���ԭʼ����
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) *lHeight*lWidth);    //��Ÿ���Ҷ���任���

	int k = 0;
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++, k++)
		{
			int k1 = lWidth*i + j;
			in[k][0] = (double)img_in[k1];     //ʵ��    Դͼ��ת����Ϊ������ʽ
			in[k][1] = 0.0;									  //�鲿
		}
	}

	p = fftw_plan_dft_2d(lHeight, lWidth, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p);

	//��fft�任���Ƶ�ף�������ȡlog10��������ֵ
	double _max = -1;
	double _min = 10000;
	for (i = 0; i < lHeight*lWidth; i++)
	{
		temp[i] = log10(sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]));
		if (_max < temp[i])
		{
			_max = temp[i];
		}
		if (_min > temp[i])
		{
			_min = temp[i];
		}
	}

	double bili = _max - _min;

	k = 0;
	int image_size = lWidth*lHeight;
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++, k++)
		{
			//Ƶ��ȥlog10���һ��
			int k1 = lWidth*i + j;
			spectual[k1] = (unsigned char) ((temp[k] - _min) * 255 / bili);   // ���ֻ��Ϊ����ʾ�ÿ�����
		}
	}

	//FFTShift(image_fft, image_fftshift, nc, nr);						// ��ȡ���Ļ�Ƶ��
	
	fftw_destroy_plan(p);
	fftw_free(in);
	fftw_free(out);

	delete temp;
}

void EnergyFrequency::FFTShift(unsigned char* image, unsigned char* fftshift_image, long lWidth, long lHeight)
{
	int linebyte = (lWidth + 3) / 4 * 4;

	int x, y;
	int half_height_ceil = (int)ceil(lHeight / 2.0);      //   ceil(5/2) =3
	int half_height_floor = (int)floor(lHeight / 2.0);     //   floor(5/2)=2  
	int half_width_ceil = (int)ceil(lWidth / 2.0);
	int half_width_floor = (int)floor(lWidth / 2.0);


	//��һ����ֵ������������
	for (y = 0; y < half_height_floor; y++)
	{
		for (x = 0; x < half_width_floor; x++)
		{
			int  k = linebyte*y + x;
			int  k1 = linebyte*(y + half_height_ceil) + x + half_width_ceil;
			fftshift_image[k] = image[k1];
		}
	}
	//��������ֵ�����ڶ�����
	for (y = half_height_floor; y < lHeight; y++)
	{
		for (x = 0; x < half_width_floor; x++)
		{
			int  k = linebyte*y + x;
			int  k1 = linebyte*(y - half_height_floor) + x + half_width_ceil;
			fftshift_image[k] = image[k1];
		}
	}

	//�ڶ�����ֵ������������
	for (y = 0; y < half_height_floor; y++)
	{
		for (x = half_width_floor; x < lWidth; x++)
		{
			int  k = linebyte*y + x;
			int  k1 = linebyte*(y + half_height_ceil) + x - half_width_floor;
			fftshift_image[k] = image[k1];
		}
	}

	//��������ֵ������һ����
	for (y = half_height_floor; y < lHeight; y++)
	{
		for (x = half_width_floor; x < lWidth; x++)
		{
			int  k = linebyte*y + x;
			int  k1 = linebyte*(y - half_height_floor) + x - half_width_floor;
			fftshift_image[k] = image[k1];
		}
	}

}