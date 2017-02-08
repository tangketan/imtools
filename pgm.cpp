#include "pgm.h"

bool writePGM(const char* fname, const unsigned char* data, int width, int height, bool is_plainPGM)
{
	FILE* fid;
	if (is_plainPGM){
		fid = fopen(fname, "w");
		if (!fid) return false;
		fprintf(fid, "P2\n%d %d\n255\n", width, height);
		for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			fprintf(fid, "%d\n", data[i*width + j]);
	}
	else {
		fid = fopen(fname, "wb");
		if (!fid) return false;
		fprintf(fid, "P2\n%d %d\n255\n", width, height);
		for (int i = 0; i < height; i++)
			fwrite(data + i*width, 1, width, fid);
	}

	fclose(fid);
	return true;
}

unsigned char* readPGM(const char* fname, int& width, int& height, bool is_plainPGM)
{
	FILE* fid;
	unsigned char* data;
	if (is_plainPGM){
		fid = fopen(fname, "r");
        if (!fid) return NULL;
		fscanf(fid, "P2\n%d %d\n255\n", &width, &height);
		data = new unsigned char[width*height];
		for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			fscanf(fid, "%hhu\n", &data[i*width + j]);
	}
	else {
		fid = fopen(fname, "rb");
        if (!fid) return NULL;
		fscanf(fid, "P2\n%d %d\n255\n", &width, &height);
		data = new unsigned char[width*height];
		for (int i = 0; i < height; i++)
			fread(data + i*width, 1, width, fid);
	}

	fclose(fid);
	return data;
}

#ifdef CV_VERSION
template<typename T>
bool writePGM(const char* fname, cv::Mat im, bool is_plainPGM)
{
	FILE* fid;
	int width = im.cols;
	int height = im.rows;
	if (is_plainPGM){
		fid = fopen(fname, "w");
		if (!fid) return false;
		fprintf(fid, "P2\n%d %d\n%d\n", width, height, sizeof(T)==1?255:65535);

		for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			fprintf(fid, "%d\n", im.at<T>(i,j));
	}
	else {
		fid = fopen(fname, "wb");
		if (!fid) return false;
		fprintf(fid, "P2\n%d %d\n%d\n", width, height, sizeof(T)==1?255:65535);
		for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			fwrite(&(im.at<T>(i,j)), 1, 1, fid);
	}

	fclose(fid);
	return true;
}

template bool writePGM<uchar>(const char* fname, cv::Mat im, bool is_plainPGM);
template bool writePGM<unsigned short>(const char* fname, cv::Mat im, bool is_plainPGM);

template<typename T>
cv::Mat readPGM(const char* fname, bool is_plainPGM)
{
	FILE* fid;
	cv::Mat im;
	int width = 0;
	int height = 0;
	if (is_plainPGM){
		fid = fopen(fname, "r");
        if (!fid) return im;
		fscanf(fid, "P2\n%d %d\n255\n", &width, &height);
		im = cv::Mat(height, width, cv::DataType<T>::type);
		for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++){
			if(sizeof(T)==1)
				fscanf(fid, "%hhu\n", &im.at<T>(i,j));
			else if (sizeof(T)==2)
				fscanf(fid, "%hu\n", &im.at<T>(i,j));
		}
	}
	else {
		fid = fopen(fname, "rb");
        if (!fid) return im;
		fscanf(fid, "P2\n%d %d\n255\n", &width, &height);
		im = cv::Mat(height, width, cv::DataType<T>::type);
		for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			fread(&(im.at<T>(i,j)), 1, 1, fid);
	}

	fclose(fid);
	return im;
}
template cv::Mat readPGM<uchar>(const char* fname, bool is_plainPGM);
template cv::Mat readPGM<unsigned short>(const char* fname, bool is_plainPGM);
#endif