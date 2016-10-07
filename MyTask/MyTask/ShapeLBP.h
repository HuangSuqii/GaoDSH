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

	

	
private:
	ShapeLBP() {}	
	void GetCircurlarLBP(Mat& srcImg, Mat& dstImg);	// 获取圆形LBP特征。
	void GetRotationLBP(Mat& featureMap);	// 获取旋转不变LBP特征。
	void GetUniformedLBP(Mat& featureMap);		// 获取等价模式LBP特征。
	Mat GetLBPVec(Mat& featureMap);		// 获取整幅图像的特征向量
	int getHopTimes(int n);	//	计算一个数值的二进制表示的跳变次数。
	Mat getLocalRegionLBPH(const Mat& src, int minValue, int maxValue, bool normed);	//	计算局部图像块的统计特征向量

	static ShapeLBP* mSingleton;
	int neighbors = 8;	// 参与比较的邻居数量，该值设置为不可改变。
	int radius = 1;		// 半径大小，长度可以调节，默认长度是1。
	bool rotation = 0;	// 是否开启旋转不变模式，默认是0，不开启。
	bool uniformed = 0;	// 是否开启等价模式，默认是0，不开启。
	int grid_x = 8;		// 将图像横向分成grid_x份，默认是8
	int grid_y = 8;		// 将图像纵向分成grid_y份，默认是8
	

};