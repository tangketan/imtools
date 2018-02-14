#include "bmp.hpp"

int main(int argc, char** argv)
{
	if(argc<3){
		printf("Usage: %s <image.bmp> <image.pgm> [is_plain=1]\n", argv[0]);
		return 0;
	}
	bool is_plain = true;
	
	if(argc>3){
		is_plain = atoi(argv[3]);
	}
	
	int height, width;
	unsigned char* im=readBMPGray(argv[1], width, height);
	
	writePGM(argv[2], im, width, height, is_plain);
	delete[] im;
	return 1;
}