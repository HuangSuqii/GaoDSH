#include "EnergySaliencyFT.h"

EnergySaliencyFT* EnergySaliencyFT::mSingleton = NULL;

void EnergySaliencyFT::AlgrithomProcessor(Mat& srcImg, Mat& desImg)
{
	srcImg.convertTo(srcImg, CV_32FC3, 1.0 / 255);
	desImg = GetFT(srcImg);
}

Mat EnergySaliencyFT::GetFT(CMat &img3f)
{
	CV_Assert(img3f.data != NULL && img3f.type() == CV_32FC3);
	Mat sal(img3f.size(), CV_32F), tImg;
	GaussianBlur(img3f, tImg, Size(3, 3), 0);
	cvtColor(tImg, tImg, CV_BGR2Lab);
	Scalar colorM = mean(tImg);
	for (int r = 0; r < tImg.rows; r++) {
		float *s = sal.ptr<float>(r);
		float *lab = tImg.ptr<float>(r);
		for (int c = 0; c < tImg.cols; c++, lab += 3)
			s[c] = (float)(sqr(colorM[0] - lab[0]) + sqr(colorM[1] - lab[1]) + sqr(colorM[2] - lab[2]));
	}
	normalize(sal, sal, 0, 1, NORM_MINMAX);
	return sal;
}