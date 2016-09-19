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
	//delete spectual_shift;	// 这里不能删，删了后desImg 也受影响。
}

void EnergyFrequency::FFTShow(uchar* img_in, uchar* spectual, int lWidth, int lHeight)
{
	fftw_complex *in, *out;
	fftw_plan p;
	int i = 0, j = 0;

	double * temp = new double[lHeight*lWidth];
	memset(temp, 0, lHeight*lWidth*sizeof(double));

	in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) *lHeight*lWidth);    //存放原始数据
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) *lHeight*lWidth);    //存放傅里叶正变换结果

	int k = 0;
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++, k++)
		{
			int k1 = lWidth*i + j;
			in[k][0] = (double)img_in[k1];     //实部    源图像转换成为复数形式
			in[k][1] = 0.0;									  //虚部
		}
	}

	p = fftw_plan_dft_2d(lHeight, lWidth, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p);

	//求fft变换后的频谱，并对其取log10，求出最大值
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
			//频谱去log10后归一化
			int k1 = lWidth*i + j;
			spectual[k1] = (unsigned char) ((temp[k] - _min) * 255 / bili);   // 这个只是为了显示好看而已
		}
	}

	//FFTShift(image_fft, image_fftshift, nc, nr);						// 求取中心化频谱
	
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


	//第一象限值换到第三象限
	for (y = 0; y < half_height_floor; y++)
	{
		for (x = 0; x < half_width_floor; x++)
		{
			int  k = linebyte*y + x;
			int  k1 = linebyte*(y + half_height_ceil) + x + half_width_ceil;
			fftshift_image[k] = image[k1];
		}
	}
	//第四象限值换到第二象限
	for (y = half_height_floor; y < lHeight; y++)
	{
		for (x = 0; x < half_width_floor; x++)
		{
			int  k = linebyte*y + x;
			int  k1 = linebyte*(y - half_height_floor) + x + half_width_ceil;
			fftshift_image[k] = image[k1];
		}
	}

	//第二象限值换到第四象限
	for (y = 0; y < half_height_floor; y++)
	{
		for (x = half_width_floor; x < lWidth; x++)
		{
			int  k = linebyte*y + x;
			int  k1 = linebyte*(y + half_height_ceil) + x - half_width_floor;
			fftshift_image[k] = image[k1];
		}
	}

	//第三象限值换到第一象限
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