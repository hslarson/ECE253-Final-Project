#ifndef ECE253_FINAL_SPRITE
#define ECE253_FINAL_SPRITE

#include "constants.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct Sprite {
    // Extents
    int extent_l_x;
    int extent_t_y;
    int extent_r_x;
    int extent_b_y;

    // Pointer to pixel matrix
    const uint16_t *mat;

} Sprite;

void sprite_blit(
    const Sprite *self, 
    uint16_t screen[SCREEN_HEIGHT][SCREEN_WIDTH], 
    int loc_x, int loc_y, 
    bool invert_x, bool invert_y,
    int limit_t_y
);

void sprite_bbox(
    const Sprite *self, 
    int loc_x, int loc_y, 
    bool inv_x, bool inv_y, 
    int *l_x, int *t_y, int *r_x, int *b_y
);

#endif