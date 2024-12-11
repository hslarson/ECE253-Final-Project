#include "bullet.h"
#include "barrier.h"
#include "constants.h"

#include <stdlib.h>
#include <stdint.h>
#include <math.h>


// Initializer
void bullet_init(Bullet *self, float x, float y, float angle) {
    self->position_x = x;
    self->position_y = y;
    self->last_x = -1;
    self->last_y = -1;
    self->delta_x = BULLET_SPEED*sin(angle);
    self->delta_y = -BULLET_SPEED*cos(angle); // Invert y
    self->explosion_ticks = 0;
}


// Move the bullet long its path
void bullet_move(Bullet *self, int ticks) {
    // Bullets do not move when exploding
    if (self->explosion_ticks) { return; }

    // Update position
    self->position_x += ticks*self->delta_x;
    self->position_y += ticks*self->delta_y; 
}


// Check if a bullet has collided with a barrier
// Return 1 if collision is detected
int bullet_check_barrier(const Bullet *self, const Barrier *barrier) {
    // Bullets do not collide with anything when exploding
    int hit = !self->explosion_ticks; 

    // Check barrier limits
    // For simplicity, we only consider centroid, not full extents
    hit &= self->position_x >= barrier->l_x;
    hit &= self->position_x <= barrier->r_x;
    hit &= self->position_y >= barrier->t_y;
    hit &= self->position_y <= barrier->b_y;
    return hit;
}


// Initiate a bullet explosion
void bullet_explode(Bullet *self) {
    if (!self->explosion_ticks) {
        self->explosion_ticks = BULLET_EXPLOSION_TICKS;
    }
}


// Draw bullet's current location on screen
// Erase old location beforehand to prevent artifacts
void bullet_draw(Bullet *self, uint16_t screen[SCREEN_HEIGHT][SCREEN_WIDTH]) {
    
    // Rotate sprite
    // Blit sprite

    // TODO: replace w/ sprite
    int t_y = (int)round(self->position_y) - 4;
    int l_x = (int)round(self->position_x) - 4;
    int b_y = (int)round(self->position_y) + 4;
    int r_x = (int)round(self->position_x) + 4;

    int start_row = max(MAP_MIN_Y, t_y);
    int start_col = max(MAP_MIN_X, l_x);
    int stop_row  = min(MAP_MAX_Y, b_y+1);
    int stop_col  = min(MAP_MAX_X, r_x+1);
    
    // Iterate rows
    for (int r=start_row; r<stop_row; r++) {
        // Iterate cols
        for (int c=start_col; c<stop_col; c++) {
            screen[r][c] = self->explosion_ticks ? (uint16_t)0xF00F : (uint16_t)0x555F; // TODO: define
        }
    }

    // Update last drawn location
    self->last_x = self->position_x;
    self->last_y = self->position_y;
}


// Get the bounding box of the bullet
// as it was last drawn to the screen
void bullet_visual_bb(const Bullet *self, int *l_x, int *t_y, int *r_x, int *b_y) {
    
    // Compute bullet extents
    *l_x = (int)round(self->last_x) - BULLET_MAX_R;
    *t_y = (int)round(self->last_y) - BULLET_MAX_R;
    *r_x = (int)round(self->last_x) + BULLET_MAX_R;
    *b_y = (int)round(self->last_y) + BULLET_MAX_R;
}
