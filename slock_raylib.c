#include "slock_raylib.h"

#include "raylib.h"

void slock_raylib_init(void) {
    InitWindow(300, 200, "screen-lock");

    SetTargetFPS(60);
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
    
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
}
