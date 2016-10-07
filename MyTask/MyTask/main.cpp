/************************************************************************/
/* �����������Ը����㷨�Ƿ��������                                                                     */
/************************************************************************/
#include "EnergyFeature.h"	// �����������Ը���

#include "EnergyLocalShannon.h"	// �����Ҷ���������
#include "EnergyLocalStd.h"
#include "EnergyLocalMeanContrast.h"
#include "EnergySaliencyHC.h"	// ��������������
#include "EnergySaliencyRC.h"
#include "EnergySaliencySR.h"
#include "EnergySaliencyLC.h"
#include "EnergySaliencyFT.h"
#include "EnergyFrequency.h"	// ����Ƶ����������

// ������״���Ը���
#include "ShapeFeature.h"
#include "ShapeFractal.h"	// ������������
#include "ShapeLBP.h"		// ����LBP��������
#include "MotionOpticalFlow.h"

int main()
{
	Mat srcImg = imread("F:\\code_search\\MSRA10K_Imgs_GT\\Imgs\\147.jpg");
	Mat outImg;
	Mat featureVec;	// ����LBP��������
	Mat srcImg1 = imread("F:\\����\\video\\egtest02\\frame00004.jpg");
	Mat srcImg2 = imread("F:\\����\\video\\egtest02\\frame00005.jpg");
	Mat featureMap;		// ���Թ���������ͼ
	

	// �Ҷ�����
	EnergyLocalShannon* p1 = EnergyLocalShannon::GetInstance();
	EnergyLocalStd* p2 = EnergyLocalStd::GetInstance();
	EnergyLocalMeanContrast* p3 = EnergyLocalMeanContrast::GetInstance();
	// ����������
	EnergySaliencyHC* p4 = EnergySaliencyHC::GetInstance();
	EnergySaliencyRC* p5 = EnergySaliencyRC::GetInstance();
	EnergySaliencySR* p6 = EnergySaliencySR::GetInstance();
	EnergySaliencyLC* p7 = EnergySaliencyLC::GetInstance();
	EnergySaliencyFT* p8 = EnergySaliencyFT::GetInstance();
	// Ƶ������
	EnergyFrequency* p9 = EnergyFrequency::GetInstance();

	// ��������
	ShaprFractal* p10 = ShaprFractal::GetInstance();
	// LBP����
	ShapeLBP* p11 = ShapeLBP::GetInstance();
	// ��������
	MotionOpticalFlow* p12 = MotionOpticalFlow::GetInstance();

	p12->AlgrithomProcessor(srcImg1, srcImg2, featureMap);
	namedWindow("in");
	imshow("in",srcImg2);
	namedWindow("out");
	imshow("out", featureMap);
	waitKey(0);
	return 0;
}