#include "maps.h"
#include "constants.h"


// Define spawn points for map 1
const int spawn_points_x_map1[] = {80,  560};
const int spawn_points_y_map1[] = {264, 264};
const float spawn_angles_map1[] = {1.57079632679, 4.71238898038};

// Define spawn points for map 1
const Barrier barriers_map1[] = {
    { .l_x=COORD_MIN, .t_y=COORD_MIN, .r_x=640, .b_y=58, .color=0x252F }, // Top bar
    { .l_x=0, .t_y=470, .r_x=640, .b_y=COORD_MAX, .color=0x252F }, // Bottom bar
    { .l_x=COORD_MIN, .t_y=48, .r_x=10, .b_y=480, .color=0x252F }, // Left bar
    { .l_x=630, .t_y=48, .r_x=COORD_MAX, .b_y=480, .color=0x252F }, // Right bar
    { .l_x=430, .t_y=164, .r_x=450, .b_y=364, .color=0x888F }, // Left barrier
    { .l_x=190, .t_y=164, .r_x=210, .b_y=364, .color=0x888F }, // Right barrier
};

// Define map 1
const Map map1 = {
    .n_spawn_points = 2,
    .spawn_points_x = spawn_points_x_map1,
    .spawn_points_y = spawn_points_y_map1,
    .spawn_angles = spawn_angles_map1,
    .background_color = 0x384F, // RGBA (48, 128, 64, 255)
    .n_barriers = 6,
    .barriers = barriers_map1
};


// Compile maps into list
const Map *map_list[NUM_MAPS] = { &map1 };

// Define current map
const Map *current_map = &map1;


// Set the current map
void map_select(int map_idx) {
    if (map_idx >= NUM_MAPS) { map_idx = 0; }
    current_map = map_list[map_idx];
}
