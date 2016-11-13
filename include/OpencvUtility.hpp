/*
Utilities for manipulating files and folders on Windows. 

Ketan Tang, tkt@ust.hk
*/

# pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>  // for setprecision
#include <stdlib.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;


namespace tkt{
	template<class T> void print(Mat M, int precision){
		cout<<"[";
		for (int i=0; i<M.size().height; i++)
			for (int j=0; j<M.size().width; j++)
			{
				cout<<setprecision(precision)<<M.at<T>(i,j);
				if(j!=M.size().width-1)
					cout<<",\t";
				else
					cout<<endl;
			}
			cout<<"]"<<endl;
	}

	void imshowMany(const std::string& _winName, const vector<Mat>& _imgs)  
	{  
		int nImg = (int)_imgs.size();  

		Mat dispImg;  

		int size;  
		int x, y;  

		// w - Maximum number of images in a row   
		// h - Maximum number of images in a column   
		int w, h;  
		// scale - How much we have to resize the image  
		float scale;  
		int max;  

		if (nImg <= 0)   
		{  
			printf("Number of arguments too small....\n");  
			return;  
		}  
		else if (nImg > 12)  
		{  
			printf("Number of arguments too large....\n");  
			return;  
		}  

		else if (nImg == 1)  
		{  
			w = h = 1;  
			size = 300;  
		}  
		else if (nImg == 2)  
		{  
			w = 2; h = 1;  
			size = 300;  
		}  
		else if (nImg == 3 || nImg == 4)  
		{  
			w = 2; h = 2;  
			size = 300;  
		}  
		else if (nImg == 5 || nImg == 6)  
		{  
			w = 3; h = 2;  
			size = 200;  
		}  
		else if (nImg == 7 || nImg == 8)  
		{  
			w = 4; h = 2;  
			size = 200;  
		}  
		else  
		{  
			w = 4; h = 3;  
			size = 150;  
		}  

		dispImg.create(Size(100 + size*w, 60 + size*h), CV_8UC3);  

		for (int i= 0, m=20, n=20; i<nImg; i++, m+=(20+size))  
		{  
			x = _imgs[i].cols;  
			y = _imgs[i].rows;  

			max = (x > y)? x: y;  
			scale = (float) ( (float) max / size );  

			if (i%w==0 && m!=20)  
			{  
				m = 20;  
				n += 20+size;  
			}  

			Mat imgROI = dispImg(Rect(m, n, (int)(x/scale), (int)(y/scale)));  
			resize(_imgs[i], imgROI, Size((int)(x/scale), (int)(y/scale)));  
		}  

		namedWindow(_winName);  
		imshow(_winName, dispImg);  
	}  

	Mat matread(FILE* fid, const char* format){
		int height=0, width=0;

		char aline[260];
		fgets(aline, 260, fid);
		sscanf(aline, "%d,%d", &height, &width);
		if(width>260){
			printf("matread: Width exceeds max number! Please check txt file or recompile your source code!\n");
			return Mat();
		}
		if (width==0 || height==0)
			return Mat();

		int i=0, j=0;
		char ch;  //to get rid of ","
		if(strcmp(format,"%d")==0){
			Mat M(height, width, CV_32S);

			for(i=0; i<height; i++)
			{
				int* p=M.ptr<int>(i);
				for (j=0;j<width;j++)
				{
					fscanf(fid, "%d,", p+j);
				}
			}

			return M;
		}else{
			Mat M(height, width, CV_32F);
			for(i=0; i<height; i++)
			{
				float* p=M.ptr<float>(i);
				for (j=0;j<width;j++)
				{
					fscanf(fid, "%f,", p+j);
				}
			}
			return M;
		}

	}

	Mat matread(const char* filename, const char* format){
		FILE* fid = fopen(filename,"r");

		Mat M = matread(fid, format);
		
		fclose(fid);
		return M;
	}

	vector<Mat> matreadVec(const char* filename, const char* format){
		vector<Mat> mvec;
		FILE* fid = fopen(filename,"r");

		while(!feof(fid)){
			Mat M = matread(fid, format);
			if(M.empty()) break;
			char aline[260];
			fgets(aline, 260, fid);  //clear current line

			mvec.push_back(M);			
		}

		fclose(fid);
		return mvec;
	}

	Mat blkdiag(vector<Mat> mvec){
		int height=0, width=0;
		for (size_t i=0; i<mvec.size(); i++)
		{
			height += mvec[i].rows;
			width += mvec[i].cols;
		}

		Mat M = Mat::zeros(height, width, mvec[0].type());

		int h=0, w=0;
		for (size_t i=0; i<mvec.size(); i++)
		{
			mvec[i].copyTo(M(Range(h, h+mvec[i].rows), Range(w, w+mvec[i].cols)));
			h += mvec[i].rows;
			w += mvec[i].cols;
		}

		return M;
	}

	int sub2ind(const int cols, const int row, const int col){
		return row*cols+col;
	}
	void ind2sub(const int cols, const int ind, int& row, int& col){
		row = ind/cols;
		col = ind%cols;
	}
	void ind2subColMajor(const int rows, const int ind, int& row, int& col){
		col = ind/rows;
		row = ind%rows;
	}

	int sub2indMatlab(const int cols, const int row, const int col){
		return (row-1)*cols+col;
	}
	void ind2subMatlab(const int cols, const int ind, int& row, int& col){
		col = (ind-1)%cols+1;
		row = (ind-col)/cols+1;		
	}

	// template T is for index
	template<class T> Mat selectCols(const Mat src, Mat index){
		index = index.reshape(1, index.total());  // change to column vector
		Mat M(src.rows, index.rows, src.type());
		for (int i=0; i<index.rows; i++)
		{
			src.col((int)index.at<T>(i,0)).copyTo(M.col(i));
		}

		return M;
	}
	template<class T> Mat selectRows(const Mat src, Mat index){
		index = index.reshape(1, index.total());  // change to column vector
		Mat M(index.rows, src.size().width, src.type());
		for (int i=0; i<index.rows; i++)
		{
			src.row((int)index.at<T>(i,0)).copyTo(M.row(i));
		}

		return M;
	}

	template<class T> vector<int> find(const Mat src){
		vector<int> index(src.total());
		Mat m = src.reshape(1,src.total());
		int n = 0;
		for (int i=0; i<src.total(); i++)
		{
			if(m.at<T>(i))
				index[n++] = i;			
		}

		index.resize(n);
		return index;
	}
}
