#pragma  once
/************************************************************************/
/* LBP����
	�����LBP����ͼ���Լ�����ͼ���LBP��������*/
/************************************************************************/
#include "ShapeFeature.h"
#include<vector>
#include <bitset>

using std::vector;
using std::bitset;
using cv::Range;

class ShapeLBP : ShapeFeature
{
public:
	~ShapeLBP() 
	{
		if ( mSingleton )
		{
			delete mSingleton;
		}
	}

	static ShapeLBP* GetInstance()
	{
		if ( mSingleton ==  NULL )
		{
			mSingleton = new ShapeLBP();
		}
		return mSingleton;
	}

	void AlgrithomProcessor(Mat& srcImg, Mat& featureMap);
	void AlgrithomProcessor(Mat& srcImg, Mat& featureMap, Mat& featureVec);

	

	
private:
	ShapeLBP() {}	
	void GetCircurlarLBP(Mat& srcImg, Mat& dstImg);	// ��ȡԲ��LBP������
	void GetRotationLBP(Mat& featureMap);	// ��ȡ��ת����LBP������
	void GetUniformedLBP(Mat& featureMap);		// ��ȡ�ȼ�ģʽLBP������
	Mat GetLBPVec(Mat& featureMap);		// ��ȡ����ͼ�����������
	int getHopTimes(int n);	//	����һ����ֵ�Ķ����Ʊ�ʾ�����������
	Mat getLocalRegionLBPH(const Mat& src, int minValue, int maxValue, bool normed);	//	����ֲ�ͼ����ͳ����������

	static ShapeLBP* mSingleton;
	int neighbors = 8;	// ����Ƚϵ��ھ���������ֵ����Ϊ���ɸı䡣
	int radius = 1;		// �뾶��С�����ȿ��Ե��ڣ�Ĭ�ϳ�����1��
	bool rotation = 0;	// �Ƿ�����ת����ģʽ��Ĭ����0����������
	bool uniformed = 0;	// �Ƿ����ȼ�ģʽ��Ĭ����0����������
	int grid_x = 8;		// ��ͼ�����ֳ�grid_x�ݣ�Ĭ����8
	int grid_y = 8;		// ��ͼ������ֳ�grid_y�ݣ�Ĭ����8
	

};