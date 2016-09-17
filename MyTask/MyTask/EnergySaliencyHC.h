#pragma once
/************************************************************************/
/*[3]M.-M. Cheng, N. J. Mitra, X. Huang, P.H.S. Torr S.-M. Hu. Global	*/
/*   Contrast based Salient Region Detection. IEEE PAMI, 2014.			*/
/************************************************************************/
#include "EnergyFeature.h"
#include "EnergySaliencyUtil.h"

class EnergySaliencyHC : EnergyFeature
{
public:
	~EnergySaliencyHC()
	{
		if (mSingleton)
			delete mSingleton;
	}
	static EnergySaliencyHC* GetInstance()
	{
		if (mSingleton == NULL)
		{
			mSingleton = new EnergySaliencyHC();
		}
		return mSingleton;
	}
	void AlgrithomProcessor(Mat& srcImg, Mat& desImg);
	//void setKsize(int ksize) { wKsize = ksize; }

private:
	EnergySaliencyHC() {}	//私有构造函数
	static EnergySaliencyHC* mSingleton;

	static int Quantize(CMat& img3f, Mat &idx1i, Mat &_color3f, Mat &_colorNum, double ratio = 0.95, const int colorNums[3] = DefaultNums);
	static const int DefaultNums[3];

	static void SmoothSaliency(Mat &sal1f, float delta, const vector<vector<CostfIdx>> &similar);
	static void SmoothSaliency(CMat &colorNum1i, Mat &sal1f, float delta, const vector<vector<CostfIdx>> &similar);

	// Histogram Contrast of [3]
	static Mat GetHC(CMat &img3f);
	static void GetHC(CMat &binColor3f, CMat &colorNums1i, Mat &colorSaliency);
};