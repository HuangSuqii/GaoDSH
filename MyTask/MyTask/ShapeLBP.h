#pragma  once
/************************************************************************/
/* LBP特征
	输出是LBP特征图像，以及整个图像的LBP特征向量*/
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

	void GetCircurlarLBP(Mat& srcImg, Mat& dstImg);
	void GetRotationLBP(Mat& featureMap);
	void GetUniformedLBP(Mat& featureMap);
	Mat GetLBPVec(Mat& featureMap);

	
private:
	ShapeLBP() {}
	int getHopTimes(int n);
	Mat getLocalRegionLBPH(const Mat& src, int minValue, int maxValue, bool normed);

	static ShapeLBP* mSingleton;

	int radius = 2;
	bool rotation = 0;
	bool uniformed = 1;
	int grid_x = 8;
	int grid_y = 8;
	int neighbors = 8;

};