// assets.h 
// Define all sprites

#ifndef ECE253_FINAL_ASSETS
#define ECE253_FINAL_ASSETS

#include "sprite.h"

// Tank sprites
#define NUM_TANK_SPRITES 7
extern const Sprite *tank_sprites[NUM_TANK_SPRITES];

// Bullet sprites
#define NUM_BULLET_SPRITES 2
extern const Sprite *bullet_sprites[NUM_BULLET_SPRITES];

// Heart sprite
extern const Sprite *heart_sprite;

#endif