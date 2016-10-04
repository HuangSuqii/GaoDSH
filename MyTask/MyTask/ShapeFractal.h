#include "ShapeFeature.h"

//�����㷨
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

	class Cleaner   //��˽����Ƕ���Ψһ��������������������ɾ������
	{
	public:
		~Cleaner()
		{
			if (ShaprFractal::mSingleton)
				delete ShaprFractal::mSingleton;
		}
	};
	static Cleaner cleaner;  //����һ����̬��Ա�������������ʱ��ϵͳ���Զ�����������������  

	void fd(unsigned char* image, int height, int width);
};