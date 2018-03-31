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

BMP_BITMAPFILEHEADER bmfh;
BMP_BITMAPINFOHEADER bmih;

typedef enum{
	CV_GRAY = 0,
	CV_RGB = 1,
	CV_BGR = 2,
}e_color_codes;

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

unsigned char* readBMP(char*	fname, 
					   int&		width,
					   int&		height,
					   int&		channels)
{ 
    FILE* file=fopen(fname,"rb");
	BMP_DWORD pos; 
 
	if ( file == NULL )  
	{
		printf("File cannot be opened");
		return NULL; 
	}

	fread(&bmfh, sizeof(BMP_BITMAPFILEHEADER), 1, file);
	fread(&bmih, sizeof(BMP_BITMAPINFOHEADER), 1, file);

 
	// error checking
	if ( bmfh.bfType!= 0x4d42 ) {	// "BM" actually
		printf("Type Error!\n");
		return NULL;
	}
	if ( bmih.biBitCount != 24 && bmih.biBitCount!=8 )
	{
		printf("bmih.biBitCount = %d\n", bmih.biBitCount );  
		return NULL; 
	}

	pos = bmfh.bfOffBits;
	fseek(file, pos, SEEK_SET);
 
	width = bmih.biWidth; 
	height = bmih.biHeight; 
	channels = bmih.biBitCount / 8;

	int padWidth = width * channels; 
	int step = padWidth;
	int pad = 0; 
	if ( padWidth % 4 != 0 ) 
	{ 
		pad = 4 - (padWidth % 4); 
		padWidth += pad; 
	} 
	int bytes = height*padWidth; 
 
	unsigned char *datain = (unsigned char*)malloc(bytes);

	int result = fread( datain, bytes, 1, file ); 
	
	if (!result) {
		free(datain);
		return NULL;
	}

	fclose( file );
	
	unsigned char *data = (unsigned char*)malloc(bytes);
	unsigned char* in = datain;
	unsigned char* out = data + (height - 1)*step;

	for (int i = height - 1; i >= 0; i--)   //BMP stores image from bottom to top, so we should read it reversely
	{
		memcpy(out, in, padWidth);
		in += padWidth;
		out -= step;
	}

	free(datain);

	return data; 
}

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
#if WRITE_FILE_MULTIPLE_TIMES
    FILE *outFile=fopen(iname,"wb");
	
	fwrite(&bmfh, sizeof(BMP_BITMAPFILEHEADER), 1, outFile);
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
#else
    unsigned char* buf = (unsigned char*)malloc(bmfh.bfSize);
    unsigned char* p_buf = buf;
    memcpy(p_buf, &bmfh, sizeof(bmfh));
    p_buf += sizeof(bmfh);
    memcpy(p_buf, &bmih, sizeof(bmih));
    p_buf += sizeof(bmih);

    if (channels == 3)
    {
        bytes /= height;
        int width3 = width * 3;
        unsigned char* p_data = data + (height-1) * width3;
        for (int j = height - 1; j >= 0; --j, p_data-=width3,p_buf+=bytes)
        {
            if (color_code == CV_BGR){
                // convert from BGR to RGB
                for (int i = 0; i < width; ++i)
                {
                    p_buf[i * 3 + 2] = p_data[i*3+0];
                    p_buf[i * 3 + 1] = p_data[i*3+1];
                    p_buf[i * 3 + 0] = p_data[i*3+2];
                }
            }
            else if (color_code == CV_RGB){
                memcpy(p_buf, data + j * 3 * width, 3 * width);
            }
        }
    }
    else
    {
        memcpy(p_buf, pPalette, nPaletteBits);
        p_buf += bmfh.bfOffBits - (p_buf - buf);

        bytes /= height;
        for (int i = height - 1; i >= 0; i--)   //should from bottom row to top: http://en.wikipedia.org/wiki/BMP_file_format
        {
            memcpy(p_buf, data + i*width, bytes);
            p_buf += bytes;
        }
    }

    FILE *outFile = fopen(iname, "wb");
    fwrite(buf, bmfh.bfSize, 1, outFile);
    fclose(outFile);
    free(buf);
#endif
    return 1;
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
		file = fopen(fname, "rb");
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
