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
        fprintf(fid, "P5\n%d %d\n255\n", width, height);
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
        fscanf(fid, "P5\n%d %d\n255\n", &width, &height);
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
        fprintf(fid, "P5\n%d %d\n%d\n", width, height, sizeof(T)==1?255:65535);
        for (int i = 0; i < height; i++)
        for (int j = 0; j < width;j++)
            //fwrite(im.ptr<T>(i), sizeof(T), width, fid);
            fwrite(&im.at<T>(i,j), sizeof(T), 1, fid);
    }

    fclose(fid);
    return true;
}

template bool writePGM<uchar>(const char* fname, cv::Mat im, bool is_plainPGM);
template bool writePGM<unsigned short>(const char* fname, cv::Mat im, bool is_plainPGM);


cv::Mat readPGM(const std::string fname)
{
    FILE* fid;
    char buffer[100];
    cv::Mat im;
    int version = 0;
    int width = 0;
    int height = 0;
    int max_val = 0;
    int byte_per_pixel = 1;

    fid = fopen(fname.c_str(), "rb");
    if (!fid) return im;
    fscanf(fid, "P%d %d %d %d", &version, &width, &height, &max_val);

    if (max_val < 256) {
        byte_per_pixel = 1;
        im = cv::Mat(height, width, CV_8U);
    }
    else {
        byte_per_pixel = 2;
        im = cv::Mat(height, width, CV_16U);
    }

    if (version==2) {        
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                if (byte_per_pixel == 1)
                    fscanf(fid, "%hhu\n", &im.at<uchar>(i, j));
                else
                    fscanf(fid, "%hu\n", &im.at<ushort>(i, j));
            }
    }
    else {
        uchar* impointer = (uchar*)im.data;
        for (int i = 0; i < height; i++) {
            fread(impointer + i * width * byte_per_pixel, byte_per_pixel, width, fid);
        }
    }

    fclose(fid);
    return im;
}
#endif