#include "bmp.hpp"

int main(int argc, char** argv)
{
	if(argc<3){
		printf("Usage: %s <image.bmp> <image.pgm>\n", argv[0]);
		return 0;
	}
	
	int height, width;
	unsigned char* im=readBMPGray(argv[1], width, height);
	
	writePGM(argv[2], im, width, height);
	delete[] im;
	return 1;
}