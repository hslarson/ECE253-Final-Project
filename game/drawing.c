#include "drawing.h"
#include "constants.h"
#include "maps.h"

#include <math.h>

// Define screen
uint16_t screen[SCREEN_HEIGHT][SCREEN_WIDTH] = {0};


// Draw title screen
void draw_start_menu() {
    ; // TODO
}

// Draw game end screen that shows winner
void draw_end_menu(int winner) {
    ; // TODO
}


// Restores the background and barriers within a specified region
// TODO: Can we keep constant background frames so I can just memcpy?
void restore_map(int l_x, int t_y, int r_x, int b_y) {
    // Restore background color
    int start_row = max(0, t_y);
    int start_col = max(0, l_x);

    int stop_row = min(SCREEN_HEIGHT, b_y+1);
    int stop_col = min(SCREEN_WIDTH,  r_x+1);
    
    // Iterate rows
    for (int r=start_row; r<stop_row; r++) {
        // Iterate cols
        for (int c=start_col; c<stop_col; c++) {
            screen[r][c] = current_map.background_color;
        }
    }

    // Iterate barriers to find intersections
    int l_int, t_int, r_int, b_int;
    for (int b=0; b<current_map.n_barriers; b++) {

        t_int = max(t_y, current_map.barriers[b].t_y);
        b_int = min(b_y, current_map.barriers[b].b_y);

        l_int = max(l_x, current_map.barriers[b].l_x);
        r_int = min(r_x, current_map.barriers[b].r_x);

        if (t_int >= b_int || l_int >= r_int) { continue; }

        start_row = max(0, t_int);
        start_col = max(0, l_int);

        stop_row = min(SCREEN_HEIGHT, b_int+1);
        stop_col = min(SCREEN_WIDTH,  r_int+1);

        // Iterate rows
        for (int r=start_row; r<stop_row; r++) {
            // Iterate cols
            for (int c=start_col; c<stop_col; c++) {
                screen[r][c] = current_map.barrier_color;
            }
        }
    }
}


// Replace a tank with background
// Uses last position of tank rather than current
void remove_tank(const Tank* tank) {
    
    // Compute tank corners
    // Get tank corners (rotate extents and shift)
    float cos_theta = cos(tank->last_angle);
    float sin_theta = sin(tank->last_angle);

    int top_extent = min(TANK_EXTENT_T, -TANK_CANNON_LENGTH); // Max negative y extent
    float tl_x = TANK_EXTENT_L*cos_theta - top_extent*sin_theta + tank->last_x;
    float tl_y = TANK_EXTENT_L*sin_theta + top_extent*cos_theta + tank->last_y;

    float tr_x = TANK_EXTENT_R*cos_theta - top_extent*sin_theta + tank->last_x;
    float tr_y = TANK_EXTENT_R*sin_theta + top_extent*cos_theta + tank->last_y;

    float bl_x = TANK_EXTENT_L*cos_theta - TANK_EXTENT_B*sin_theta + tank->last_x;
    float bl_y = TANK_EXTENT_L*sin_theta + TANK_EXTENT_B*cos_theta + tank->last_y;

    float br_x = TANK_EXTENT_R*cos_theta - TANK_EXTENT_B*sin_theta + tank->last_x;
    float br_y = TANK_EXTENT_R*sin_theta + TANK_EXTENT_B*cos_theta + tank->last_y;

    // Matrixify
    float t_corner_x[3] = {tr_x, bl_x, br_x};
    float t_corner_y[3] = {tr_y, bl_y, br_y};

    // Find min/max x and y values
    int min_x = tl_x;
    int max_x = tl_x;
    for (int i=0; i<3; i++) {
        if (t_corner_x[i] < min_x) { min_x = t_corner_x[i]; }
        else if (t_corner_x[i] > max_x) { max_x = t_corner_x[i]; }
    }

    int min_y = tl_y;
    int max_y = tl_y;
    for (int i=0; i<3; i++) {
        if (t_corner_y[i] < min_y) { min_y = t_corner_y[i]; }
        else if (t_corner_y[i] > max_y) { max_y = t_corner_y[i]; }
    }

    // Restore background
    restore_map(min_x, min_y, max_x, max_y);
}


// Replace a bullet with background
// Uses last position of bullet rather than current
void remove_bullet(const Bullet* bullet) {
    // Get bullet extents
    int min_x = bullet->last_x - BULLET_MAX_R;
    int min_y = bullet->last_y - BULLET_MAX_R;
    int max_x = bullet->last_x + BULLET_MAX_R;
    int max_y = bullet->last_y + BULLET_MAX_R;
    
    // Restore background
    restore_map(min_x, min_y, max_x, max_y);
}


// Draw the scoreboard (player lives, gun cool-downs)
void draw_scoreboard(const Tank* tank1, const Tank* tank2) {
    // TODO
}


// Draw the tank in its current position
// Should be called after remove_tank()
void draw_tank(Tank* tank) {
    
    // Rotate sprite
    // Blit sprite

    // TODO: replace
    int t_y = (int)round(tank->position_y) + TANK_EXTENT_T;
    int l_x = (int)round(tank->position_x) + TANK_EXTENT_L;
    int b_y = (int)round(tank->position_y) + TANK_EXTENT_B;
    int r_x = (int)round(tank->position_x) + TANK_EXTENT_R;

    int start_row = max(0, t_y);
    int start_col = max(0, l_x);
    int stop_row  = min(SCREEN_HEIGHT, b_y+1);
    int stop_col  = min(SCREEN_WIDTH,  r_x+1);
    
    // Iterate rows
    for (int r=start_row; r<stop_row; r++) {
        // Iterate cols
        for (int c=start_col; c<stop_col; c++) {
            screen[r][c] = (uint16_t)0x222F;
        }
    }

    // Update last drawn location
    tank->last_x = tank->position_x;
    tank->last_y = tank->position_y;
    tank->last_angle = tank->angle;
}

#include <stdio.h>


// Draw the tank in its current position
// Should be called after remove_tank()
void draw_bullet(Bullet* bullet) {

    // Rotate sprite
    // Blit sprite

    // TODO: replace
    int t_y = (int)round(bullet->position_y) - 4;
    int l_x = (int)round(bullet->position_x) - 4;
    int b_y = (int)round(bullet->position_y) + 4;
    int r_x = (int)round(bullet->position_x) + 4;

    int start_row = max(0, t_y);
    int start_col = max(0, l_x);
    int stop_row  = min(SCREEN_HEIGHT, b_y+1);
    int stop_col  = min(SCREEN_WIDTH,  r_x+1);

    printf("%d, %d, %d, %d\n", start_row, start_col, stop_row, stop_col);
    
    // Iterate rows
    for (int r=start_row; r<stop_row; r++) {
        // Iterate cols
        for (int c=start_col; c<stop_col; c++) {
            screen[r][c] = (uint16_t)0xF00F; // TODO: define
        }
    }

    // Update last drawn location
    bullet->last_x = bullet->position_x;
    bullet->last_y = bullet->position_y;
}
