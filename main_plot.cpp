// Matlab style plot functions for OpenCV
//author libing64
//https://github.com/libing64/CPlot

#include "plot.hpp"
#include "pgm.h"
#include "FileUtility.h"
#include "bmp.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc > 1){
#if 0
        tkt::RemoveFileInFolder(argv[1]);
#endif
#if 0
        if (tkt::isdir(argv[1])) cout << "is dir\n";
        else cout << "is file\n";
#endif

#if 1
        vector<string> v = tkt::ListDir(argv[1]);
        for (size_t i = 0; i < v.size(); i++)
        {
            cout << v[i] << endl;
        }

#endif

        return 0;
    }


	const int Cnt = 80;
	double X[Cnt] = {0};
	double Y[Cnt] = {0};
	for(int i = 0; i < Cnt; i++)
	{
		X[i] = (double)i;
		Y[i] = (double)4*i - 20;
	}
	
	cvNamedWindow("hello",1);
	Plot plot;
	CvScalar color = CV_RGB(255, 0, 0);
	plot.plot(X, Y, Cnt, color, 's');
	cvShowImage("hello", plot.Figure);
	cvWaitKey(0);
	//plot.clear();
	
	
	//如何在一幅图中绘制多组数据？每次绘制的同时还对数据进行存储？
	for(int i = 0; i < Cnt; i++)
	{
		X[i] = (double)i;
		Y[i] = (double)(5*i - 20);
	}
	plot.plot(X, Y, Cnt, color, '.');
	cvShowImage("hello", plot.Figure);
	cvWaitKey(0);
	
	for(int i = 0; i < Cnt; i++)
	{
		X[i] = (double)i;
		Y[i] = (double)(-5*i - 20);
	}
	plot.plot(X, Y, Cnt, color, '+');
	cvShowImage("hello", plot.Figure);
	cvWaitKey(0);
	
	
	for(int i = 0; i < Cnt; i++)
	{
		X[i] = (double)i;
		Y[i] = (double)(50*sin(i*0.1));
	}
	plot.plot(X, Y, Cnt, color, 'x');
	cvShowImage("hello", plot.Figure);
	cvWaitKey(0);
	
	for(int i = 0; i < Cnt; i++)
	{
		X[i] = (double)i;
		Y[i] = (double)(100*sin(i*0.1));
	}
	plot.plot(X, Y, Cnt, color, 'o');
	cvShowImage("hello", plot.Figure);
	cvWaitKey(0);
	
	int X2[Cnt] = {0};
	int Y2[Cnt] = {0};
	for(int i = 0; i < Cnt; i++)
	{
		X2[i] = i;
		Y2[i] = -30*i - 20;
	}
	plot.plot(X2, Y2, Cnt/2, color);
	cvShowImage("hello", plot.Figure);
	cvWaitKey(0);
	//cvSaveImage("example.jpg", plot.Figure);

	// write and read 16 bit pgm
	Mat im(plot.Figure), im16;
	cvtColor(im, im, CV_BGR2GRAY);
	im.convertTo(im16, CV_16UC1, 255);
	writePGM<unsigned short>("plot.pgm", im16, 0);

	Mat im1, im2 = readPGM<ushort>("plot.pgm", 0);
	im2.convertTo(im1, CV_8UC1, 1 / 255.0);
	imshow("im1", im1);

    // write color and grayscale bmp
	Mat imcolor(plot.Figure);
	writeBMP("bmp_color.bmp", imcolor.cols, imcolor.rows, imcolor.channels(), imcolor.data, CV_RGB);
	writeBMP("bmp_gray.bmp", im.cols, im.rows, im.channels(), im.data, CV_GRAY);

	waitKey();
	return 0;
}

