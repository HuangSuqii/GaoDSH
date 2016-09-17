#pragma once
/************************************************************************/
/*   [4]X. Hou and L. Zhang. Saliency detection: A spectral residual		*/
/*   approach. In IEEE CVPR 2007, 2007.									*/
/************************************************************************/
#include "EnergyFeature.h"
#include "EnergySaliencyUtil.h"

class EnergySaliencySR : EnergyFeature
{
public:
	~EnergySaliencySR()
	{
		if (mSingleton)
			delete mSingleton;
	}
	static EnergySaliencySR* GetInstance()
	{
		if (mSingleton == NULL)
		{
			mSingleton = new EnergySaliencySR();
		}
		return mSingleton;
	}
	void AlgrithomProcessor(Mat& srcImg, Mat& desImg);	// 接口函数


private:
	EnergySaliencySR() {}	//私有构造函数
	static EnergySaliencySR* mSingleton;

	static void AbsAngle(CMat& cmplx32FC2, Mat& mag32FC1, Mat& ang32FC1);
	static void GetCmplx(CMat& mag32F, CMat& ang32F, Mat& cmplx32FC2);

	// Spectral Residual [4]
	static Mat GetSR(CMat &img3f);	// 核心函数
};