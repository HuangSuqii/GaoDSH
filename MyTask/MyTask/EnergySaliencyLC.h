#pragma once
/************************************************************************/
/*  [2]Y. Zhai and M. Shah. Visual attention detection in video sequences */
/*   using spatiotemporal cues. In ACM Multimedia 2006.					*/
/************************************************************************/
#include "EnergyFeature.h"
#include "EnergySaliencyUtil.h"

class EnergySaliencyLC : EnergyFeature
{
public:
	~EnergySaliencyLC()
	{
		if (mSingleton)
			delete mSingleton;
	}
	static EnergySaliencyLC* GetInstance()
	{
		if (mSingleton == NULL)
		{
			mSingleton = new EnergySaliencyLC();
		}
		return mSingleton;
	}
	void AlgrithomProcessor(Mat& srcImg, Mat& desImg);	// 接口函数


private:
	EnergySaliencyLC() {}	//私有构造函数
	static EnergySaliencyLC* mSingleton;

	// Luminance Contrast [2]
	static Mat GetLC(CMat &img3f);	// 核心函数
};