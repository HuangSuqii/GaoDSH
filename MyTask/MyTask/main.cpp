#include "EnergyFeature.h"

#include "EnergyLocalShannon.h"
#include "EnergyLocalStd.h"
#include "EnergyLocalMeanContrast.h"

#include "EnergySaliencyHC.h"
//#include "EnergySaliencyRC.h"
#include "EnergySaliencySR.h"
#include "EnergySaliencyLC.h"
#include "EnergySaliencyFT.h"

int main()
{
	Mat srcImg = imread("F:\\code_search\\MSRA10K_Imgs_GT\\Imgs\\147.jpg");
	Mat outImg;

	// �Ҷ�����
	EnergyLocalShannon* p1 = EnergyLocalShannon::GetInstance();
	EnergyLocalStd* p2 = EnergyLocalStd::GetInstance();
	EnergyLocalMeanContrast* p3 = EnergyLocalMeanContrast::GetInstance();

	// ����������
	EnergySaliencyHC* p4 = EnergySaliencyHC::GetInstance();
	//EnergySaliencyRC* p5 = EnergySaliencyRC::GetInstance();
	EnergySaliencySR* p6 = EnergySaliencySR::GetInstance();
	EnergySaliencyLC* p7 = EnergySaliencyLC::GetInstance();
	EnergySaliencyFT* p8 = EnergySaliencyFT::GetInstance();

	p3->AlgrithomProcessor(srcImg, outImg);
	namedWindow("in");
	imshow("in",srcImg);
	namedWindow("out");
	imshow("out", outImg);
	waitKey(0);
	return 0;
}