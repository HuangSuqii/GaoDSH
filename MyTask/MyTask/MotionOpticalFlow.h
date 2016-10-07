// Farneback dense optical flow calculate and show in Munsell system of colors  
// Author : Zouxy  
// Date   : 2013-3-15  
// HomePage : http://blog.csdn.net/zouxy09  
// Email  : zouxy09@qq.com  

// API calcOpticalFlowFarneback() comes from OpenCV, and this  
// 2D dense optical flow algorithm from the following paper:  
// Gunnar Farneback. "Two-Frame Motion Estimation Based on Polynomial Expansion".  
// And the OpenCV source code locate in ..\opencv2.4.3\modules\video\src\optflowgf.cpp  

#include <iostream>  
#include "opencv2/opencv.hpp"  

using namespace cv;
using namespace std;

#define UNKNOWN_FLOW_THRESH 1e9  

// Color encoding of flow vectors from:  
// http://members.shaw.ca/quadibloc/other/colint.htm  
// This code is modified from:  
// http://vision.middlebury.edu/flow/data/  

class MotionOpticalFlow
{
public:
	~MotionOpticalFlow() {}
	static MotionOpticalFlow* GetInstance()
	{
		if (mSingleton ==NULL)
		{
			mSingleton = new MotionOpticalFlow();
		}
		return mSingleton;
	}
	void AlgrithomProcessor(Mat& preImg, Mat& srcImg, Mat& featureMap);


private:
	MotionOpticalFlow() {}
	void makecolorwheel(vector<Scalar> &colorwheel);
	void motionToColor(Mat flow, Mat &color);

private:
	static MotionOpticalFlow* mSingleton;
};