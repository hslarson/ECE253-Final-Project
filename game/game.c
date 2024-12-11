#include "game.h"
#include "drawing.h"
#include "controller.h"
#include "tank.h"
#include "maps.h"
#include "barrier.h"
#include "bullet.h"
#include "constants.h"

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>


// === Globals ===
// Bullet array
int num_bullets = 0;
Bullet* bullets[MAX_BULLETS];

// Tank objects
Tank tank1, tank2;

// Delete
#include <windows.h>


// Main game loop
// Returns ID of winner (0: draw, 1: player 1 wins, 2: player 2 wins)
int play_game() {

    // Initialize timer
    // TODO
    uint32_t ticks, timer_val;
    timer_val = 0;

    // Choose map
    int map_idx = timer_val % NUM_MAPS;
    map_select(map_idx);

    // Initialize tanks
    tank_init(
        &tank1, 
        current_map.spawn_points_x[0],
        current_map.spawn_points_y[0],
        current_map.spawn_angles[0]
    );

    tank_init(
        &tank2,
        current_map.spawn_points_x[1],
        current_map.spawn_points_y[1],
        current_map.spawn_angles[1]
    );

    // Initialize screen
    restore_map(0, SCOREBOARD_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);
    draw_scoreboard(&tank1, &tank2);

    // TODO: Initialize to timer_val
    uint32_t task_timestamps[3] = {0};
    
    // Grand loop
    for (int t=0;;t = (t>=2) ? 0:t+1) {

        // TODO: Get current timer value
        timer_val++;

        // Compute ticks since last run & update timestamp
        ticks = timer_val - task_timestamps[t];
        task_timestamps[t] = timer_val;

        // Run task
        switch (t) {
            case 0: t1_read_controller(ticks); break;
            case 1: t2_move_objects(ticks); break;
            case 2: t3_draw_objects(ticks); break;
        }

        // TODO: do this when tank tries to respawn but can't?
        // Check tank lives
        if (tank1.lives <= 0 || tank2.lives <= 0) {
            // Clean up memory
            for (; num_bullets>0; num_bullets--) {
                free(bullets[num_bullets-1]);
            }

            // Return winner
            if (tank1.lives <= 0 && tank2.lives <= 0) { return 0; }
            else if (tank1.lives <= 0) { return 2; }
            else { return 1; }
        }

        // Delete
        Sleep(50);
    }
    
    return 0;
}


// Task 1: Read joystick and trigger state from each player's nunchuk
void t1_read_controller(int ticks) {
    // Joystick readings
    int j1_x, j1_y, j2_x, j2_y;

    // Trigger readings
    int trig1, trig2;

    // I2C read controller
    controller_read(1, &j1_x, &j1_y, &trig1);
    controller_read(2, &j2_x, &j2_y, &trig2);

    // Update steering parameters
    tank_update_steering(&tank1, j1_x, j1_y);
    tank_update_steering(&tank2, j2_x, j2_y);

    // Update gun cool-downs
    tank1.gun_cooldown = max(0, tank1.gun_cooldown - ticks);
    tank2.gun_cooldown = max(0, tank2.gun_cooldown - ticks);

    // Fire new bullets
    if (trig1) {printf("Tank 1 trig\n");}
    if (trig1 && num_bullets < MAX_BULLETS) {
        printf("Tank 1 shoot\n");
        Bullet* b = NULL;
        b = tank_shoot(&tank1);
        if (b) {
            printf("Bullet: (%f, %f)\n", b->position_x, b->position_y);
            bullets[num_bullets] = b;
            num_bullets++;
        }
    }

    if (trig2) {printf("Tank 1 trig\n");}
    if (trig2 && num_bullets < MAX_BULLETS) {
        Bullet* b = NULL;
        b = tank_shoot(&tank2);
        printf("Tank 2 shoot\n");
        if (b) {
            bullets[num_bullets] = b;
            num_bullets++;
        }
    }
}


// Task 2: Move objects (tanks/bullets) and check for collisions.
void t2_move_objects(int ticks) {

    // Update the position of tanks
    tank_update_position(&tank1, &tank2, ticks);
    tank_update_position(&tank2, &tank1, ticks);
    
    // Iterate bullets
    int bullet_hit;
    for (int i=0; i < num_bullets; i++) {
        // Move bullet
        bullet_move(bullets[i], ticks);

        // Check bullet collisions with barrier
        bullet_hit = 0;
        for (int n=0; !bullet_hit && n<current_map.n_barriers; n++) {
            bullet_hit = bullet_check_barrier(bullets[i], &current_map.barriers[n]);
        }

        // Check bullet collisions with tank
        // Note: checks only current position, 
        // not full trajectory since last run
        if (!bullet_hit) {
            if (tank_check_bullet(&tank1, bullets[i])) {
                bullet_hit = 1;
                tank1.lives--;
                tank1.respawn_ticks = TANK_RESPAWN_COOLDOWN;
            }
            if (tank_check_bullet(&tank2, bullets[i])) {
                bullet_hit = 1;

            }
        }

        // Destroy bullet if it hit something
        if (bullet_hit) {
            printf("Bullet hit\n");
            bullet_explode(bullets[i]);
        }
    }
}


// Task 3: Update frame buffer to 
void t3_draw_objects(int ticks) {

    // Remove tanks from screen
    remove_tank(&tank1);
    remove_tank(&tank2);

    // Iterate bullets
    for (int b=0; b<num_bullets;) {
        // Remove bullet from screen
        remove_bullet(bullets[b]);

        // Stop tracking bullet once it's done exploding
        if (bullets[b]->explosion_ticks) {
            bullets[b]->explosion_ticks -= ticks;

            if (bullets[b]->explosion_ticks <= 0) {
                free(bullets[b]);
                bullets[b] = bullets[num_bullets-1];
                num_bullets--;
                continue; // Don't increment index
            }
        }
        b++;
    }

    // Re-draw tanks
    draw_tank(&tank1);
    draw_tank(&tank2);

    // Re-draw bullets
    for (int b=0; b<num_bullets; b++) {
        draw_bullet(bullets[b]);
    }

    // Update scoreboard
    draw_scoreboard(&tank1, &tank2); 
}


// High-level tank position updater
// Checks tank respawn status, barrier collisions, etc.
void tank_update_position(Tank *tank, const Tank *other, int ticks) {

    // Check if tank is pending respawn
    if (tank->respawn_ticks) {
        tank->respawn_ticks -= ticks;
        
        // Respawn tank
        if (tank->respawn_ticks <= 0) {
            tank_respawn(tank, other);
        }
        return;
    } 

    // Save last position & move tank
    float pos_x = tank->position_x;
    float pos_y = tank->position_y;
    float angle = tank->angle;
    tank_move(tank, ticks);

    // Check for tank collisions with barrier
    // Revert movement if a collision is detected
    for (int i=0; i<current_map.n_barriers; i++) {
        if (tank_check_barrier(tank, &current_map.barriers[i])) {
            tank->position_x = pos_x;
            tank->position_y = pos_y;
            tank->angle = angle;
            break;
        }
    }
}


// Place the tank as far away from the other as possible
void tank_respawn(Tank *tank, const Tank* other) {
    // Check if respawn is allowed
    if (tank->lives <= 0) { return; }

    // Find respawn point that's furthest from other tank
    // Only consider x direction for simplicity
    int diff, max_diff = 0;
    int max_idx = 0;
    for (int i=0; i<current_map.n_spawn_points; i++) {
        diff = abs((int)other->position_x - current_map.spawn_points_x[i]);
        if (diff > max_diff) {
            max_diff = diff;
            max_idx = i;
        }
    }

    // Update tank state variables
    tank->position_x = current_map.spawn_points_x[max_idx];
    tank->position_y = current_map.spawn_points_y[max_idx];
    tank->gun_cooldown = 0;
    tank->respawn_ticks = 0;
}
