/************************************************************************/
/* 主函数，测试各个算法是否可以运行                                                                     */
/************************************************************************/
#include "EnergyFeature.h"	// 包含能量特性父类

#include "EnergyLocalShannon.h"	// 包含灰度能量特性
#include "EnergyLocalStd.h"
#include "EnergyLocalMeanContrast.h"
#include "EnergySaliencyHC.h"	// 包含显著性特性
#include "EnergySaliencyRC.h"
#include "EnergySaliencySR.h"
#include "EnergySaliencyLC.h"
#include "EnergySaliencyFT.h"
#include "EnergyFrequency.h"	// 包含频谱能量特性

// 包含形状特性父类
#include "ShapeFeature.h"
#include "ShapeFractal.h"	// 包含分形特性
#include "ShapeLBP.h"		// 包含LBP纹理特征
#include "MotionOpticalFlow.h"

int main()
{
	Mat srcImg = imread("F:\\code_search\\MSRA10K_Imgs_GT\\Imgs\\147.jpg");
	Mat outImg;
	Mat featureVec;	// 测试LBP特征向量
	Mat srcImg1 = imread("F:\\数据\\video\\egtest02\\frame00004.jpg");
	Mat srcImg2 = imread("F:\\数据\\video\\egtest02\\frame00005.jpg");
	Mat featureMap;		// 测试光流法特征图
	

	// 灰度特性
	EnergyLocalShannon* p1 = EnergyLocalShannon::GetInstance();
	EnergyLocalStd* p2 = EnergyLocalStd::GetInstance();
	EnergyLocalMeanContrast* p3 = EnergyLocalMeanContrast::GetInstance();
	// 显著性特性
	EnergySaliencyHC* p4 = EnergySaliencyHC::GetInstance();
	EnergySaliencyRC* p5 = EnergySaliencyRC::GetInstance();
	EnergySaliencySR* p6 = EnergySaliencySR::GetInstance();
	EnergySaliencyLC* p7 = EnergySaliencyLC::GetInstance();
	EnergySaliencyFT* p8 = EnergySaliencyFT::GetInstance();
	// 频谱特性
	EnergyFrequency* p9 = EnergyFrequency::GetInstance();

	// 分形特性
	ShaprFractal* p10 = ShaprFractal::GetInstance();
	// LBP特征
	ShapeLBP* p11 = ShapeLBP::GetInstance();
	// 光流特征
	MotionOpticalFlow* p12 = MotionOpticalFlow::GetInstance();

	p12->AlgrithomProcessor(srcImg1, srcImg2, featureMap);
	namedWindow("in");
	imshow("in",srcImg2);
	namedWindow("out");
	imshow("out", featureMap);
	waitKey(0);
	return 0;
}