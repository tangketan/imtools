# pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "opencv2/opencv.hpp"

bool writePGM(const char* fname, const unsigned char* data, int width, int height, bool is_plainPGM=true);

unsigned char* readPGM(const char* fname, int& width, int& height, bool is_plainPGM=true);

#ifdef CV_VERSION
template<typename T>
bool writePGM(const char* fname, cv::Mat im, bool is_plainPGM);

template<typename T>
cv::Mat readPGM(const char* fname, bool is_plainPGM);
#endif