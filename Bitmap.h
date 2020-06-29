#pragma once
// bitmap

#include<Windows.h>
#include<iostream>
using namespace std;

/* Signature */
struct BmpSignature
{
	unsigned char data[2];
};

#pragma pack(1)
/* bitmap file header*/
struct BmpHeader
{
	BmpSignature signature;
	uint32_t fileSize;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t dataOffset;
};

/* Device independent bitmap */
struct BmpDIB
{
	uint32_t dibSize;
	uint32_t imageWidth;
	uint32_t imageHeight;
	uint16_t colorPlaneCount; //  I don't get it 
	uint16_t pixelSize;
	uint32_t compressMethod;
	uint32_t bitmapByteCount;
	int32_t horizontalResolution;
	int32_t verticalResolution;
	uint32_t colorCount;
	uint32_t importantColorCount;
};

/* RGB color*/
struct Color
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};

/* color table */
struct ColorTable
{
	Color* colors;
	uint32_t length;
};

/* pixel array */
struct PixelArray
{
	Color** pixels;
	uint32_t rowCount;
	uint32_t columnCount;
};

bool isBmpFile(FILE* f); // check if file is .bmp
void readBmpHeader(FILE* f, BmpHeader& header); // read Bitmap file header
void printBmpHeader(BmpHeader header); // print header
void readBmpDIB(FILE* f, BmpDIB& dib); // read DIB Informaiton
void printBmpDIB(BmpDIB dib); // print DIB Information
void scanBmpPixelLine(FILE* f, Color*& line, uint32_t length); // scan pixels line
void skipBmpPadding(FILE* f, char count); // skip padding
void readBmpPixelArray(FILE* f, BmpHeader header, BmpDIB dib, PixelArray& data); // read pixel array
void convertToBlackWhiteBmp(BmpDIB dib, PixelArray& data); // convert to back - white
void writeBmpFile(FILE* f, BmpHeader header, BmpDIB dib, PixelArray data); // write to file
void drawBmp(BmpDIB dib, PixelArray data); // draw image through the console
void releaseBmpPixelArray(PixelArray data); // release pixel array
