#include "slock_raylib.h"

#include "raylib.h"

void slock_raylib_init(void) {
    int display = GetCurrentMonitor();
    int screen_width = GetMonitorWidth(display);
    int screen_height = GetMonitorHeight(display);

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(screen_width, screen_height, "screen-lock");

    SetTargetFPS(60);

    if (!IsWindowFullscreen()) {

        // ToggleFullscreen does not scale the window to fit the monitor, it changes the monitor resolution to match the window.
        ToggleFullscreen();
    }
}

void slock_raylib_run(void) {
    while (!WindowShouldClose()) {   // Detect window close button or ESC key
        BeginDrawing();

        ClearBackground(BLACK);

        Rectangle p1;

        p1.x = 10.0f;
        p1.y = 10.0f;
        p1.width = 100.0f;
        p1.height = 100.0f;

        DrawRectangleRec(p1, RED);
    
        int key = GetKeyPressed();

        DrawText(TextFormat("Key pressed: %i", key), 110, 110, 20, RAYWHITE);
    
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
}
