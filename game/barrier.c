#include "barrier.h"
#include <stdint.h>

void barrier_init(Barrier *self, int left, int top, int right, int bottom, uint16_t color) {
    self->l_x = left;
    self->t_y = top;
    self->r_x = right;
    self->b_y = bottom;

    self->color = color;
}
