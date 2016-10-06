#include "ShapeFeature.h"

//·ÖÐÎËã·¨
class ShaprFractal : public ShapeFeature
{
public:

	~ShaprFractal(void) 
	{
		if (mSingleton)
		{
			delete mSingleton;
		}
	}

	static ShaprFractal* GetInstance()
	{
		if (mSingleton == NULL)
		{
			mSingleton = new ShaprFractal();
		}
		return mSingleton;
	}

	void AlgrithomProcessor(Mat& srcImg, Mat& desImg);

private:
	ShaprFractal(void) {}
	static ShaprFractal* mSingleton;

	void fd(unsigned char* image, int height, int width);
};