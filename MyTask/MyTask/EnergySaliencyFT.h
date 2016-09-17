#pragma once
/************************************************************************/
/* �������㷨FT                                                                     */
/*[1]R. Achanta, S. Hemami, F. Estrada and S. Susstrunk, Frequency-tuned*/
/*   Salient Region Detection, IEEE CVPR, 2009.							*/
/************************************************************************/
#include "EnergyFeature.h"
#include "EnergySaliencyUtil.h"

class EnergySaliencyFT : EnergyFeature
{
public:
	~EnergySaliencyFT()
	{
		if (mSingleton)
			delete mSingleton;
	}
	static EnergySaliencyFT* GetInstance()
	{
		if (mSingleton == NULL)
		{
			mSingleton = new EnergySaliencyFT();
		}
		return mSingleton;
	}
	void AlgrithomProcessor(Mat& srcImg, Mat& desImg);


private:
	EnergySaliencyFT() {}	//˽�й��캯��
	static EnergySaliencyFT* mSingleton;

	// Frequency Tuned [1]
	static Mat GetFT(CMat &img3f);
};