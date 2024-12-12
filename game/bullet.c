#include "bullet.h"
#include "barrier.h"
#include "constants.h"
#include "sprite.h"
#include "assets.h"

#include <stdlib.h>
#include <stdint.h>
#include <math.h>


// Initializer
void bullet_init(Bullet *self, float x, float y, float angle) {
    self->position_x = x;
    self->position_y = y;

    self->delta_x = BULLET_SPEED*sin(angle);
    self->delta_y = -BULLET_SPEED*cos(angle); // Invert y

    self->bbox_l_x = 0;
    self->bbox_t_y = 0;
    self->bbox_r_x = 0;
    self->bbox_b_y = 0;
    
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
    
    // Pick appropriate sprite based on explosion state
    int sprite_index = (int)(self->explosion_ticks != 0);
    const Sprite *bullet_sprite = bullet_sprites[sprite_index];

    // Compute centroid in screen coordinates
    int pos_x = (int)round(self->position_x);
    int pos_y = (int)round(self->position_y);

    // Blit sprite
    sprite_blit(bullet_sprite, screen, pos_x, pos_y, 0, 0);

    // Update last drawn bounds
    sprite_bbox(
        bullet_sprite, 
        pos_x, pos_y, 0, 0, 
        &(self->bbox_l_x),
        &(self->bbox_t_y),
        &(self->bbox_r_x),
        &(self->bbox_b_y)
    );
}
