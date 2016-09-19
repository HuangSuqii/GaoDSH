#pragma once
/**************************************
							��������
���е����������㷨�඼�Ǽ̳����������
�࣬�����а���mat����ָ���ת��������
**************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using cv::Mat;

//�����������Եĸ���
class EnergyFeature
{
public:
	EnergyFeature() {}
	~EnergyFeature() {}
	virtual void AlgrithomProcessor(Mat& srcImg, Mat& desImg) = 0;

protected:
	unsigned char* mat2GrayImgPointer(const Mat& mat, Mat& desMat)
	{
		cv::cvtColor(mat, desMat, CV_BGR2GRAY);		// ˳���ʼ��desMat
		int nr = desMat.rows;
		int nc = desMat.cols;
		unsigned char* covertimage = new unsigned char[nr * nc];							     // ����һ��һάͼ����Ϊʵ���������㷨������
		unsigned char* p;
		int i, j;
		for (i = 0; i<nr; i++)
		{
			p = desMat.ptr<unsigned char>(i);			  // ��ָ��ָ��mat���о����ÿ������
			for (j = 0; j<nc; j++)
			{
				covertimage[i*nc + j] = p[j];							  // ��mat���еľ���ת����һ��һάͼ��
			}
		}
		return covertimage;
	}


	
};