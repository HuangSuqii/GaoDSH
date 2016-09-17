#include "EnergyFeature.h"
/************************************************************************/
/* �Ҷ����ԡ����ֲ�����                                                                    */
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
	void AlgrithomProcessor(Mat& srcImg, Mat& desImg);	// �ӿں���
	void setKsize(int ksize) { wKsize = ksize; }	// ���η���

private:
	EnergyLocalStd() : wKsize(5) {}
	static EnergyLocalStd* mSingleton;
	int wKsize;
	void localStd( unsigned char* image, int height, int width, int Ksize );	//	���ĺ���

};