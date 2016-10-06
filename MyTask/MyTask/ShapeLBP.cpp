#include "ShapeLBP.h"

ShapeLBP* ShapeLBP::mSingleton = NULL;

void ShapeLBP::AlgrithomProcessor(Mat& srcImg, Mat& featureMap)
{
	Mat featureVec;
	AlgrithomProcessor(srcImg, featureMap, featureVec);
}

void ShapeLBP::AlgrithomProcessor(Mat& srcImg, Mat& featureMap, Mat& featureVec)
{
	Mat srcImg_gray;
	if (srcImg.channels() == 3)
		cvtColor(srcImg, srcImg_gray, CV_BGR2GRAY);
	else
		srcImg_gray = srcImg;

	GetCircurlarLBP(srcImg_gray, featureMap);
	if (rotation)
		GetRotationLBP(featureMap);
	if (uniformed)
		GetUniformedLBP(featureMap);
	featureVec = GetLBPVec(featureMap);
}

void ShapeLBP:: GetCircurlarLBP(Mat& srcImg, Mat& featureMap)
{
	int nr = srcImg.rows - 2 * radius;
	int nc = srcImg.cols - 2 * radius;
	featureMap.create(nr, nc, CV_8UC1);
	for (int i = radius; i < srcImg.rows - radius; i++)
	{
		for (int j = radius; j < srcImg.cols - radius; j++)
		{
			uchar lbpcode = 0;
			for (int k = 0; k < neighbors; k++)
			{
				//根据公式计算第k个采样点的坐标，这个地方可以优化，不必每次都进行计算radius*cos，radius*sin
				float x = i + static_cast<float>(radius * \
					cos(2.0 * CV_PI * k / neighbors));
				float y = j - static_cast<float>(radius * \
					sin(2.0 * CV_PI * k / neighbors));
				//根据取整结果进行双线性插值，得到第k个采样点的灰度值

				//1.分别对x，y进行上下取整
				int x1 = static_cast<int>(floor(x));
				int x2 = static_cast<int>(ceil(x));
				int y1 = static_cast<int>(floor(y));
				int y2 = static_cast<int>(ceil(y));

				//2.计算四个点(x1,y1),(x1,y2),(x2,y1),(x2,y2)的权重

				//将坐标映射到0-1之间
				float tx = x - x1;
				float ty = y - y1;
				//根据0-1之间的x，y的权重计算公式计算权重
				float w1 = (1 - tx) * (1 - ty);
				float w2 = tx  * (1 - ty);
				float w3 = (1 - tx) *    ty;
				float w4 = tx  *    ty;
				//3.根据双线性插值公式计算第k个采样点的灰度值
				float neighbor = srcImg.at<uchar>(x1, y1) * w1 + srcImg.at<uchar>(x1, y2) *w2 \
					+ srcImg.at<uchar>(x2, y1) * w3 + srcImg.at<uchar>(x2, y2) *w4;
				//通过比较获得LBP值，并按顺序排列起来
				lbpcode |= ( neighbor > srcImg.at<uchar>(i, j) )<< (neighbors - k - 1);		// that's cool
			}
			featureMap.at<uchar>(i - radius, j - radius) = lbpcode;
		}
	}
}

void ShapeLBP::GetRotationLBP(Mat& featureMap)
{
	//进行旋转不变处理
	for (int i = 0; i < featureMap.rows; i++)
	{
		for (int j = 0; j < featureMap.cols; j++)
		{
			unsigned char currentValue = featureMap.at<uchar>(i, j);
			unsigned char minValue = currentValue;
			for (int k = 1; k < neighbors; k++)
			{
				//循环左移
				unsigned char temp = (currentValue >> (neighbors - k)) | (currentValue << k);
				if (temp < minValue)
				{
					minValue = temp;
				}
			}
			featureMap.at<uchar>(i, j) = minValue;
		}
	}
}

void ShapeLBP::GetUniformedLBP(Mat& featureMap)
{
	uchar temp = 1;
	uchar table[256] = { 0 };
	for (int i = 0; i < 256; i++)
	{
		if (getHopTimes(i) < 3)
		{
			table[i] = temp;
			temp++;
		}
	}
	for (int i = 0; i < featureMap.rows; i++)
	{
		for (int j = 0; j < featureMap.cols; j++)
		{
			featureMap.at<uchar>(i, j) = table[featureMap.at<uchar>(i, j)];
		}
	}
}

int ShapeLBP::getHopTimes(int n)
{
	int count = 0;
	bitset<8> binaryCode = n;
	for (int i = 0; i < 8; i++)
	{
		if (binaryCode[i] != binaryCode[(i + 1) % 8])
		{
			count++;
		}
	}
	return count;
}

Mat ShapeLBP::GetLBPVec(Mat& featureMap)
{
	int width = featureMap.cols / grid_x;
	int height = featureMap.rows / grid_y;
	int numPatterns;
	//定义LBPH的行和列，grid_x*grid_y表示将图像分割成这么些块，numPatterns表示LBP值的模式种类
	if (uniformed)
		numPatterns = 59;
	else
		numPatterns = 256;
	Mat result = Mat::zeros(grid_x * grid_y, numPatterns, CV_32FC1);
	if (featureMap.empty())
	{
		return result.reshape(1, 1);
	}
	int resultRowIndex = 0;
	//对图像进行分割，分割成grid_x*grid_y块，grid_x，grid_y默认为8
	for (int i = 0; i < grid_x; i++)
	{
		for (int j = 0; j < grid_y; j++)
		{
			//图像分块
			Mat featureMap_cell = Mat(featureMap, Range(i*height, (i + 1)*height), Range(j*width, (j + 1)*width));
			//计算直方图
			Mat hist_cell = getLocalRegionLBPH(featureMap_cell, 0, (numPatterns - 1), true);
			//将直方图放到result中
			Mat rowResult = result.row(resultRowIndex);
			hist_cell.reshape(1, 1).convertTo(rowResult, CV_32FC1);
			resultRowIndex++;
		}
	}
	return result.reshape(1, 1);
}

Mat ShapeLBP::getLocalRegionLBPH(const Mat& src, int minValue, int maxValue, bool normed)
{
	//定义存储直方图的矩阵
	Mat result;
	//计算得到直方图bin的数目，直方图数组的大小
	int histSize = maxValue - minValue + 1;
	//定义直方图每一维的bin的变化范围
	float range[] = { static_cast<float>(minValue), static_cast<float>(maxValue + 1) };
	//定义直方图所有bin的变化范围
	const float* ranges = { range };
	//计算直方图，src是要计算直方图的图像，1是要计算直方图的图像数目，0是计算直方图所用的图像的通道序号，从0索引
	//Mat()是要用的掩模，result为输出的直方图，1为输出的直方图的维度，histSize直方图在每一维的变化范围
	//ranges，所有直方图的变化范围（起点和终点）
	calcHist(&src, 1, 0, Mat(), result, 1, &histSize, &ranges, true, false);
	//归一化
	if (normed)
	{
		result /= (int)src.total();
	}
	//结果表示成只有1行的矩阵
	return result.reshape(1, 1);
}