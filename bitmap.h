#ifndef ECE253_FINAL_BITMAP
#define ECE253_FINAL_BITMAP

#include <stdint.h>

#pragma pack(push, 1) // Ensure structures are packed
typedef struct {
    uint16_t bfType;      // Bitmap file type ('BM')
    uint32_t bfSize;      // Size of the file in bytes
    uint16_t bfReserved1; // Reserved, must be zero
    uint16_t bfReserved2; // Reserved, must be zero
    uint32_t bfOffBits;   // Offset to the pixel data
} BMPFileHeader;

typedef struct {
    uint32_t biSize;          // Size of this header (40 bytes)
    int32_t  biWidth;         // Width of the bitmap in pixels
    int32_t  biHeight;        // Height of the bitmap in pixels
    uint16_t biPlanes;        // Number of color planes (must be 1)
    uint16_t biBitCount;      // Bits per pixel (24 for RGB)
    uint32_t biCompression;   // Compression type (0 for no compression)
    uint32_t biSizeImage;     // Size of the pixel data
    int32_t  biXPelsPerMeter; // Horizontal resolution (pixels per meter)
    int32_t  biYPelsPerMeter; // Vertical resolution (pixels per meter)
    uint32_t biClrUsed;       // Number of colors used (0 for all colors)
    uint32_t biClrImportant;  // Number of important colors (0 for all colors)
} BMPInfoHeader;
#pragma pack(pop)

void saveBMP(const char *filename, uint8_t *rgbData, int width, int height)

#endif
