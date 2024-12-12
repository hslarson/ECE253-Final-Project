// bullet.h
// Define a structure for tank projectiles

#ifndef ECE253_FINAL_BULLET
#define ECE253_FINAL_BULLET

#include "barrier.h"
#include "constants.h"

// Bullet parameters
#define BULLET_SPEED (4) // Pixels per tick
#define BULLET_EXPLOSION_TICKS (15) // Ticks. How many ticks the bullet explosion should last


typedef struct Bullet {
    // Current position
    float position_x;
    float position_y;

    // Pre-computed movement rates
    // Pixels per tick
    float delta_x;
    float delta_y;

    // Bounds of tank sprite
    // as it was last drawn to the screen
    int bbox_l_x;
    int bbox_t_y;
    int bbox_r_x;
    int bbox_b_y;

    // Counter to track the duration of a bullet explosion
    int explosion_ticks;
} Bullet;


void bullet_init(Bullet *self, float x, float y, float angle);
void bullet_move(Bullet *self, int ticks);
int  bullet_check_barrier(const Bullet *self, const Barrier *barrier);
void bullet_explode(Bullet *self);
void bullet_draw(Bullet *self, uint16_t screen[SCREEN_HEIGHT][SCREEN_WIDTH]);

#endif
