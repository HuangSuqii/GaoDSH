#include "EnergyFeature.h"

#include "EnergyLocalShannon.h"
#include "EnergyLocalStd.h"
#include "EnergyLocalMeanContrast.h"

#include "EnergySaliencyHC.h"
#include "EnergySaliencyRC.h"
#include "EnergySaliencySR.h"
#include "EnergySaliencyLC.h"
#include "EnergySaliencyFT.h"

#include "EnergyFrequency.h"

int main()
{
	Mat srcImg = imread("F:\\code_search\\MSRA10K_Imgs_GT\\Imgs\\147.jpg");
	//Mat srcImg = imread("test1.jpg");
	Mat outImg;

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

	p9->AlgrithomProcessor(srcImg, outImg);
	namedWindow("in");
	imshow("in",srcImg);
	namedWindow("out");
	imshow("out", outImg);
	waitKey(0);
	return 0;
}