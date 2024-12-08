#ifndef ECE253_FINAL_BULLET
#define ECE253_FINAL_BULLET

#include "barrier.h"


typedef struct Bullet {
    // Current position
    float position_x;
    float position_y;

    // Last position
    float last_x;
    float last_y;

    // Orientation
    // 0    --> pointed towards screen top
    // pi/2 --> pointed towards screen right
    float angle;
} Bullet;


void bullet_init(Bullet *self, float x, float y, float angle);
void bullet_move(Bullet *self);
int  bullet_check_barrier(const Bullet *self, const Barrier *barrier);

#endif
