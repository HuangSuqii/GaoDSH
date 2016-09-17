#pragma once
/************************************************************************/
/* 显著性算法工具头文件
	调用显著性相关算法时，经常需要用到作者自己定义的函数、类、标识符，
	也需要用到很多头文件。此文件涵盖了这些定义和声明。
	任何显著性类的头文件中，需要包含这个头文件。*/
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
