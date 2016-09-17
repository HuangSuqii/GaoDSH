#include "EnergySaliencyHC.h"

EnergySaliencyHC* EnergySaliencyHC::mSingleton = NULL;
const int EnergySaliencyHC::DefaultNums[3] = { 12, 12, 12 };

void EnergySaliencyHC::AlgrithomProcessor(Mat& srcImg, Mat& desImg)
{
	srcImg.convertTo(srcImg, CV_32FC3, 1.0 / 255);
	desImg = GetHC(srcImg);
}

Mat EnergySaliencyHC::GetHC(CMat &img3f)
{
	// Quantize colors and
	Mat idx1i, binColor3f, colorNums1i, _colorSal;
	Quantize(img3f, idx1i, binColor3f, colorNums1i);
	cvtColor(binColor3f, binColor3f, CV_BGR2Lab);

	GetHC(binColor3f, colorNums1i, _colorSal);
	float* colorSal = (float*)(_colorSal.data);
	Mat salHC1f(img3f.size(), CV_32F);
	for (int r = 0; r < img3f.rows; r++){
		float* salV = salHC1f.ptr<float>(r);
		int* _idx = idx1i.ptr<int>(r);
		for (int c = 0; c < img3f.cols; c++)
			salV[c] = colorSal[_idx[c]];
	}
	GaussianBlur(salHC1f, salHC1f, Size(3, 3), 0);
	normalize(salHC1f, salHC1f, 0, 1, NORM_MINMAX);
	return salHC1f;
}
void EnergySaliencyHC::GetHC(CMat &binColor3f, CMat &colorNums1i, Mat &_colorSal)
{
	Mat weight1f;
	normalize(colorNums1i, weight1f, 1, 0, NORM_L1, CV_32F);

	int binN = binColor3f.cols;
	_colorSal = Mat::zeros(1, binN, CV_32F);
	float* colorSal = (float*)(_colorSal.data);
	vector<vector<CostfIdx>> similar(binN); // Similar color: how similar and their index
	Vec3f* color = (Vec3f*)(binColor3f.data);
	float *w = (float*)(weight1f.data);
	for (int i = 0; i < binN; i++){
		vector<CostfIdx> &similari = similar[i];
		similari.push_back(make_pair(0.f, i));
		for (int j = 0; j < binN; j++){
			if (i == j)
				continue;
			float dij = vecDist<float, 3>(color[i], color[j]);
			similari.push_back(make_pair(dij, j));
			colorSal[i] += w[j] * dij;
		}
		sort(similari.begin(), similari.end());
	}

	SmoothSaliency(_colorSal, 0.25f, similar);
}

int EnergySaliencyHC::Quantize(CMat& img3f, Mat &idx1i, Mat &_color3f, Mat &_colorNum, double ratio, const int clrNums[3])
{
	float clrTmp[3] = { clrNums[0] - 0.0001f, clrNums[1] - 0.0001f, clrNums[2] - 0.0001f };
	int w[3] = { clrNums[1] * clrNums[2], clrNums[2], 1 };

	CV_Assert(img3f.data != NULL);
	idx1i = Mat::zeros(img3f.size(), CV_32S);
	int rows = img3f.rows, cols = img3f.cols;
	if (img3f.isContinuous() && idx1i.isContinuous()){
		cols *= rows;
		rows = 1;
	}

	// Build color pallet
	map<int, int> pallet;
	for (int y = 0; y < rows; y++)
	{
		const float* imgData = img3f.ptr<float>(y);
		int* idx = idx1i.ptr<int>(y);
		for (int x = 0; x < cols; x++, imgData += 3)
		{
			idx[x] = (int)(imgData[0] * clrTmp[0])*w[0] + (int)(imgData[1] * clrTmp[1])*w[1] + (int)(imgData[2] * clrTmp[2]);
			pallet[idx[x]] ++;
		}
	}

	// Find significant colors
	int maxNum = 0;
	{
		int count = 0;
		vector<pair<int, int>> num; // (num, color) pairs in num
		num.reserve(pallet.size());
		for (map<int, int>::iterator it = pallet.begin(); it != pallet.end(); it++)
			num.push_back(pair<int, int>(it->second, it->first)); // (color, num) pairs in pallet
		sort(num.begin(), num.end(), std::greater<pair<int, int>>());

		maxNum = (int)num.size();
		int maxDropNum = cvRound(rows * cols * (1 - ratio));
		for (int crnt = num[maxNum - 1].first; crnt < maxDropNum && maxNum > 1; maxNum--)
			crnt += num[maxNum - 2].first;
		maxNum = min(maxNum, 256); // To avoid very rarely case
		if (maxNum <= 10)
			maxNum = min(10, (int)num.size());

		pallet.clear();
		for (int i = 0; i < maxNum; i++)
			pallet[num[i].second] = i;

		vector<Vec3i> color3i(num.size());
		for (unsigned int i = 0; i < num.size(); i++)
		{
			color3i[i][0] = num[i].second / w[0];
			color3i[i][1] = num[i].second % w[0] / w[1];
			color3i[i][2] = num[i].second % w[1];
		}

		for (unsigned int i = maxNum; i < num.size(); i++)
		{
			int simIdx = 0, simVal = INT_MAX;
			for (int j = 0; j < maxNum; j++)
			{
				int d_ij = vecSqrDist<int, 3>(color3i[i], color3i[j]);
				if (d_ij < simVal)
					simVal = d_ij, simIdx = j;
			}
			pallet[num[i].second] = pallet[num[simIdx].second];
		}
	}

	_color3f = Mat::zeros(1, maxNum, CV_32FC3);
	_colorNum = Mat::zeros(_color3f.size(), CV_32S);

	Vec3f* color = (Vec3f*)(_color3f.data);
	int* colorNum = (int*)(_colorNum.data);
	for (int y = 0; y < rows; y++)
	{
		const Vec3f* imgData = img3f.ptr<Vec3f>(y);
		int* idx = idx1i.ptr<int>(y);
		for (int x = 0; x < cols; x++)
		{
			idx[x] = pallet[idx[x]];
			color[idx[x]] += imgData[x];
			colorNum[idx[x]] ++;
		}
	}
	for (int i = 0; i < _color3f.cols; i++)
		color[i] /= (float)colorNum[i];

	return _color3f.cols;
}

void EnergySaliencyHC::SmoothSaliency(Mat &sal1f, float delta, const vector<vector<CostfIdx>> &similar)
{
	Mat colorNum1i = Mat::ones(sal1f.size(), CV_32SC1);
	SmoothSaliency(colorNum1i, sal1f, delta, similar);
}

void EnergySaliencyHC::SmoothSaliency(CMat &colorNum1i, Mat &sal1f, float delta, const vector<vector<CostfIdx>> &similar)
{
	if (sal1f.cols < 2)
		return;
	CV_Assert(sal1f.rows == 1 && sal1f.type() == CV_32FC1);
	CV_Assert(colorNum1i.size() == sal1f.size() && colorNum1i.type() == CV_32SC1);

	int binN = sal1f.cols;
	Mat newSal1d = Mat::zeros(1, binN, CV_64FC1);
	float *sal = (float*)(sal1f.data);
	double *newSal = (double*)(newSal1d.data);
	int *pW = (int*)(colorNum1i.data);

	// Distance based smooth
	int n = max(cvRound(binN * delta), 2);
	vecD dist(n, 0), val(n), w(n);
	for (int i = 0; i < binN; i++){
		const vector<CostfIdx> &similari = similar[i];
		double totalDist = 0, totoalWeight = 0;
		for (int j = 0; j < n; j++){
			int ithIdx = similari[j].second;
			dist[j] = similari[j].first;
			val[j] = sal[ithIdx];
			w[j] = pW[ithIdx];
			totalDist += dist[j];
			totoalWeight += w[j];
		}
		double valCrnt = 0;
		for (int j = 0; j < n; j++)
			valCrnt += val[j] * (totalDist - dist[j]) * w[j];

		newSal[i] = valCrnt / (totalDist * totoalWeight);
	}
	normalize(newSal1d, sal1f, 0, 1, NORM_MINMAX, CV_32FC1);
}