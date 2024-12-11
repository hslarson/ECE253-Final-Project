// controller.h
// Handles operations related to the Wii Nunchuk controller

#ifndef ECE253_FINAL_CONTROLLER
#define ECE253_FINAL_CONTROLLER

// ==== FOR DEBUG ====
extern int p1_up;
extern int p1_down;
extern int p1_left;
extern int p1_right;
extern int p1_trig;

extern int p2_up;
extern int p2_down;
extern int p2_left;
extern int p2_right;
extern int p2_trig;
// ===================

void controller_read(int index, int *x_val, int *y_val, int *trigger);

#endif 