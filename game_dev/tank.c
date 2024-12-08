#include "tank.h"

#include <stdlib.h>
#include <math.h>

#include "game_constants.h"
#include "bullet.h"
#include "barrier.h"


// Initializer
void tank_init(Tank *self) {
    self->position_x = 0;
    self->position_y = 0;
    self->last_x = 0;
    self->last_y = 0;
    self->angle = 0;
    self->last_angle = 0;
    self->drive_rate = 0;
    self->steer_rate = 0;
    self->lives = NUM_LIVES;
    self->gun_cooldown = 0;
}


// Compute the next position of the tank
// given the current drive speed and steering angle
void tank_move(Tank *self) {
    // Compute angle after tick
    self->last_angle = self->angle;
    self->angle += self->steer_rate;
    if (self->angle > TWO_PI) { self->angle -= TWO_PI; }
    else if (self->angle < 0) { self->angle += TWO_PI; }
   
    // Compute position after tick
    self->last_y = self->position_y;
    self->last_x = self->position_x;
    if (self->steer_rate != 0) {
        self->position_x += self->drive_rate/self->steer_rate*(cos(self->last_angle) - cos(self->steer_rate+self->last_angle));
        self->position_y -= self->drive_rate/self->steer_rate*(sin(self->steer_rate+self->last_angle) - sin(self->last_angle)); // Invert y
    } else {
        self->position_y -= self->drive_rate; // Invert y
    }
}


// Set the tank position and angle back to the previous value
void tank_unmove(Tank *self) {
    self->angle = self->last_angle;
    self->position_y = self->last_y;
    self->position_x = self->last_x;
}


// Check if tank is colliding with the specified barrier
// Returns 1 if a collision is detected
int tank_check_barrier(const Tank *self, const Barrier *barrier) {

    // Coarse check
    // TODO
    
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
    float corner_x[4] = {tl_x, tr_x, bl_x, br_x};
    float corner_y[4] = {tl_y, tr_y, bl_y, br_y};

    // Check tank corner inside barrier
    for (int i=0; i<4; i++) {
        // Horizontal check
        if (corner_x[i] >= barrier->l_x && corner_x[i] <= barrier->r_x)  {
            // Vertical check
            if (corner_y[i] >= barrier->t_y && corner_y[i] <= barrier->b_y) {
                return 1;
            }
        }
    }

    // Shift tank to origin
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

    // Coarse check. Quickly ignore targets that aren't close
    if      (bullet->position_x > self->position_x + TANK_LENGTH) { return 0; }
    else if (bullet->position_x < self->position_x - TANK_LENGTH) { return 0; }
    else if (bullet->position_y > self->position_y + TANK_LENGTH) { return 0; }
    else if (bullet->position_y < self->position_y - TANK_LENGTH) { return 0; }

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


// Given a raw joystick input, 
// update the steering direction and drive speed
void tank_update_steering(Tank *self, int joystick_x, int joystick_y) {
    // Set drive rate
    if (abs(joystick_y-128) >= JOYSTICK_THRESHOLD) {
        self->drive_rate = ((float)joystick_y*2/255-1) * TANK_MAX_DRIVE_RATE;
    } else { 
        self->drive_rate = 0; // Below threshold (no input)
    }

    // Set steering rate
    if (abs(joystick_x-128) >= JOYSTICK_THRESHOLD) {
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
    if (self->gun_cooldown > 0) { return NULL; }

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
