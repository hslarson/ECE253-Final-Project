#include "sprite.h"
#include "constants.h"

#include <stdint.h>
#include <stdbool.h>


// const uint16_t test_mat[6][6] = {
//     {0xF00F, 0xF00F, 0xF00F, 0x0000, 0x0F0F, 0x0F0F},
//     {0xF00F, 0xF00F, 0xF00F, 0x0000, 0x0F0F, 0x0F0F},
//     {0xF00F, 0xF00F, 0xF00F, 0x0000, 0x0F0F, 0x0F0F},
//     {0xF00F, 0xF00F, 0xF00F, 0x0000, 0x0F0F, 0x0F0F},
//     {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
//     {0x00FF, 0x00FF, 0x00FF, 0x0000, 0x0FFF, 0x0FFF}
// };

// const Sprite test_sprite = {
//     .extent_l_x = -3, 
//     .extent_r_x = 2,
//     .extent_t_y = -4,
//     .extent_b_y = 1,

//     .mat = (const uint16_t *)test_mat
// };


void sprite_blit(const Sprite *self, uint16_t screen[SCREEN_HEIGHT][SCREEN_WIDTH], int loc_x, int loc_y, bool invert_x, bool invert_y) {

    // Compute extents after inversion
    int ext_l_x = invert_x ? -self->extent_r_x : self->extent_l_x;
    int ext_t_y = invert_y ? -self->extent_b_y : self->extent_t_y;
    int ext_r_x = invert_x ? -self->extent_l_x : self->extent_r_x;
    int ext_b_y = invert_y ? -self->extent_t_y : self->extent_b_y;

    // Get screen region
    int l_x = max(MAP_MIN_X,   ext_l_x+loc_x);
    int t_y = max(MAP_MIN_Y,   ext_t_y+loc_y);
    int r_x = min(MAP_MAX_X-1, ext_r_x+loc_x);
    int b_y = min(MAP_MAX_Y-1, ext_b_y+loc_y);

    // Pixel matrix indices
    int col_start, mat_c;
    const int width = -self->extent_l_x + self->extent_r_x + 1;
    int mat_r;

    // Iterate rows
    for (int r=t_y; r<=b_y; r++) {
        // Compute matrix row index
        mat_r = (invert_y ?  loc_y-r : r-loc_y) - self->extent_t_y;
        col_start = mat_r*width;

        for (int c=l_x; c<=r_x; c++) {
            // Compute matrix column index
            mat_c = (invert_x ? loc_x-c : c-loc_x) - self->extent_l_x;

            // Treat alpha channel as boolean
            screen[r][c] = (self->mat[col_start+mat_c] & 0xF) ? self->mat[col_start+mat_c] : screen[r][c];
        }
    }
}


// Get the bounding box for a given sprite at a specific location
// Does not consider map bounds
void sprite_bbox(const Sprite *self, int loc_x, int loc_y, bool inv_x, bool inv_y, int *l_x, int *t_y, int *r_x, int *b_y) {

    *l_x = loc_x + (inv_x ? -self->extent_r_x : self->extent_l_x);
    *r_x = loc_x + (inv_x ? -self->extent_l_x : self->extent_r_x);

    *t_y = loc_y + (inv_y ? -self->extent_b_y : self->extent_t_y);
    *b_y = loc_y + (inv_y ? -self->extent_t_y : self->extent_b_y);
}
