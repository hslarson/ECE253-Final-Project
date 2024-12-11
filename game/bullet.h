// bullet.h
// Define a structure for tank projectiles

#ifndef ECE253_FINAL_BULLET
#define ECE253_FINAL_BULLET

#include "barrier.h"

// Bullet parameters
#define BULLET_SPEED (4) // Pixels per tick
#define BULLET_EXPLOSION_TICKS (15) // Ticks. How many ticks the bullet explosion should last
#define BULLET_MAX_R (5) // Pixels. Largest radius from bullet centroid to exterior of hit box


typedef struct Bullet {
    // Current position
    float position_x;
    float position_y;

    // Last drawn position
    float last_x;
    float last_y;

    // Pre-computed movement rates
    // Pixels per tick
    float delta_x;
    float delta_y;

    // Counter to track the duration of a bullet explosion
    int explosion_ticks;
} Bullet;


void bullet_init(Bullet *self, float x, float y, float angle);
void bullet_move(Bullet *self, int ticks);
int  bullet_check_barrier(const Bullet *self, const Barrier *barrier);
void bullet_explode(Bullet *self);

#endif
