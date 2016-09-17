#include "EnergySaliencySR.h"

EnergySaliencySR* EnergySaliencySR::mSingleton = NULL;

void EnergySaliencySR::AlgrithomProcessor(Mat& srcImg, Mat& desImg)
{
	srcImg.convertTo(srcImg, CV_32FC3, 1.0 / 255);
	desImg = GetSR(srcImg);
}

void EnergySaliencySR::AbsAngle(CMat& cmplx32FC2, Mat& mag32FC1, Mat& ang32FC1)
{
	CV_Assert(cmplx32FC2.type() == CV_32FC2);
	mag32FC1.create(cmplx32FC2.size(), CV_32FC1);
	ang32FC1.create(cmplx32FC2.size(), CV_32FC1);

	for (int y = 0; y < cmplx32FC2.rows; y++)
	{
		const float* cmpD = cmplx32FC2.ptr<float>(y);
		float* dataA = ang32FC1.ptr<float>(y);
		float* dataM = mag32FC1.ptr<float>(y);
		for (int x = 0; x < cmplx32FC2.cols; x++, cmpD += 2)
		{
			dataA[x] = atan2(cmpD[1], cmpD[0]);
			dataM[x] = sqrt(cmpD[0] * cmpD[0] + cmpD[1] * cmpD[1]);
		}
	}
}

void EnergySaliencySR::GetCmplx(CMat& mag32F, CMat& ang32F, Mat& cmplx32FC2)
{
	CV_Assert(mag32F.type() == CV_32FC1 && ang32F.type() == CV_32FC1 && mag32F.size() == ang32F.size());
	cmplx32FC2.create(mag32F.size(), CV_32FC2);
	for (int y = 0; y < mag32F.rows; y++)
	{
		float* cmpD = cmplx32FC2.ptr<float>(y);
		const float* dataA = ang32F.ptr<float>(y);
		const float* dataM = mag32F.ptr<float>(y);
		for (int x = 0; x < mag32F.cols; x++, cmpD += 2)
		{
			cmpD[0] = dataM[x] * cos(dataA[x]);
			cmpD[1] = dataM[x] * sin(dataA[x]);
		}
	}
}

Mat EnergySaliencySR::GetSR(CMat &img3f)
{
	Size sz(64, 64);
	Mat img1f[2], sr1f, cmplxSrc2f, cmplxDst2f;
	cvtColor(img3f, img1f[1], CV_BGR2GRAY);
	resize(img1f[1], img1f[0], sz, 0, 0, CV_INTER_AREA);

	img1f[1] = Mat::zeros(sz, CV_32F);
	merge(img1f, 2, cmplxSrc2f);
	dft(cmplxSrc2f, cmplxDst2f);
	AbsAngle(cmplxDst2f, img1f[0], img1f[1]);

	log(img1f[0], img1f[0]);
	blur(img1f[0], sr1f, Size(3, 3));
	sr1f = img1f[0] - sr1f;

	exp(sr1f, sr1f);
	GetCmplx(sr1f, img1f[1], cmplxDst2f);
	dft(cmplxDst2f, cmplxSrc2f, DFT_INVERSE | DFT_SCALE);
	split(cmplxSrc2f, img1f);

	pow(img1f[0], 2, img1f[0]);
	pow(img1f[1], 2, img1f[1]);
	img1f[0] += img1f[1];

	GaussianBlur(img1f[0], img1f[0], Size(3, 3), 0);
	normalize(img1f[0], img1f[0], 0, 1, NORM_MINMAX);
	resize(img1f[0], img1f[1], img3f.size(), 0, 0, INTER_CUBIC);

	return img1f[1];
}