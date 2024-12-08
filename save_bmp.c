#include <stdlib.h>
#include <stdint.h>

#include "bitmap.h"

int main() {
    int width = 640;
    int height = 480;
    uint8_t *rgbData = (uint8_t *)malloc(width * height * 3);

    // Example: Fill with gradient
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            rgbData[(y * width + x) * 3 + 0] = (uint8_t)(x % 256); // Blue
            rgbData[(y * width + x) * 3 + 1] = (uint8_t)(y % 256); // Green
            rgbData[(y * width + x) * 3 + 2] = 0;                  // Red
        }
    }

    saveBMP("output.bmp", rgbData, width, height);

    free(rgbData);
    return 0;
}
