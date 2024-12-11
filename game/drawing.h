// drawing.h
// Functions for writing to frame buffer

#ifndef ECE253_FINAL_DRAWING
#define ECE253_FINAL_DRAWING

#include "constants.h"
#include "tank.h"
#include "bullet.h"
#include "barrier.h"

#include <stdint.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))


// Global screen object
extern uint16_t screen[SCREEN_HEIGHT][SCREEN_WIDTH];

// Drawing functions
void draw_start_menu(void);
void draw_end_menu(int winner);

void restore_map(int l_x, int t_y, int r_x, int b_y);

void remove_tank(const Tank* tank);
void remove_bullet(const Bullet* bullet);

void draw_scoreboard(const Tank* tank1, const Tank* tank2);
void draw_tank(Tank* tank);
void draw_bullet(Bullet* bullet);

#endif
