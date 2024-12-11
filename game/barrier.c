#include "barrier.h"

void barrier_init(Barrier *self, int left, int top, int right, int bottom) {
    self->l_x = left;
    self->t_y = top;
    self->r_x = right;
    self->b_y = bottom;
}
