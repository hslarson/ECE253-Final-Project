#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H

#include <stdint.h>
#include <windows.h> // For Windows threading

#include "constants.h"

extern int running;

void sdl_init(void);
void sdl_cleanup(void);
void sdl_handle_input(void);
void sdl_update(void);
DWORD WINAPI sdl_thread_function(LPVOID arg);

#endif // SDL_WRAPPER_H
