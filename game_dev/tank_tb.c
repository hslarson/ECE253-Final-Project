#include "tank.h"
#include "bullet.h"
#include "barrier.h"
#include "game_constants.h"

#include <stdio.h>

int main() {
    // Create tank
    Tank t;

    // Test init
    tank_init(&t);
    printf("Lives: %d\n", t.lives);

    // Test steering
    tank_update_steering(&t, 128, 128);
    printf("1. Drive: %f; Steer: %f\n", t.drive_rate, t.steer_rate); // Expect 0, 0
    
    tank_update_steering(&t, 120, 136);
    printf("2. Drive: %f; Steer: %f\n", t.drive_rate, t.steer_rate); // Expect 0, 0

    tank_update_steering(&t, 255, 128);
    printf("3. Drive: %f; Steer: %f\n", t.drive_rate, t.steer_rate); // Expect max positive rotation

    tank_update_steering(&t, 0, 128);
    printf("4. Drive: %f; Steer: %f\n", t.drive_rate, t.steer_rate); // Expect max negative rotation

    tank_update_steering(&t, 128, 255);
    printf("5. Drive: %f; Steer: %f\n", t.drive_rate, t.steer_rate); // Expect max positive drive

    tank_update_steering(&t, 128, 0);
    printf("6. Drive: %f; Steer: %f\n", t.drive_rate, t.steer_rate); // Expect max negative drive


    t.position_x = 100;
    t.position_y = 100;
    t.angle = 0;

    // Forward, turn left
    printf("\nForward left\n");
    tank_update_steering(&t, 0, 255);
    for (int i = 0; i < 10; i++) {
        tank_move(&t);
        printf("%d --> (%f, %f) %f\n", i, t.position_x, t.position_y, t.angle*360/TWO_PI);
    }

    printf("\nRevert Move\n");
    tank_unmove(&t);
    printf("(%f, %f) %f\n", t.position_x, t.position_y, t.angle*360/TWO_PI);


    t.position_x = 100;
    t.position_y = 100;
    t.angle = 0;

    // Backward, turn left
    printf("\nReverse left\n");
    tank_update_steering(&t, 0, 0);
    for (int i = 0; i < 10; i++) {
        tank_move(&t);
        printf("%d --> (%f, %f) %f\n", i, t.position_x, t.position_y, t.angle*360/TWO_PI);
    }

    t.position_x = 100;
    t.position_y = 100;
    t.angle = 0;

    // Forward, turn right
    printf("\nForward right\n");
    tank_update_steering(&t, 255, 255);
    for (int i = 0; i < 10; i++) {
        tank_move(&t);
        printf("%d --> (%f, %f) %f\n", i, t.position_x, t.position_y, t.angle*360/TWO_PI);
    }

    t.position_x = 100;
    t.position_y = 100;
    t.angle = 0;

    // Backward, turn right
    printf("\nReverse right\n");
    tank_update_steering(&t, 255, 0);
    for (int i = 0; i < 10; i++) {
        tank_move(&t);
        printf("%d --> (%f, %f) %f\n", i, t.position_x, t.position_y, t.angle*360/TWO_PI);
    }

    t.position_x = 100;
    t.position_y = 100;
    t.angle = 0;

    // Rotate clockwise
    printf("\nClockwise\n");
    tank_update_steering(&t, 255, 128);
    for (int i = 0; i < 24; i++) {
        tank_move(&t);
        printf("%d --> (%f, %f) %f\n", i, t.position_x, t.position_y, t.angle*360/TWO_PI);
    }

    t.position_x = 100;
    t.position_y = 100;
    t.angle = 0;

    // Rotate counterclockwise
    printf("\nCounterclockwise\n");
    tank_update_steering(&t, 0, 128);
    for (int i = 0; i < 24; i++) {
        tank_move(&t);
        printf("%d --> (%f, %f) %f\n", i, t.position_x, t.position_y, t.angle*360/TWO_PI);
    }

    t.position_x = 100;
    t.position_y = 100;
    t.angle = 0;

    // Forward
    printf("\nForward\n");
    tank_update_steering(&t, 128, 255);
    for (int i = 0; i < 10; i++) {
        tank_move(&t);
        printf("%d --> (%f, %f) %f\n", i, t.position_x, t.position_y, t.angle*360/TWO_PI);
    }

    t.position_x = 100;
    t.position_y = 100;
    t.angle = 0;

    // Bottom
    printf("\nReverse\n");
    tank_update_steering(&t, 128, 0);
    for (int i = 0; i < 10; i++) {
        tank_move(&t);
        printf("%d --> (%f, %f) %f\n", i, t.position_x, t.position_y, t.angle*360/TWO_PI);
    }

    // Check bullets
    Bullet b;
    int hit;

    t.position_x = 0;
    t.position_y = 0;
    t.angle = 0.349065850399; // 20 degrees

    printf("\nBullet Hits:\n");

    bullet_init(&b, 13, -10, 0);
    hit = tank_check_bullet(&t, &b); 
    printf("1. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // HIT, top right
    
    bullet_init(&b, -4, -16, 0);
    hit = tank_check_bullet(&t, &b); 
    printf("2. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // HIT, top left

    bullet_init(&b, 4, 16, 0);
    hit = tank_check_bullet(&t, &b); 
    printf("3. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // HIT, bottom right

    bullet_init(&b, -12, 10, 0);
    hit = tank_check_bullet(&t, &b); 
    printf("4. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // HIT, bottom left
    
    bullet_init(&b, -14, 13, 0);
    hit = tank_check_bullet(&t, &b); 
    printf("5. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // MISS, bottom left

    bullet_init(&b, 12,0, 0);
    hit = tank_check_bullet(&t, &b); 
    printf("6. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // MISS, right
    
    bullet_init(&b, 10, -13, 0);
    hit = tank_check_bullet(&t, &b); 
    printf("7. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // MISS, top

    bullet_init(&b, -10, -3, 0);
    hit = tank_check_bullet(&t, &b); 
    printf("8. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // MISS, left


    // Check collisions with barrier
    Barrier bar;
    
    t.position_x = 0;
    t.position_y = 0;
    t.angle = 0.349065850399; // 20 degrees

    printf("\nBarrier Hits:\n");

    // tank corner in barrier
    barrier_init(&bar, 10, -20, 30, 0);
    hit = tank_check_barrier(&t, &bar); 
    printf("1. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // HIT, tank top right

    barrier_init(&bar,-20, -30, 5, -15);
    hit = tank_check_barrier(&t, &bar); 
    printf("2. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // HIT, tank top left

    barrier_init(&bar,-20,0,-10,15);
    hit = tank_check_barrier(&t, &bar); 
    printf("3. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // HIT, tank bottom left

    barrier_init(&bar,-5,15,10,20);
    hit = tank_check_barrier(&t, &bar); 
    printf("4. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // HIT, tank bottom right

    // barrier corner in tank
    barrier_init(&bar, 5,0,20,10);
    hit = tank_check_barrier(&t, &bar); 
    printf("5. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // HIT, tank right

    barrier_init(&bar, -20,-10,-5,0);
    hit = tank_check_barrier(&t, &bar); 
    printf("6. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // HIT, tank left

    barrier_init(&bar, 0,-30,15,-13);
    hit = tank_check_barrier(&t, &bar); 
    printf("7. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // HIT, tank top

    barrier_init(&bar, -20,12,0,30);
    hit = tank_check_barrier(&t, &bar); 
    printf("8. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // HIT, tank left

    // Misses
    barrier_init(&bar,-20,-20,-10,-10);
    hit = tank_check_barrier(&t, &bar); 
    printf("9. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // MISS, left

    barrier_init(&bar, 0,-30,10,-17);
    hit = tank_check_barrier(&t, &bar); 
    printf("10. ");
    if (hit) { printf("HIT\n"); } else { printf("MISS\n"); } // MISS, top

    
    // Bullet* tank_shoot(Tank *self);

    return 0;
}
