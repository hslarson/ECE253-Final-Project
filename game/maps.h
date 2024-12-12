// maps.h
// Set up map (barriers, spawn points, etc.)

#ifndef ECE253_FINAL_MAP
#define ECE253_FINAL_MAP

#include "barrier.h"
#include <stdint.h>

#define COORD_MIN INT16_MIN
#define COORD_MAX INT16_MAX


typedef struct Map {
    // Spawn points
    int n_spawn_points;
    const int* spawn_points_x;
    const int* spawn_points_y;
    const float* spawn_angles;

    // Color palette
    uint16_t background_color;
    
    // Barriers
    int n_barriers;
    const Barrier* barriers;
} Map;

#define NUM_MAPS 1
extern const Map *current_map;

void map_select(int map_idx);

#endif