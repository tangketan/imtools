/*
	
	Content : Header file for MS bitmap format
  */

# pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BMP_BI_RGB      0L

typedef unsigned short	BMP_WORD; 
typedef unsigned int	BMP_DWORD; 
typedef int				BMP_LONG; 

#pragma pack(1)
// file header has to strictly 14 bytes,
// otherwise windows photo viewer won't recognize RGB image
typedef struct __pack__ { 
	BMP_WORD	bfType; 
	BMP_DWORD	bfSize; 
	BMP_WORD	bfReserved1; 
	BMP_WORD	bfReserved2; 
	BMP_DWORD	bfOffBits; 
} BMP_BITMAPFILEHEADER; 

// info header has to be strictly 40 bytes 
typedef struct { 
	BMP_DWORD	biSize; 
	BMP_LONG	biWidth; 
	BMP_LONG	biHeight; 
	BMP_WORD	biPlanes; 
	BMP_WORD	biBitCount; 
	BMP_DWORD	biCompression; 
	BMP_DWORD	biSizeImage; 
	BMP_LONG	biXPelsPerMeter; 
	BMP_LONG	biYPelsPerMeter; 
	BMP_DWORD	biClrUsed; 
	BMP_DWORD	biClrImportant; 
} BMP_BITMAPINFOHEADER; 
#pragma pack()

#ifndef MIN
#define MIN(x,y) ((x)<(y)? (x):(y))
#endif // !MIN

//extern unsigned char* readBMP( char* fname, int& width, int& height );
//extern void writeBMP( char* iname, int width, int height, unsigned char* data );
//
//extern unsigned char* readBMPGray( char* fname, int& width, int& height);
//extern void writeBMPGray( char* iname, int width, int height, unsigned char* data);
//
//extern unsigned char* readBMPGray2( const char* fname, int& width, int& height, int rowIndex);
//extern unsigned char* readBMPGray3( const char* fname, FILE ** file, int &pos, int& width, int& height, int rowIndex);
//
//FILE * genBMPHeader(char*				iname,
//			  int				width, 
//			  int				height);
//
//extern unsigned char * createPalette(int& nPaletteBits);

// tkt: 2d array manipulation
template <class T> T** malloc_2d(int height, int width)
{
	T ** img=NULL, *tmp=NULL;
	img = (T **)malloc(height* sizeof(T *));
	tmp = (T *)malloc(height*width* sizeof(T));
	for (int i=0; i<height; i++, tmp+=width)
		img[i] = tmp;
	return img;
}

template <class T1, class T2> void memcpy_2d(T1 ** Dst, T2 ** Src, int height, int width)
{
	for (int i=0; i<height; i++)
		for (int j=0; j<width; j++)
			Dst[i][j] = (T1)Src[i][j];
}
template <class T1> void memset_2d(T1 ** Dst, int val, int height, int width)
{
	for (int i=0; i<height; i++)
		for (int j=0; j<width; j++)
			Dst[i][j] = (T1)val;
}



unsigned char * createPalette(int& nPaletteBits)
{
	int i, i1;
	nPaletteBits =1024;  // size of a grayscale image palette.
	unsigned char * pPalette = (unsigned char *) malloc(nPaletteBits*sizeof(unsigned char));
	for (i=0; i<256; i++)
	{
		i1 = i*4;
		pPalette[i1] = pPalette[i1+1] = pPalette[i1+2] = i;
		pPalette[i1+3] = 0;
	}	

	return pPalette;
}
 
BMP_BITMAPFILEHEADER bmfh; 
BMP_BITMAPINFOHEADER bmih; 

unsigned char* readBMP(char*	fname, 
					   int&		width,
					   int&		height)
{ 
        FILE* file=fopen(fname,"rb");
	BMP_DWORD pos; 
 
	if ( file == NULL )  
	{
		printf("File cannot be opened");
		return NULL; 
	}
	 

	fread( &(bmfh.bfType), sizeof(unsigned short), 1, file); 
	fread( &(bmfh.bfSize), sizeof(unsigned int), 1, file); 
	fread( &(bmfh.bfReserved1), sizeof(unsigned short), 1, file); 
	fread( &(bmfh.bfReserved2), sizeof(unsigned short), 1, file); 
	fread( &(bmfh.bfOffBits), sizeof(unsigned int), 1, file); 

	pos = bmfh.bfOffBits; 
 
	fread( &bmih, sizeof(BMP_BITMAPINFOHEADER), 1, file ); 


	// error checking
	if ( bmfh.bfType!= 0x4d42 ) {	// "BM" actually
		printf("Type Error!\n");
		return NULL;
	}
	if ( bmih.biBitCount != 24 )
	{
		printf("bmih.biBitCount = %d\n", bmih.biBitCount );  
		return NULL; 
	}

	fseek( file, pos, SEEK_SET ); 
 
	width = bmih.biWidth; 
	height = bmih.biHeight; 
 
	int padWidth = width * 3; 
	int pad = 0; 
	if ( padWidth % 4 != 0 ) 
	{ 
		pad = 4 - (padWidth % 4); 
		padWidth += pad; 
	} 
	int bytes = height*padWidth; 
 
	unsigned char *data = new unsigned char [bytes]; 

	int result = fread( data, bytes, 1, file ); 
	
	if (!result) {
		delete [] data;
		return NULL;
	}

	fclose( file );
	
	// shuffle bitmap data such that it is (R,G,B) tuples in row-major order
	int i, j;
	j = 0;
	unsigned char temp;
	unsigned char* in;
	unsigned char* out;

	in = data;
	out = data;

	for ( j = 0; j < height; ++j )
	{
		for ( i = 0; i < width; ++i )
		{
			out[1] = in[1];
			temp = in[2];
			out[2] = in[0];
			out[0] = temp;

			in += 3;
			out += 3;
		}
		in += pad;
	}

	return data; 
}

typedef enum{
	CV_GRAY = 0,
	CV_RGB = 1,
	CV_BGR = 2,
}e_color_codes;
 
int writeBMP(char*				iname,
			  int				width, 
			  int				height, 
              int               channels,
			  unsigned char*	data,
			  e_color_codes 	color_code)
{ 
    if(channels!=3 && channels!=1){
        printf("Input channels should be 3 or 1!\n");
        return 0;
    }
    
	int bytes, pad;
	int nPaletteBits = 0;
	bytes = width * channels;
	pad = (bytes%4) ? 4-(bytes%4) : 0;
	bytes += pad;
	bytes *= height;

	// for grayscale image, must save the palette
	unsigned char * pPalette = NULL;
	if (channels==1)
		pPalette = createPalette(nPaletteBits);

	bmfh.bfType = 0x4d42;    // "BM"
	bmfh.bfSize = sizeof(BMP_BITMAPFILEHEADER)+sizeof(BMP_BITMAPINFOHEADER) + nPaletteBits + bytes;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BMP_BITMAPFILEHEADER)+sizeof(BMP_BITMAPINFOHEADER)+nPaletteBits;


	bmih.biSize = sizeof(BMP_BITMAPINFOHEADER);
	bmih.biWidth = width;
	bmih.biHeight = height;
	bmih.biPlanes = 1;
	bmih.biBitCount = 8*channels;
	bmih.biCompression = BMP_BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = (int)(100 / 2.54 * 72);
	bmih.biYPelsPerMeter = (int)(100 / 2.54 * 72);
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

    FILE *outFile=fopen(iname,"wb");
	
	fwrite( &(bmfh.bfType), 2, 1, outFile); 
	fwrite( &(bmfh.bfSize), 4, 1, outFile); 
	fwrite( &(bmfh.bfReserved1), 2, 1, outFile); 
	fwrite( &(bmfh.bfReserved2), 2, 1, outFile); 
	fwrite( &(bmfh.bfOffBits), 4, 1, outFile); 

	fwrite(&bmih, sizeof(BMP_BITMAPINFOHEADER), 1, outFile); 

	if (channels == 3)
	{
		bytes /= height;
		unsigned char* scanline = new unsigned char[bytes];
		for (int j = height-1 ; j >= 0; --j)
		{
			if (color_code == CV_BGR){
				// convert from BGR to RGB
				memcpy(scanline, data + j * 3 * width, bytes);
				for (int i = 0; i < width; ++i)
				{
					unsigned char temp = scanline[i * 3];
					scanline[i * 3] = scanline[i * 3 + 2];
					scanline[i * 3 + 2] = temp;
				}
			}
			else if (color_code == CV_RGB){
				memcpy(scanline, data + j * 3 * width, 3 * width);
			}
			fwrite(scanline, bytes, 1, outFile);
		}
		delete[] scanline;
	}
	else
	{
		fwrite(pPalette, nPaletteBits, 1, outFile);
		fseek(outFile, bmfh.bfOffBits, SEEK_SET);

		bytes /= height;
		for (int i = height - 1; i >= 0; i--)   //should from bottom row to top: http://en.wikipedia.org/wiki/BMP_file_format
			fwrite(data + i*width, bytes, 1, outFile);
	}

	fclose(outFile);
    
    return 1;
} 


unsigned char* readBMPGray(char*	fname, 
					   int&		width,
					   int&		height)					  
{ 
        FILE* file=fopen(fname,"rb");
	BMP_DWORD pos; 
 
	if ( file == NULL )  
	{
		printf("File cannot be opened");
		return NULL; 
	}
	 

	fread( &(bmfh.bfType), sizeof(unsigned short), 1, file); 
	fread( &(bmfh.bfSize), sizeof(unsigned int), 1, file); 
	fread( &(bmfh.bfReserved1), sizeof(unsigned short), 1, file); 
	fread( &(bmfh.bfReserved2), sizeof(unsigned short), 1, file); 
	fread( &(bmfh.bfOffBits), sizeof(unsigned int), 1, file); 

	pos = bmfh.bfOffBits; 
	
 
	fread( &bmih, sizeof(BMP_BITMAPINFOHEADER), 1, file ); 

	//int nPaletteBits = bmfh.bfOffBits - (sizeof(bmfh)+sizeof(bmih));
	//unsigned char *pPalette = (unsigned char *)malloc(nPaletteBits* sizeof(unsigned char));
	//fread(pPalette, nPaletteBits, 1, file);

	// error checking
	if ( bmfh.bfType!= 0x4d42 ) {	// "BM" actually
		printf("Type Error!\n");
		return NULL;
	}
	if ( bmih.biBitCount != 8 )  //tkt: 8 bit/pixel for grayscale image
	{
		printf("bmih.biBitCount = %d\n", bmih.biBitCount );  
		return NULL; 
	}

	fseek( file, pos, SEEK_SET ); 	
 
	width = bmih.biWidth; 
	height = bmih.biHeight; 
 
	int padWidth = width; 
	int pad = 0; 
	if ( padWidth % 4 != 0 ) 
	{ 
		pad = 4 - (padWidth % 4); 
		padWidth += pad; 
	} 
	int bytes = height*padWidth; 
 
	unsigned char *data = new unsigned char [bytes]; 

	/*int result = fread( data, bytes, 1, file ); 	
	if (!result) {
		delete [] data;
		return NULL;
	}*/
	for (int i=height-1; i>=0; i--)   //tkt: don't know why, should be it reversely
		fread( data+i*width, width, 1, file );	

	fclose( file );	

	return data; 
} 

// read a specific row of BMP grayscale image
unsigned char* readBMPGray2(const char*	fname, 
					   int&		width,
					   int&		height, int rowIndex)					  
{ 
        FILE* file=fopen(fname,"rb");
	BMP_DWORD pos; 

	if ( file == NULL )  
	{
		printf("File cannot be opened!\n");
		return NULL; 
	}
	 

	fread( &(bmfh.bfType), sizeof(unsigned short), 1, file); 
	fread( &(bmfh.bfSize), sizeof(unsigned int), 1, file); 
	fread( &(bmfh.bfReserved1), sizeof(unsigned short), 1, file); 
	fread( &(bmfh.bfReserved2), sizeof(unsigned short), 1, file); 
	fread( &(bmfh.bfOffBits), sizeof(unsigned int), 1, file); 

	pos = bmfh.bfOffBits; 
	
	fread( &bmih, sizeof(BMP_BITMAPINFOHEADER), 1, file ); 

	// error checking
	if ( bmfh.bfType!= 0x4d42 ) {	// "BM" actually
		printf("Type Error!\n");
		return NULL;
	}
	if ( bmih.biBitCount != 8 )  //tkt: 8 bit/pixel for grayscale image
	{
		printf("bmih.biBitCount = %d\n", bmih.biBitCount );  
		return NULL; 
	}

	fseek( file, pos, SEEK_SET ); 	
 
	width = bmih.biWidth; 
	height = bmih.biHeight; 
 
	int padWidth = width; 
	int pad = 0; 
	if ( padWidth % 4 != 0 ) 
	{ 
		pad = 4 - (padWidth % 4); 
		padWidth += pad; 
	} 
 
	unsigned char *data = new unsigned char [width]; 

	fseek(file, rowIndex* padWidth, SEEK_CUR);
	fread( data, width, 1, file );	

	fclose( file );	

	return data; 
} 

// read a specific row of BMP grayscale image. If file is already
// opened, then don't re-open it.
unsigned char* readBMPGray3(const char*	fname, FILE ** aFile, int& pos,
					   int&		width,
					   int&		height, int rowIndex)					  
{ 
	FILE * file = *aFile;
	if (file==NULL)
	{
                file=fopen(fname,"rb");
		if ( file == NULL )  
		{
			printf("File cannot be opened!\n");
			return NULL; 
		}


		fread( &(bmfh.bfType), sizeof(unsigned short), 1, file); 
		fread( &(bmfh.bfSize), sizeof(unsigned int), 1, file); 
		fread( &(bmfh.bfReserved1), sizeof(unsigned short), 1, file); 
		fread( &(bmfh.bfReserved2), sizeof(unsigned short), 1, file); 
		fread( &(bmfh.bfOffBits), sizeof(unsigned int), 1, file); 

		pos = bmfh.bfOffBits; 

		fread( &bmih, sizeof(BMP_BITMAPINFOHEADER), 1, file ); 
		// error checking
		if ( bmfh.bfType!= 0x4d42 ) {	// "BM" actually
			printf("Type Error!\n");
			return NULL;
		}
		if ( bmih.biBitCount != 8 )  //tkt: 8 bit/pixel for grayscale image
		{
			printf("bmih.biBitCount = %d\n", bmih.biBitCount );  
			return NULL; 
		}
	}

	fseek( file, pos, SEEK_SET ); 	

	int padWidth = width; 
	int pad = 0; 
	if ( padWidth % 4 != 0 ) 
	{ 
		pad = 4 - (padWidth % 4); 
		padWidth += pad; 
	} 

 
	unsigned char *data = new unsigned char [width]; 

	fseek(file, rowIndex* padWidth, SEEK_CUR);
	fread( data, width, 1, file );	

	return data; 
}


FILE * genBMPHeader(char*				iname,
			  int				width, 
			  int				height)
{ 
	int bytes, pad, nPaletteBits;
	bytes = width;
	pad = (bytes%4) ? 4-(bytes%4) : 0;
	bytes += pad;
	bytes *= height;

	unsigned char * pPalette;
	pPalette = createPalette(nPaletteBits);

	bmfh.bfType = 0x4d42;    // "BM"
	bmfh.bfSize = sizeof(BMP_BITMAPFILEHEADER) + sizeof(BMP_BITMAPINFOHEADER) + nPaletteBits + bytes;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BMP_BITMAPFILEHEADER) + sizeof(BMP_BITMAPINFOHEADER) + nPaletteBits;

	bmih.biSize = sizeof(BMP_BITMAPINFOHEADER);
	bmih.biWidth = width;
	bmih.biHeight = height;
	bmih.biPlanes = 1;
	bmih.biBitCount = 8;  //tkt: 8 bit/pixel
	bmih.biCompression = BMP_BI_RGB;  //no compression
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = (int)(100 / 2.54 * 72);
	bmih.biYPelsPerMeter = (int)(100 / 2.54 * 72);
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

        FILE *outFile=fopen(iname,"wb");
	
	fwrite( &(bmfh.bfType), 2, 1, outFile); 
	fwrite( &(bmfh.bfSize), 4, 1, outFile); 
	fwrite( &(bmfh.bfReserved1), 2, 1, outFile); 
	fwrite( &(bmfh.bfReserved2), 2, 1, outFile); 
	fwrite( &(bmfh.bfOffBits), 4, 1, outFile); 	

	fwrite(&bmih, sizeof(BMP_BITMAPINFOHEADER), 1, outFile); 


	fwrite(pPalette, nPaletteBits, 1, outFile);
	free(pPalette);
	
	fseek(outFile, bmfh.bfOffBits, SEEK_SET);

	return outFile;	
} 
