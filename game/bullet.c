#include "bullet.h"
#include "barrier.h"

#include <stdlib.h>
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


// Initiates a bullet explosion
void bullet_explode(Bullet *self) {
    if (!self->explosion_ticks) {
        self->explosion_ticks = BULLET_EXPLOSION_TICKS;
    }
}
