#include "ImageUtility.h"


ImageUtility::ImageUtility( const Mat& im_ )
{
	im = im_.clone();
	height = im.size().height;
	width = im.size().width;
	
	setRadius(3);
}

void ImageUtility::setRadius( int r )
{
	radius = r;

	im.convertTo(im, CV_64F);
	if(im_denom.empty()){
		Mat one = Mat::ones(height, width, CV_64FC1);
		im_denom = boxfilter(one, radius);
	}

	im_mean.release();
	im_var.release();
}

void ImageUtility::setImage( const Mat& im_ )
{
	im = im_.clone();
	height = im.size().height;
	width = im.size().width;
	
	setRadius(3);
}


cv::Mat ImageUtility::cumsum(Mat &imSrc, int rc)
{
	if(!imSrc.data){
		return Mat();
	}
	int hei = imSrc.rows;
	int wid = imSrc.cols;
	Mat imCum = imSrc.clone();
	if( rc == 1){
		for( int i =1;i < hei; i++){
			for( int j = 0; j< wid; j++){
				imCum.at<double>(i,j) += imCum.at<double>(i-1,j);
			}
		}
	}

	if( rc == 2){
		for( int i =0;i < hei; i++){
			for( int j = 1; j< wid; j++){
				imCum.at<double>(i,j) += imCum.at<double>(i,j-1);
			}
		}
	}
	return imCum;
}

Mat ImageUtility::boxfilter(Mat &imSrc, int r){
	int hei = imSrc.rows;
	int wid = imSrc.cols;
	Mat imDst = Mat::zeros( hei, wid, CV_64FC1);
	Mat imCum = cumsum(imSrc,1);
	//imDst(1:r+1, :) = imCum(1+r:2*r+1, :);
	for( int i = 0; i<r+1; i++){
		for( int j=0; j<wid; j++ ){
			imDst.at<double>(i,j) = imCum.at<double>(i+r,j);
		}
	}
	//imDst(r+2:hei-r, :) = imCum(2*r+2:hei, :) - imCum(1:hei-2*r-1, :);
	for( int i =r+1; i<hei-r;i++){
		for( int j = 0; j<wid;j++){
			imDst.at<double>(i,j) = imCum.at<double>(i+r,j)-imCum.at<double>(i-r-1,j);
		}
	}
	//imDst(hei-r+1:hei, :) = repmat(imCum(hei, :), [r, 1]) - imCum(hei-2*r:hei-r-1, :);
	for( int i = hei-r; i< hei; i++){
		for( int j = 0; j< wid; j++){
			imDst.at<double>(i,j) = imCum.at<double>(hei-1,j)-imCum.at<double>(i-r-1,j);
		}
	}
	imCum = cumsum(imDst, 2);
	//imDst(:, 1:r+1) = imCum(:, 1+r:2*r+1);
	for( int i = 0; i<hei; i++){
		for( int j=0; j<r+1; j++ ){
			imDst.at<double>(i,j) = imCum.at<double>(i,j+r);
		}
	}
	//imDst(:, r+2:wid-r) = imCum(:, 2*r+2:wid) - imCum(:, 1:wid-2*r-1);
	for( int i =0 ; i<hei;i++){
		for( int j = r+1; j<wid-r ;j++ ){
			imDst.at<double>(i,j) = imCum.at<double>(i,j+r)-imCum.at<double>(i,j-r-1);
		}
	}
	//imDst(:, wid-r+1:wid) = repmat(imCum(:, wid), [1, r]) - imCum(:, wid-2*r:wid-r-1);
	for( int i = 0; i< hei; i++){
		for( int j = wid-r; j<wid; j++){
			imDst.at<double>(i,j) = imCum.at<double>(i,wid-1)-imCum.at<double>(i,j-r-1);
		}
	}
	return imDst;
}

cv::Mat ImageUtility::guidedfilter( Mat &p, double eps )
{
	localMean();
	localVar();
	//mean_p = boxfilter(p, r) ./ N;
	Mat mean_p(height, width, CV_64FC1);
	divide(boxfilter(p, radius), im_denom, mean_p);

	//mean_Ip = boxfilter(I.*p, r) ./ N;
	Mat mul_Ip(height, width, CV_64FC1);
	Mat mean_Ip(height, width, CV_64FC1);
	multiply(im,p,mul_Ip);
	divide(boxfilter(mul_Ip, radius), im_denom, mean_Ip);

	//cov_Ip = mean_Ip - mean_I .* mean_p
	//this is the covariance of (I, p) in each local patch.
	Mat mul_mean_Ip(height, width, CV_64FC1);
	Mat cov_Ip(height, width, CV_64FC1);
	multiply(im_mean, mean_p, mul_mean_Ip);
	subtract(mean_Ip, mul_mean_Ip, cov_Ip);

	

	//a = cov_Ip ./ (var_I + eps);
	Mat a(height, width, CV_64FC1);
	for( int i = 0; i< height; i++){
		double *p = im_var.ptr<double>(i);
		for( int j = 0; j< width; j++){
			p[j] = p[j] +eps; 	
		}
	}
	divide(cov_Ip, im_var, a);

	//b = mean_p - a .* mean_I;
	Mat a_mean_I(height ,width, CV_64FC1);
	Mat b(height ,width, CV_64FC1);
	multiply(a, im_mean, a_mean_I);
	subtract(mean_p, a_mean_I, b);

	//mean_a = boxfilter(a, r) ./ N;
	Mat mean_a(height, width, CV_64FC1);
	divide(boxfilter(a, radius), im_denom, mean_a);
	//mean_b = boxfilter(b, r) ./ N;
	Mat mean_b(height, width, CV_64FC1);
	divide(boxfilter(b, radius), im_denom, mean_b);

	//q = mean_a .* I + mean_b;
	Mat mean_a_I(height, width, CV_64FC1);
	Mat q(height, width, CV_64FC1);
	multiply(mean_a, im, mean_a_I);
	add(mean_a_I, mean_b, q);

	return q;
}

cv::Mat ImageUtility::localMean()
{	
	if(im_mean.empty()){
		divide(boxfilter(im, radius), im_denom, im_mean);
	}

	return im_mean;
}

cv::Mat ImageUtility::localVar()
{
	localMean();

	if(im_var.empty()){
		Mat mul_II(height, width, CV_64FC1);
		Mat II_mean(height, width, CV_64FC1);
		multiply(im, im, mul_II);
		divide(boxfilter(mul_II, radius), im_denom, II_mean);

		//var_I = mean_II - mean_I .* mean_I;
		Mat mul_mean_II(height, width, CV_64FC1);
		multiply(im_mean, im_mean, mul_mean_II);
		subtract(II_mean, mul_mean_II, im_var);
	}

	return im_var;
}


ImageUtilityColor::ImageUtilityColor( const vector<Mat> rgb )
{
	cc = rgb.size();
	IUs.resize(cc);
	for (int c=0; c<cc; c++)
	{
		IUs[c].setImage(rgb[c]);
	}
}

void ImageUtilityColor::setImage( const vector<Mat> rgb )
{
	cc = rgb.size();
	for (int c=0; c<cc; c++)
	{
		IUs[c].setImage(rgb[c]);
	}
}

void ImageUtilityColor::setRadius( int r )
{
	for (int c=0; c<cc; c++)
	{
		IUs[c].setRadius(r);
	}
}

cv::Mat ImageUtilityColor::localVar( int c )
{
	return IUs[c].localVar();
}

cv::Mat ImageUtilityColor::localMean( int c )
{
	return IUs[c].localMean();
}

// calc inter-color offset
cv::Mat ImageUtilityColor::localMeanColorDiff()
{
	if(offset.empty()){
		vector<Mat> tmp;
		for(int i=0; i<3; i++)
			for(int j=i+1;j<3; j++)
				tmp.push_back(IUs[i].localMean() - IUs[j].localMean());

		merge(tmp,offset);
	}

	return offset;
}
