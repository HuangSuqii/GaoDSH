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
				//���ݹ�ʽ�����k������������꣬����ط������Ż�������ÿ�ζ����м���radius*cos��radius*sin
				float x = i + static_cast<float>(radius * \
					cos(2.0 * CV_PI * k / neighbors));
				float y = j - static_cast<float>(radius * \
					sin(2.0 * CV_PI * k / neighbors));
				//����ȡ���������˫���Բ�ֵ���õ���k��������ĻҶ�ֵ

				//1.�ֱ��x��y��������ȡ��
				int x1 = static_cast<int>(floor(x));
				int x2 = static_cast<int>(ceil(x));
				int y1 = static_cast<int>(floor(y));
				int y2 = static_cast<int>(ceil(y));

				//2.�����ĸ���(x1,y1),(x1,y2),(x2,y1),(x2,y2)��Ȩ��

				//������ӳ�䵽0-1֮��
				float tx = x - x1;
				float ty = y - y1;
				//����0-1֮���x��y��Ȩ�ؼ��㹫ʽ����Ȩ��
				float w1 = (1 - tx) * (1 - ty);
				float w2 = tx  * (1 - ty);
				float w3 = (1 - tx) *    ty;
				float w4 = tx  *    ty;
				//3.����˫���Բ�ֵ��ʽ�����k��������ĻҶ�ֵ
				float neighbor = srcImg.at<uchar>(x1, y1) * w1 + srcImg.at<uchar>(x1, y2) *w2 \
					+ srcImg.at<uchar>(x2, y1) * w3 + srcImg.at<uchar>(x2, y2) *w4;
				//ͨ���Ƚϻ��LBPֵ������˳����������
				lbpcode |= ( neighbor > srcImg.at<uchar>(i, j) )<< (neighbors - k - 1);		// that's cool
			}
			featureMap.at<uchar>(i - radius, j - radius) = lbpcode;
		}
	}
}

void ShapeLBP::GetRotationLBP(Mat& featureMap)
{
	//������ת���䴦��
	for (int i = 0; i < featureMap.rows; i++)
	{
		for (int j = 0; j < featureMap.cols; j++)
		{
			unsigned char currentValue = featureMap.at<uchar>(i, j);
			unsigned char minValue = currentValue;
			for (int k = 1; k < neighbors; k++)
			{
				//ѭ������
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
	//����LBPH���к��У�grid_x*grid_y��ʾ��ͼ��ָ����ôЩ�飬numPatterns��ʾLBPֵ��ģʽ����
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
	//��ͼ����зָ�ָ��grid_x*grid_y�飬grid_x��grid_yĬ��Ϊ8
	for (int i = 0; i < grid_x; i++)
	{
		for (int j = 0; j < grid_y; j++)
		{
			//ͼ��ֿ�
			Mat featureMap_cell = Mat(featureMap, Range(i*height, (i + 1)*height), Range(j*width, (j + 1)*width));
			//����ֱ��ͼ
			Mat hist_cell = getLocalRegionLBPH(featureMap_cell, 0, (numPatterns - 1), true);
			//��ֱ��ͼ�ŵ�result��
			Mat rowResult = result.row(resultRowIndex);
			hist_cell.reshape(1, 1).convertTo(rowResult, CV_32FC1);
			resultRowIndex++;
		}
	}
	return result.reshape(1, 1);
}

Mat ShapeLBP::getLocalRegionLBPH(const Mat& src, int minValue, int maxValue, bool normed)
{
	//����洢ֱ��ͼ�ľ���
	Mat result;
	//����õ�ֱ��ͼbin����Ŀ��ֱ��ͼ����Ĵ�С
	int histSize = maxValue - minValue + 1;
	//����ֱ��ͼÿһά��bin�ı仯��Χ
	float range[] = { static_cast<float>(minValue), static_cast<float>(maxValue + 1) };
	//����ֱ��ͼ����bin�ı仯��Χ
	const float* ranges = { range };
	//����ֱ��ͼ��src��Ҫ����ֱ��ͼ��ͼ��1��Ҫ����ֱ��ͼ��ͼ����Ŀ��0�Ǽ���ֱ��ͼ���õ�ͼ���ͨ����ţ���0����
	//Mat()��Ҫ�õ���ģ��resultΪ�����ֱ��ͼ��1Ϊ�����ֱ��ͼ��ά�ȣ�histSizeֱ��ͼ��ÿһά�ı仯��Χ
	//ranges������ֱ��ͼ�ı仯��Χ�������յ㣩
	calcHist(&src, 1, 0, Mat(), result, 1, &histSize, &ranges, true, false);
	//��һ��
	if (normed)
	{
		result /= (int)src.total();
	}
	//�����ʾ��ֻ��1�еľ���
	return result.reshape(1, 1);
}