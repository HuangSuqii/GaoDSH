#include "EnergySaliencyLC.h"

EnergySaliencyLC* EnergySaliencyLC::mSingleton = NULL;

void EnergySaliencyLC::AlgrithomProcessor(Mat& srcImg, Mat& desImg)
{
	srcImg.convertTo(srcImg, CV_32FC3, 1.0 / 255);
	desImg = GetLC(srcImg);
}

Mat EnergySaliencyLC::GetLC(CMat &img3f)
{
	Mat img;
	cvtColor(img3f, img, CV_BGR2GRAY);
	img.convertTo(img, CV_8U, 255);
	double f[256], s[256];
	memset(f, 0, 256 * sizeof(double));
	memset(s, 0, 256 * sizeof(double));
	for (int r = 0; r < img.rows; r++){
		byte* data = img.ptr<byte>(r);
		for (int c = 0; c < img.cols; c++)
			f[data[c]] += 1;
	}
	for (int i = 0; i < 256; i++)
		for (int j = 0; j < 256; j++)
			s[i] += abs(i - j) * f[j];
	Mat sal1f(img3f.size(), CV_64F);
	for (int r = 0; r < img.rows; r++){
		byte* data = img.ptr<byte>(r);
		double* sal = sal1f.ptr<double>(r);
		for (int c = 0; c < img.cols; c++)
			sal[c] = s[data[c]];
	}
	normalize(sal1f, sal1f, 0, 1, NORM_MINMAX, CV_32F);
	return sal1f;
}