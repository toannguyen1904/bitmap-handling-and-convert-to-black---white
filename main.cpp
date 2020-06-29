/*driver code*/

#include"Bitmap.h"
int main() {

	const char* filePath = "D:\iu.bmp";

	FILE* f = fopen(filePath, "rb");
	BmpHeader header;

	BmpDIB dib;
	PixelArray data;

	readBmpHeader(f, header);

	readBmpDIB(f, dib);
	readBmpPixelArray(f, header, dib, data);

	convertToBlackWhiteBmp(dib, data);

	writeBmpFile(f, header, dib, data);

	const char* fileLink = "D:\iu1.bmp";
	FILE* ff = fopen(fileLink, "wb");
	writeBmpFile(ff, header, dib, data);

	releaseBmpPixelArray(data);
	return 0;

}