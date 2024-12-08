#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void saveBMP(const char *filename, uint8_t *rgbData, int width, int height) {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    FILE *file;

    int rowSize = (width * 3 + 3) & ~3; // Row size (padded to 4 bytes)
    int dataSize = rowSize * height;

    fileHeader.bfType = 0x4D42; // 'BM'
    fileHeader.bfSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + dataSize;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    infoHeader.biSize = sizeof(BMPInfoHeader);
    infoHeader.biWidth = width;
    infoHeader.biHeight = -height; // Negative to indicate top-down bitmap
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24; // 24-bit RGB
    infoHeader.biCompression = 0; // No compression
    infoHeader.biSizeImage = dataSize;
    infoHeader.biXPelsPerMeter = 0;
    infoHeader.biYPelsPerMeter = 0;
    infoHeader.biClrUsed = 0;
    infoHeader.biClrImportant = 0;

    file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    fwrite(&fileHeader, sizeof(BMPFileHeader), 1, file);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, file);

    // Write pixel data with padding
    uint8_t padding[3] = {0, 0, 0};
    for (int y = 0; y < height; y++) {
        fwrite(rgbData + (y * width * 3), 3, width, file);
        fwrite(padding, 1, rowSize - (width * 3), file);
    }

    fclose(file);
}
