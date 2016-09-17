#include "EnergyFeature.h"
/************************************************************************/
/*	灰度特性——局部均值对比度     */
/************************************************************************/
class EnergyLocalMeanContrast : public EnergyFeature
{
public:
	~EnergyLocalMeanContrast()
	{
		if (mSingleton)
		{
			delete mSingleton;
		}
	}
	static EnergyLocalMeanContrast* GetInstance()
	{
		if (mSingleton == NULL)
		{
			mSingleton = new EnergyLocalMeanContrast();
		}
		return mSingleton;
	}
	void AlgrithomProcessor(Mat& srcImg, Mat& desImg);	// 接口函数
	void setKsize(int sksize, int bksize)		// 调参方法
	{
		mswKsize = sksize; 
		mbwKsize = bksize;
	}

private:
	EnergyLocalMeanContrast() : mswKsize(5) , mbwKsize(9) {}
	static EnergyLocalMeanContrast* mSingleton;
	// 原始函数，来自716平台，红外仿真与验证系统。
	void localmeancontrast( unsigned char* image, int height, int width, int sksize, int bksize );	
	int mswKsize;	// 小窗口
	int mbwKsize;	// 大窗口
};