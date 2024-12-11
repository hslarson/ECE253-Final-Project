#include "tank.h"
#include "bullet.h"
#include "barrier.h"
#include "constants.h"

#include <stdlib.h>
#include <stdint.h>
#include <math.h>


// Initializer
void tank_init(Tank *self, int pos_x, int pos_y, float angle) {
    self->position_x = (float)pos_x;
    self->position_y = (float)pos_y;

    self->last_x = -1;
    self->last_y = -1;

    self->angle = angle;
    self->last_angle = 0;

    self->drive_rate = 0;
    self->steer_rate = 0;

    self->lives = TANK_NUM_LIVES;
    self->respawn_ticks = 0;

    self->gun_cooldown = 0;
}


// Compute the position of the tank after a given number of ticks
// given the current drive speed and steering angle
void tank_move(Tank *self, int ticks) {
    // Tanks do not move when waiting to respawn
    if (self->respawn_ticks) { return; }

    // Compute position after n ticks
    if (self->steer_rate != 0) {
        float delta_angle = self->steer_rate * ticks;
        float turn_radius = self->drive_rate / self->steer_rate;
        self->position_x += turn_radius*(cos(self->angle) - cos(delta_angle+self->angle));
        self->position_y += turn_radius*(sin(self->angle) - sin(delta_angle+self->angle)); // Invert y
    
        // Compute angle after n ticks
        self->angle += delta_angle;
        if (self->angle > TWO_PI) { self->angle -= TWO_PI; }
        else if (self->angle < 0) { self->angle += TWO_PI; }
    }
    else {
        // Avoid div. by zero for zero turn radius
        self->position_x += self->drive_rate*ticks*sin(self->angle);
        self->position_y -= self->drive_rate*ticks*cos(self->angle);; // Invert y
    }
}


// Check if tank is colliding with the specified barrier
// Returns 1 if a collision is detected
int tank_check_barrier(const Tank *self, const Barrier *barrier) {

    // Coarse check. Quickly ignore barriers that aren't close
    if (self->respawn_ticks) { return 0; }
    else if (self->position_x + TANK_MAX_R < barrier->l_x) { return 0; }
    else if (self->position_x - TANK_MAX_R > barrier->r_x) { return 0; }
    else if (self->position_y + TANK_MAX_R < barrier->t_y) { return 0; }
    else if (self->position_y - TANK_MAX_R > barrier->b_y) { return 0; }

    // Fine check
    // Get tank corners (rotate extents and shift)
    float cos_theta = cos(self->angle);
    float sin_theta = sin(self->angle);

    float tl_x = TANK_EXTENT_L*cos_theta - TANK_EXTENT_T*sin_theta + self->position_x;
    float tl_y = TANK_EXTENT_L*sin_theta + TANK_EXTENT_T*cos_theta + self->position_y;

    float tr_x = TANK_EXTENT_R*cos_theta - TANK_EXTENT_T*sin_theta + self->position_x;
    float tr_y = TANK_EXTENT_R*sin_theta + TANK_EXTENT_T*cos_theta + self->position_y;

    float bl_x = TANK_EXTENT_L*cos_theta - TANK_EXTENT_B*sin_theta + self->position_x;
    float bl_y = TANK_EXTENT_L*sin_theta + TANK_EXTENT_B*cos_theta + self->position_y;

    float br_x = TANK_EXTENT_R*cos_theta - TANK_EXTENT_B*sin_theta + self->position_x;
    float br_y = TANK_EXTENT_R*sin_theta + TANK_EXTENT_B*cos_theta + self->position_y;

    // Matrixify
    float t_corner_x[4] = {tl_x, tr_x, bl_x, br_x};
    float t_corner_y[4] = {tl_y, tr_y, bl_y, br_y};

    // Check tank corner inside barrier
    for (int i=0; i<4; i++) {
        // Horizontal check
        if (t_corner_x[i] >= barrier->l_x && t_corner_x[i] <= barrier->r_x)  {
            // Vertical check
            if (t_corner_y[i] >= barrier->t_y && t_corner_y[i] <= barrier->b_y) {
                return 1;
            }
        }
    }

    // Shift tank & barrier such that tank centroid is at (0,0)
    float barr_l = barrier->l_x - self->position_x;
    float barr_t = barrier->t_y - self->position_y;
    float barr_r = barrier->r_x - self->position_x;
    float barr_b = barrier->b_y - self->position_y;

    // Rotate barrier corners
    tl_x =  barr_l*cos_theta + barr_t*sin_theta;
    tl_y = -barr_l*sin_theta + barr_t*cos_theta;

    tr_x =  barr_r*cos_theta + barr_t*sin_theta;
    tr_y = -barr_r*sin_theta + barr_t*cos_theta;

    bl_x =  barr_l*cos_theta + barr_b*sin_theta;
    bl_y = -barr_l*sin_theta + barr_b*cos_theta;

    br_x =  barr_r*cos_theta + barr_b*sin_theta;
    br_y = -barr_r*sin_theta + barr_b*cos_theta;

    // Matrixify
    float b_corner_x[4] = {tl_x, tr_x, bl_x, br_x};
    float b_corner_y[4] = {tl_y, tr_y, bl_y, br_y};

    // Check barrier corner inside tank
    for (int i=0; i<4; i++) {
        // Horizontal check
        if (b_corner_x[i] >= TANK_EXTENT_L && b_corner_x[i] <= TANK_EXTENT_R)  {
            // Vertical check
            if (b_corner_y[i] >= TANK_EXTENT_T && b_corner_y[i] <= TANK_EXTENT_B) {
                return 1;
            }
        }
    }
    return 0;
}


// Check if a bullet collided with a tank
// Return 1 if a hit was detected
int tank_check_bullet(const Tank *self, const Bullet *bullet) {

    // Coarse check. Quickly ignore bullets that aren't close
    if (bullet->explosion_ticks || self->respawn_ticks) { return 0; }
    else if (self->position_x + TANK_MAX_R < bullet->position_x) { return 0; }
    else if (self->position_x - TANK_MAX_R > bullet->position_x) { return 0; }
    else if (self->position_y + TANK_MAX_R < bullet->position_y) { return 0; }
    else if (self->position_y - TANK_MAX_R > bullet->position_y) { return 0; }

    // Precise check
    // Shift tank centroid to origin
    float x_vect = bullet->position_x - self->position_x;
    float y_vect = bullet->position_y - self->position_y;

    // Rotate bullet around origin
    // This lets us treat the tank as having zero angle
    float cos_theta = cos(self->angle);
    float sin_theta = sin(self->angle);
    float blt_x =  x_vect*cos_theta + y_vect*sin_theta;
    float blt_y = -x_vect*sin_theta + y_vect*cos_theta;

    // Check bounds
    if (blt_x >= TANK_EXTENT_L && blt_x <= TANK_EXTENT_R) {
        if (blt_y >= TANK_EXTENT_T && blt_y <= TANK_EXTENT_B) {
            return 1;
        }
    }
    return 0;
}


// Update state variables when a tank has been hit by a bullet
void tank_register_hit(Tank *self) {
    self->lives--;
    self->respawn_ticks = TANK_RESPAWN_COOLDOWN;
}  


// Given a raw joystick input, 
// update the steering direction and drive speed
void tank_update_steering(Tank *self, int joystick_x, int joystick_y) {
    // Set drive rate
    if (abs(joystick_y-128) >= TANK_JOYSTICK_THRESHOLD) {
        self->drive_rate = ((float)joystick_y*2/255-1) * TANK_MAX_DRIVE_RATE;
    } else { 
        self->drive_rate = 0; // Below threshold (no input)
    }

    // Set steering rate
    if (abs(joystick_x-128) >= TANK_JOYSTICK_THRESHOLD) {
        self->steer_rate = ((float)joystick_x*2/255-1) * TANK_MAX_ROTATION_RATE;
        if (self->drive_rate < 0) { self->steer_rate *= -1; }
    } else {
        self->steer_rate = 0; // Below threshold (no input)
    }
}


// Fire a bullet
// Returns a pointer to a bullet object (or null)
Bullet* tank_shoot(Tank *self) {
    // Gun is on cooldown, can't fire
    // Respawning tanks also cannot fire
    if (self->gun_cooldown > 0 || self->respawn_ticks) { return NULL; }

    // Create new bullet object
    Bullet* b = (Bullet*)malloc(sizeof(Bullet));
    if (b) {
        // Set cooldown
        self->gun_cooldown = TANK_CANNON_COOLDOWN; // TODO: decrement somewhere

        // Initialize bullet
        float muzzle_x =  TANK_CANNON_LENGTH*sin(self->angle) + self->position_x;
        float muzzle_y = -TANK_CANNON_LENGTH*cos(self->angle) + self->position_y;

        bullet_init(b, muzzle_x, muzzle_y, self->angle);
    }
    return b;
}


// Draw tank's current location on screen
// Erase old location before calling to prevent artifacts
void tank_draw(Tank* self, uint16_t screen[SCREEN_HEIGHT][SCREEN_WIDTH]) {
    // Rotate sprite
    // Blit sprite

    // TODO: replace with sprite stuff
    int t_y = (int)round(self->position_y) + TANK_EXTENT_T;
    int l_x = (int)round(self->position_x) + TANK_EXTENT_L;
    int b_y = (int)round(self->position_y) + TANK_EXTENT_B;
    int r_x = (int)round(self->position_x) + TANK_EXTENT_R;

    int start_row = max(MAP_MIN_Y, t_y);
    int start_col = max(MAP_MIN_X, l_x);
    int stop_row  = min(MAP_MAX_Y, b_y+1);
    int stop_col  = min(MAP_MAX_X, r_x+1);
    
    // Iterate rows
    for (int r=start_row; r<stop_row; r++) {
        // Iterate cols
        for (int c=start_col; c<stop_col; c++) {
            screen[r][c] = (uint16_t)0x222F;
        }
    }

    // Update last drawn location
    self->last_x = self->position_x;
    self->last_y = self->position_y;
    self->last_angle = self->angle;
}


// Get the bounding box of the bullet
// as it was last drawn to the screen
void tank_visual_bb(const Tank *self, int *l_x, int *t_y, int *r_x, int *b_y) {
    // Compute tank corners
    // Get tank corners (rotate extents and shift)
    float cos_theta = cos(self->last_angle);
    float sin_theta = sin(self->last_angle);

    int top_extent = min(TANK_EXTENT_T, -TANK_CANNON_LENGTH); // Max negative y extent
    float tl_x = TANK_EXTENT_L*cos_theta - top_extent*sin_theta + self->last_x;
    float tl_y = TANK_EXTENT_L*sin_theta + top_extent*cos_theta + self->last_y;

    float tr_x = TANK_EXTENT_R*cos_theta - top_extent*sin_theta + self->last_x;
    float tr_y = TANK_EXTENT_R*sin_theta + top_extent*cos_theta + self->last_y;

    float bl_x = TANK_EXTENT_L*cos_theta - TANK_EXTENT_B*sin_theta + self->last_x;
    float bl_y = TANK_EXTENT_L*sin_theta + TANK_EXTENT_B*cos_theta + self->last_y;

    float br_x = TANK_EXTENT_R*cos_theta - TANK_EXTENT_B*sin_theta + self->last_x;
    float br_y = TANK_EXTENT_R*sin_theta + TANK_EXTENT_B*cos_theta + self->last_y;

    // Matrixify
    int t_corner_x[3] = {round(tr_x), round(bl_x), round(br_x)};
    int t_corner_y[3] = {round(tr_y), round(bl_y), round(br_y)};

    // Find min/max x and y values
    *l_x = tl_x;
    *r_x = tl_x;
    for (int i=0; i<3; i++) {
        if (t_corner_x[i] < *l_x) { *l_x = t_corner_x[i]; }
        else if (t_corner_x[i] > *r_x) { *r_x = t_corner_x[i]; }
    }

    *t_y = tl_y;
    *b_y = tl_y;
    for (int i=0; i<3; i++) {
        if (t_corner_y[i] < *t_y) { *t_y = t_corner_y[i]; }
        else if (t_corner_y[i] > *b_y) { *b_y = t_corner_y[i]; }
    }
}
