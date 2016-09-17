#include "EnergyFeature.h"
/************************************************************************/
/* �Ҷ����ԡ����ֲ���	*/
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

	void AlgrithomProcessor(Mat& srcImg, Mat& desImg);	// �ӿں���

	void setKsize(int ksize) { mwKsize = ksize; }	// ���η���

private:
	EnergyLocalShannon() : mwKsize(5) {}
	static EnergyLocalShannon* mSingleton;
	// �ֲ��صĺ��ĺ���
	void localshannon(unsigned char* imge, int height, int width, int Ksize);
	int mwKsize;
};