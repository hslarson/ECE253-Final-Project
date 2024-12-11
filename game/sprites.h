#ifndef ECE253_FINAL_SPRITE
#define ECE253_FINAL_SPRITE

#include <stdint.h>

typedef struct Sprite {
    // Extents
    int extent_l_x;
    int extent_t_y;
    int extent_r_x;
    int extent_b_y;

    // Pointer to matrix
    uint16_t **mat;

} Sprite;

void blit(void);




#endif