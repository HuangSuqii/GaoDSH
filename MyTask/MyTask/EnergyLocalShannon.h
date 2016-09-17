#include "EnergyFeature.h"
/************************************************************************/
/* 灰度特性——局部熵	*/
/************************************************************************/
class EnergyLocalShannon : public EnergyFeature
{
public:
	~EnergyLocalShannon() 
	{
		if(mSingleton)  
			delete mSingleton;
	}
	static EnergyLocalShannon* GetInstance()
	{
		if (mSingleton == NULL)
		{
			mSingleton = new EnergyLocalShannon();
		}
		return mSingleton;
	}

	void AlgrithomProcessor(Mat& srcImg, Mat& desImg);	// 接口函数

	void setKsize(int ksize) { mwKsize = ksize; }	// 调参方法

private:
	EnergyLocalShannon() : mwKsize(5) {}
	static EnergyLocalShannon* mSingleton;
	// 局部熵的核心函数
	void localshannon(unsigned char* imge, int height, int width, int Ksize);
	int mwKsize;
};