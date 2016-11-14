#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;

class ImageUtility
{
public:
	ImageUtility(){};
	ImageUtility(const Mat& im_);
	~ImageUtility(void){};

	void setImage(const Mat& im_);
	void setRadius(int r);

	Mat localVar();
	Mat localMean();

	cv::Mat cumsum(Mat &imSrc, int rc);
	Mat boxfilter(Mat &imSrc, int r);
	Mat guidedfilter(Mat &p, double eps );

private:
	Mat im;
	Mat im_var, im_mean, II_mean;
protected:
	Mat im_denom;
	int height, width;
	int radius;
};


class ImageUtilityColor
{
public:
	ImageUtilityColor(){};
	ImageUtilityColor(const vector<Mat> rgb);
	~ImageUtilityColor(){};

	void setImage(const vector<Mat> rgb);
	void setRadius(int r);
	
	Mat localVar(int c);
	Mat localMean(int c);
	Mat localMeanColorDiff();
protected:
	vector<ImageUtility> IUs;
	Mat offset;
	int cc;
	int height, width;
	int radius;
};