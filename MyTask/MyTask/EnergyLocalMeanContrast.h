#include "EnergyFeature.h"
/************************************************************************/
/*	�Ҷ����ԡ����ֲ���ֵ�Աȶ�     */
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
	void AlgrithomProcessor(Mat& srcImg, Mat& desImg);	// �ӿں���
	void setKsize(int sksize, int bksize)		// ���η���
	{
		mswKsize = sksize; 
		mbwKsize = bksize;
	}

private:
	EnergyLocalMeanContrast() : mswKsize(5) , mbwKsize(9) {}
	static EnergyLocalMeanContrast* mSingleton;
	// ԭʼ����������716ƽ̨�������������֤ϵͳ��
	void localmeancontrast( unsigned char* image, int height, int width, int sksize, int bksize );	
	int mswKsize;	// С����
	int mbwKsize;	// �󴰿�
};