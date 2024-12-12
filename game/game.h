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
void restore_map(int l_x, int t_y, int r_x, int b_y);
void draw_scoreboard(Tank *tank, int heart_locs_x[TANK_NUM_LIVES]);

#endif
