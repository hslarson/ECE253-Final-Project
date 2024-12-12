#ifndef ECE253_FINAL_TANK
#define ECE253_FINAL_TANK

#include "bullet.h"
#include "barrier.h"

#define TANK_NUM_LIVES (3) // Number of lives for each tank
#define TANK_RESPAWN_COOLDOWN (50) // Number of ticks to wait before respawning

#define TANK_JOYSTICK_THRESHOLD (10) // [0,255] central dead zone of joystick.

// Tank movement speed
#define TANK_MAX_ROTATION_RATE (0.05) // Radians per pick
#define TANK_MAX_DRIVE_RATE (1.0f) // Pixels per tick

// Tank gun parameters
#define TANK_CANNON_LENGTH (25) // Pixels. Length of cannon barrel from tank centroid
#define TANK_CANNON_COOLDOWN (100) // Ticks. Minimum time between gun use

// Tank hit box extents (distance from centroid to edge in each direction)
// Top is in the negative y direction. Right is in the positive x direction
#define TANK_EXTENT_T (-15)
#define TANK_EXTENT_L (-10)
#define TANK_EXTENT_B ( 15)
#define TANK_EXTENT_R ( 10)

// Largest radius from tank centroid to exterior
// Must be >= sqrt( max(TANK_EXTENT_T^2, TANK_EXTENT_B^2) + max(TANK_EXTENT_L^2, TANK_EXTENT_R^2))
#define TANK_MAX_R (19)


typedef struct Tank {
    // Current position (pixels)
    float position_x;
    float position_y;

    // Orientation
    // 0    --> pointed towards screen top
    // pi/2 --> pointed towards screen right
    float angle;

    // Bounds of tank sprite
    // as it was last drawn to the screen
    int bbox_l_x;
    int bbox_t_y;
    int bbox_r_x;
    int bbox_b_y;

    // Movement direction
    float drive_rate; // Pixels per second. Positive is up on screen
    float steer_rate; // Radians per second. Positive is clockwise on screen

    // Tank health
    int lives; // Remaining lives
    int respawn_ticks; // Timer to delay tank respawning

    // Cannon cooldown
    int gun_cooldown;
} Tank;


void tank_init(Tank *self, int pos_x, int pos_y, float angle);
void tank_move(Tank *self, int ticks);
int  tank_check_barrier(const Tank *self, const Barrier *barrier);
int  tank_check_bullet(const Tank *self, const Bullet *bullet);
void tank_register_hit(Tank *self);
void tank_update_steering(Tank *self, int joystick_x, int joystick_y);
Bullet* tank_shoot(Tank *self);
void tank_draw(Tank* self, uint16_t screen[SCREEN_HEIGHT][SCREEN_WIDTH]);

#endif
