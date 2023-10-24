#include "slock_raylib.h"

#include "raylib.h"
#include <string.h>

static char _passwd[32];
static int screen_height;
static int screen_width;
static int cell_size = 30;

void slock_raylib_init(void) {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(screen_width, screen_height, "screen-lock");

    int display = GetCurrentMonitor();
    screen_width = GetMonitorWidth(display);
    screen_height = GetMonitorHeight(display);


    //SetExitKey(KEY_NULL); // disables ESC key from closing the window

    DisableCursor(); // Limit cursor to relative movement inside the window

    SetTargetFPS(60);

    if (!IsWindowFullscreen()) {

        // ToggleFullscreen does not scale the window to fit the monitor, it changes the monitor resolution to match the window.
        ToggleFullscreen();
    }

    memset(&_passwd, 0, sizeof(_passwd));
}

void slock_raylib_run(void) {
    Camera2D camera1 = { 0 };
    camera1.target = (Vector2){ screen_width/2.0f, screen_height/2.0f };
    camera1.offset = (Vector2){ screen_width/2.0f, screen_height/2.0f };
    camera1.rotation = 0.0f;
    camera1.zoom = 0.90f;

    while (!WindowShouldClose()) {   // Detect window close button or ESC key
        BeginDrawing();

        BeginMode2D(camera1);

        ClearBackground(RAYWHITE);

        // draw grid
        int nlines_h = 1 + (screen_height / cell_size);
        for (int i = 0; i < nlines_h; i++) {
            DrawLineV((Vector2){0, cell_size * i}, (Vector2){screen_width, cell_size * i}, LIGHTGRAY);
        }
        int nlines_v = 1 + (screen_width / cell_size);
        for (int i = 0; i < nlines_v; i++) {
            DrawLineV((Vector2){cell_size * i, 0}, (Vector2){cell_size * i, screen_height}, LIGHTGRAY);
        }

        int key = GetKeyPressed();
        if (key == KEY_ENTER) {
            break;
        }

        DrawText(TextFormat("Key pressed: %i", key), 110, 110, 20, RAYWHITE);

        EndMode2D();
    
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
}
