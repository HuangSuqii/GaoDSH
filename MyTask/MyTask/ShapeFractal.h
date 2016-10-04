#include "ShapeFeature.h"

//分形算法
class ShaprFractal : public ShapeFeature
{
public:

	~ShaprFractal(void) {}

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

	class Cleaner   //该私有内嵌类的唯一工作就是在析构函数中删除单例
	{
	public:
		~Cleaner()
		{
			if (ShaprFractal::mSingleton)
				delete ShaprFractal::mSingleton;
		}
	};
	static Cleaner cleaner;  //定义一个静态成员变量，程序结束时，系统会自动调用它的析构函数  

	void fd(unsigned char* image, int height, int width);
};