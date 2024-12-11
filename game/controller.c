#include "controller.h"

// ==== FOR DEBUG ====
int p1_up = 0;
int p1_down = 0;
int p1_left = 0;
int p1_right = 0;
int p1_trig = 0;

int p2_up = 0;
int p2_down = 0;
int p2_left = 0;
int p2_right = 0;
int p2_trig = 0;
// ===================



// TODO: replace with actual I2C read
void controller_read(int index, int *x_val, int *y_val, int *trigger) {
    switch (index) {
    case 1:
        *x_val = 128*p1_right - 127*p1_left + 127;
        *y_val = 128*p1_up - 127*p1_down + 127;
        *trigger = p1_trig;
        break;
    case 2:
        *x_val = 128*p2_right - 127*p2_left + 127;
        *y_val = 128*p2_up - 127*p2_down + 127;
        *trigger = p2_trig;
        break;
    }
}