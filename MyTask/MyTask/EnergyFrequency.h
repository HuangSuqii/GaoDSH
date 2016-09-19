#include "EnergyFeature.h"
/************************************************************************/
/* 频谱能量     */
/************************************************************************/
#include "fftw3.h"   //添加fftw头文件

typedef unsigned char BYTE;
typedef unsigned char uchar;

class EnergyFrequency : public EnergyFeature
{
public:
	~EnergyFrequency()
	{
		if (mSingleton)
		{
			delete mSingleton;
		}
	}
	static EnergyFrequency* GetInstance()
	{
		if (mSingleton == NULL)
		{
			mSingleton = new EnergyFrequency();
		}
		return mSingleton;
	}
	void AlgrithomProcessor(Mat& srcImg, Mat& desImg);	// 接口函数


private:
	EnergyFrequency() {}
	static EnergyFrequency* mSingleton;

	// 傅里叶变换
	void FFTShow(uchar* img_in, uchar* spectual, int lWidth, int lHeight);

	//傅里叶频谱搬移,对任意宽度，其中处理了长宽不为偶数的情况
	void FFTShift(unsigned char* image, unsigned char* fftshift_image, long lWidth, long lHeight);

};