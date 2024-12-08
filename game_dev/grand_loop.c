#include "barrier.h"
#include "bullet.h"
#include "tank.h"

// Barrier objects
#define NUM_BARRIERS 4
Barrier barriers[NUM_BARRIERS];

// Bullet array
int num_bullets = 0;
Bullet bullets[4];

int main() {
    // Create tank objects
    Tank player1, player2;
    tank_init(&player1);
    tank_init(&player2);

    // Create barrier objects
    // TODO


    return 0;
}
// Grand loop
