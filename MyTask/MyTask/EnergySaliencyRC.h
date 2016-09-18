#pragma once
#include "EnergyFeature.h"
#include "EnergySaliencyUtil.h"
#include <cmath>

typedef struct {
	float w;
	int a, b;
} edge;

typedef struct {
	int rank;
	int p;
	int size;
} uni_elt;

template<class T> inline T pntSqrDist(const Point_<T> &p1, const Point_<T> &p2) { 
	return sqr(p1.x - p2.x) + sqr(p1.y - p2.y); 
} // out of range risk for T = byte, ...

const double EPS = 1e-200;		// Epsilon (zero value)

class EnergySaliencyRC : EnergyFeature
{
public:
	~EnergySaliencyRC()
	{
		if (mSingleton)
		{
			delete mSingleton;
		}
	}
	static EnergySaliencyRC* GetInstance()
	{
		if (mSingleton == NULL)
		{
			mSingleton = new EnergySaliencyRC();
		}
		return mSingleton;
	}
	void AlgrithomProcessor(Mat& srcImg, Mat& desImg);



private:
	EnergySaliencyRC() {}
	static EnergySaliencyRC* mSingleton;

	struct Region{
		Region() { pixNum = 0; ad2c = Point2d(0, 0); }
		int pixNum;  // Number of pixels
		vector<CostfIdx> freIdx;  // Frequency of each color and its index
		Point2d centroid;
		Point2d ad2c; // Average distance to image center
	};
	static void BuildRegions(CMat& regIdx1i, vector<Region> &regs, CMat &colorIdx1i, int colorNum);
	static void RegionContrast(const vector<Region> &regs, CMat &color3fv, Mat& regSal1d, double sigmaDist);

	static int Quantize(CMat& img3f, Mat &idx1i, Mat &_color3f, Mat &_colorNum, double ratio = 0.95, const int colorNums[3] = DefaultNums);
	static const int DefaultNums[3];

	static void SmoothSaliency(CMat &colorNum1i, Mat &sal1f, float delta, const vector<vector<CostfIdx>> &similar);
	static Mat GetBorderReg(CMat &idx1i, int regNum, double ratio, double thr);
	static void SmoothByHist(CMat &img3f, Mat &sal1f, float delta);
	static void SmoothByRegion(Mat &sal1f, CMat &idx1i, int regNum, bool bNormalize = true);

	// Region Contrast [5]
	static Mat GetRC(CMat &img3f);
	static Mat GetRC(CMat &img3f, CMat &idx1i, int regNum, double sigmaDist = 0.4);
	static Mat GetRC(CMat &img3f, double sigmaDist, double segK, int segMinSize, double segSigma);

	class universe 
	{
	public:
		universe(int elements);
		~universe();
		int find(int x);
		void join(int x, int y);
		int size(int x) const { return elts[x].size; }
		int nu_sets() const { return num; }

	private:
		uni_elt *elts;
		int num;
	};

	static inline float diff(CMat &img3f, int x1, int y1, int x2, int y2)
	{
		const Vec3f &p1 = img3f.at<Vec3f>(y1, x1);
		const Vec3f &p2 = img3f.at<Vec3f>(y2, x2);
		return sqrt(sqr(p1[0] - p2[0]) + sqr(p1[1] - p2[1]) + sqr(p1[2] - p2[2]));
	}

	static universe* segment_graph(int nu_vertices, int nu_edges, edge *edges, float c);

	static int SegmentImage(CMat &_src3f, Mat &pImgInd, double sigma, double c, int min_size);

};