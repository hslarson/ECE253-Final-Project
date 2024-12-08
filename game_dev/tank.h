#ifndef ECE253_FINAL_TANK
#define ECE253_FINAL_TANK

#include "bullet.h"


typedef struct Tank {
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
    float last_angle;

    // Movement direction
    float drive_rate; // Pixels per second 
    float steer_rate; // Radians per second

    // Remaining lives
    int lives;

    // Cannon cooldown
    int gun_cooldown;
} Tank;


void tank_init(Tank *self);
void tank_move(Tank *self);
void tank_unmove(Tank *self);
int  tank_check_barrier(const Tank *self, const Barrier *barrier);
int  tank_check_bullet(const Tank *self, const Bullet *bullet);
void tank_update_steering(Tank *self, int joystick_x, int joystick_y);
Bullet* tank_shoot(Tank *self);

#endif
