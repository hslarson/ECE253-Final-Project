// game.h
// Grand loop for tank game

#ifndef ECE253_FINAL_GAME
#define ECE253_FINAL_GAME

#include "tank.h"

#define MAX_BULLETS 8

// Grand loop tasks
// "ticks" parameter denotes time since the task ran last
void t1_read_controller(int ticks);
void t2_move_objects(int ticks);
void t3_draw_objects(int ticks);

// Grand loop
int play_game(void);

// Helpers
void tank_update_position(Tank *tank, const Tank *other, int ticks);
void tank_respawn(Tank *tank, const Tank* other);

#endif