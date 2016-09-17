#include "EnergyFeature.h"
/************************************************************************/
/* 灰度特性——局部方差                                                                    */
/************************************************************************/
class EnergyLocalStd : EnergyFeature
{
public:
	~EnergyLocalStd() 
	{
		if (mSingleton)
			delete mSingleton;
	}
	static EnergyLocalStd* GetInstance()
	{
		if (mSingleton == NULL)
		{
			mSingleton = new EnergyLocalStd();
		}
		return mSingleton;
	}
	void AlgrithomProcessor(Mat& srcImg, Mat& desImg);	// 接口函数
	void setKsize(int ksize) { wKsize = ksize; }	// 调参方法

private:
	EnergyLocalStd() : wKsize(5) {}
	static EnergyLocalStd* mSingleton;
	int wKsize;
	void localStd( unsigned char* image, int height, int width, int Ksize );	//	核心函数

};