#ifndef ECE253_FINAL_GAME_SETTINGS
#define ECE253_FINAL_GAME_SETTINGS

#define NUM_LIVES (3)

#define JOYSTICK_THRESHOLD (10)

#define BULLET_SPEED (4) // Pixels per tick

#define TANK_MAX_ROTATION_RATE (0.3) // Radians per pick
#define TANK_MAX_DRIVE_RATE (1.0f) // Pixels per tick

#define TANK_WIDTH  (20) // Pixels 
#define TANK_LENGTH (30) // Pixels. Assumed > TANK_WIDTH

#define TANK_CANNON_LENGTH (25)
#define TANK_CANNON_COOLDOWN (100)

#define TANK_EXTENT_T (-15)
#define TANK_EXTENT_L (-10)
#define TANK_EXTENT_B ( 15)
#define TANK_EXTENT_R ( 10)

#define TWO_PI 6.28318530718f

#endif