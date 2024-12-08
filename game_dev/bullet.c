#include "bullet.h"

#include <stdlib.h>
#include <math.h>

#include "game_constants.h"
#include "barrier.h"


// Initializer
void bullet_init(Bullet *self, float x, float y, float angle) {
    self->position_x = x;
    self->position_y = y;
    self->last_x = x;
    self->last_y = y;
    self->angle = angle;
}


// Move the bullet long its path
void bullet_move(Bullet *self) {
    // Store last position
    self->last_x = self->position_x;
    self->last_y = self->position_y;

    // Update position
    self->position_x += BULLET_SPEED*sin(self->angle);
    self->position_y -= BULLET_SPEED*cos(self->angle); // Invert y
}


// Check if a bullet has collided with a barrier
// Return 1 if collision is detected
int bullet_check_barrier(const Bullet *self, const Barrier *barrier) {
    int hit = 1;
    hit &= self->position_x >= barrier->l_x;
    hit &= self->position_x <= barrier->r_x;
    hit &= self->position_y >= barrier->t_y;
    hit &= self->position_y <= barrier->b_y;
    return hit;
}
