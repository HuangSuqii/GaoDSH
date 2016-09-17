#pragma once
/************************************************************************/
/* �������㷨����ͷ�ļ�
	��������������㷨ʱ��������Ҫ�õ������Լ�����ĺ������ࡢ��ʶ����
	Ҳ��Ҫ�õ��ܶ�ͷ�ļ������ļ���������Щ�����������
	�κ����������ͷ�ļ��У���Ҫ�������ͷ�ļ���*/
/************************************************************************/

#include "EnergyFeature.h"
#include <vector>
#include <functional>
#include <map>
#include <utility>
#include<algorithm>

using namespace std;
using namespace cv;

#define ForPoints2(pnt, xS, yS, xE, yE)	for (Point pnt(0, (yS)); pnt.y != (yE); pnt.y++) for (pnt.x = (xS); pnt.x != (xE); pnt.x++)
#define THRESHOLD(size, c) (c/size)

typedef const cv::Mat CMat;
typedef std::pair<float, int> CostfIdx;
typedef std::vector<double> vecD;
typedef std::vector<int> vecI;
typedef unsigned char byte;

template<typename T> inline T sqr(T x) { return x * x; } // out of range risk for T = byte, ...

template<class T, int D> inline T vecSqrDist(const cv::Vec<T, D> &v1, const cv::Vec<T, D> &v2)  // out of range risk for T = byte, ...
{
	T s = 0;
	for (int i = 0; i < D; i++)
		s += sqr(v1[i] - v2[i]);
	return s;
}

template<class T, int D> inline T    vecDist(const Vec<T, D> &v1, const Vec<T, D> &v2)  // out of range risk for T = byte, ...
{
	return sqrt(vecSqrDist(v1, v2));
}
