#include"Bitmap.h"

bool isBmpFile(FILE* f)
{
	if (f == NULL)return false;

	BmpSignature signature;
	fseek(f, 0, SEEK_SET);
	fread(&signature, sizeof(BmpSignature), 1, f);

	return signature.data[0] == 'B' && signature.data[1] == 'M';
}

void readBmpHeader(FILE* f, BmpHeader& header)
{
	if (f == NULL)return;

	fseek(f, 0, SEEK_SET);
	fread(&header, sizeof(BmpHeader), 1, f);
}

void printBmpHeader(BmpHeader header)
{
	cout << "*** BMP HEADER ****" << endl;
	cout << "File Size: " << header.fileSize << endl;
	cout << "Reserved1: " << header.reserved1 << endl;
	cout << "Reserved2: " << header.reserved2 << endl;
	cout << "Data Offset: " << header.dataOffset << endl;
}

void readBmpDIB(FILE* f, BmpDIB& dib)
{
	if (f == NULL)
		return;

	fseek(f, sizeof(BmpHeader), SEEK_SET);
	fread(&dib, sizeof(BmpDIB), 1, f);
}

void printBmpDIB(BmpDIB dib)
{
	cout << "*** BMP DIB ***" << endl;
	cout << "DIB Size: " << dib.dibSize << endl;
	cout << "Image Width: " << dib.imageWidth << endl;
	cout << "Image Height: " << dib.imageHeight << endl;
	cout << "Number of Color Planes: " << dib.colorPlaneCount << endl;
	cout << "Pixel Size: " << dib.pixelSize << endl;
	cout << "Compress Method: " << dib.compressMethod << endl;
	cout << "Bitmap Size: " << dib.bitmapByteCount << endl;
	cout << "Horizontal Resolution: " << dib.horizontalResolution << endl;
	cout << "Vertical Resolution: " << dib.verticalResolution << endl;
	cout << "Number of Colors: " << dib.colorCount << endl;
	cout << "Number of Impt Colors: " << dib.importantColorCount << endl;
}

void scanBmpPixelLine(FILE* f, Color*& line, uint32_t length)
{
	if (f == NULL)
		return;

	line = new Color[length];
	fread(line, sizeof(Color), length, f);
}

void skipBmpPadding(FILE* f, char count)
{

	if (f == NULL)
		return;

	if (count == 0)
		return;

	char padding[3];
	fread(padding, count, 1, f);
}

void readBmpPixelArray(FILE* f, BmpHeader header, BmpDIB dib, PixelArray& data)
{
	if (f == NULL)
		return;

	data.rowCount = dib.imageHeight;
	data.columnCount = dib.imageWidth;
	data.pixels = new Color * [data.rowCount];

	char paddingCount = (4 - (dib.imageWidth * (dib.pixelSize / 8) % 4)) % 4;

	fseek(f, header.dataOffset, SEEK_SET);

	for (int i = 0; i < data.rowCount; i++)
	{
		scanBmpPixelLine(f, data.pixels[data.rowCount - 1 - i], dib.imageWidth);
		skipBmpPadding(f, paddingCount);
	}
}

void convertToBlackWhiteBmp(BmpDIB dib, PixelArray& data)
{
	for (int i = 0; i < dib.imageHeight; i++)
		for (int j = 0; j < dib.imageWidth; j++)
		{
			Color cl = { data.pixels[i][j].red,data.pixels[i][j].green,data.pixels[i][j].blue };

			data.pixels[i][j].red = data.pixels[i][j].green = data.pixels[i][j].blue =
				cl.red * 0.299 + cl.green * 0.587 + cl.blue * 0.114;
		}
}

void writeBmpFile(FILE* f, BmpHeader header, BmpDIB dib, PixelArray data)
{
	if (f == NULL)
	{
		cout << "error" << endl;
		return;
	}
	fseek(f, 0, SEEK_SET);
	fwrite(&header, sizeof(BmpHeader), 1, f);
	fseek(f, sizeof(BmpHeader), SEEK_SET);
	fwrite(&dib, sizeof(BmpDIB), 1, f);
	fseek(f, header.dataOffset, SEEK_SET);

	char paddingCount = (4 - (dib.imageWidth * (dib.pixelSize / 8) % 4)) % 4;
	char padding[3] = { 0,0,0 };

	for (int i = data.rowCount - 1; i >= 0; i--)
	{
		fwrite(data.pixels[i], 3, dib.imageWidth, f);
		fwrite(padding, paddingCount, 1, f);
	}
	fclose(f);
}

void drawBmp(BmpDIB dib, PixelArray data)
{
	HWND console = GetConsoleWindow();
	HDC hdc = GetDC(console);

	for (int i = 0; i < dib.imageHeight; i++)
		for (int j = 0; j < dib.imageWidth; j++)
		{
			Color pixel = data.pixels[i][j];
			SetPixel(hdc, j, i, RGB(pixel.red, pixel.green, pixel.blue));
		}

	ReleaseDC(console, hdc);
}

void releaseBmpPixelArray(PixelArray data)
{
	for (int i = 0; i < data.rowCount; i++)
		delete[]data.pixels[i];
	delete[]data.pixels;
}
