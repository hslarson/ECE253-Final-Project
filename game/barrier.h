// barrier.h
// Define a structure for map barriers

#ifndef ECE253_FINAL_BARRIER
#define ECE253_FINAL_BARRIER

#include <stdint.h>

typedef struct Barrier {
    int l_x;
    int t_y;
    int r_x;
    int b_y;

    uint16_t color;
} Barrier;

void barrier_init(Barrier *self, int left, int top, int right, int bottom, uint16_t color);

#endif