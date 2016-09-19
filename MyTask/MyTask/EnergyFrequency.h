#include "EnergyFeature.h"
/************************************************************************/
/* Ƶ������     */
/************************************************************************/
#include "fftw3.h"   //���fftwͷ�ļ�

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
	void AlgrithomProcessor(Mat& srcImg, Mat& desImg);	// �ӿں���


private:
	EnergyFrequency() {}
	static EnergyFrequency* mSingleton;

	// ����Ҷ�任
	void FFTShow(uchar* img_in, uchar* spectual, int lWidth, int lHeight);

	//����ҶƵ�װ���,�������ȣ����д����˳���Ϊż�������
	void FFTShift(unsigned char* image, unsigned char* fftshift_image, long lWidth, long lHeight);

};