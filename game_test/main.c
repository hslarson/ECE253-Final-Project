#include "sdl.h"
#include "game.h"

#include <SDL2/SDL.h>
#include <windows.h> // For threading
#include <stdio.h>

// Declare a handle for the game loop thread
static HANDLE game_thread;

// Start the game loop in a separate thread
DWORD WINAPI game_thread_function(LPVOID arg) {
    play_game(); // Run the main game loop here
    return 0;
}

int main(int argc, char *argv[]) {
    // Initialize SDL (this will run in the main thread)
    sdl_init();

    // Start game loop in a separate thread
    game_thread = CreateThread(NULL, 0, game_thread_function, NULL, 0, NULL);
    if (game_thread == NULL) {
        printf("CreateThread Error: %d\n", GetLastError());
        sdl_cleanup();
        return 1;
    }

    while (running) {
        sdl_handle_input();
        sdl_update();
        Sleep(100);
    }

    // Wait for the game loop to finish before cleaning up
    WaitForSingleObject(game_thread, 100);
    CloseHandle(game_thread);

    // Cleanup SDL (this will run in the main thread)
    sdl_cleanup();
    
    return 0;
}