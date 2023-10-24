#include "slock_raylib.h"

#include "raylib.h"
#include <string.h>

struct gamectx {
    struct {
        int height;
        int width;
        int nlines_h;
        int nlines_v;
        int cell_size;
    } grid;
    struct {
        int width;
        int height;
    } display;
};
static struct gamectx _g;

static char _passwd[32];

void slock_raylib_init(void) {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(0, 0, "screen-lock");

    int display = GetCurrentMonitor();
    _g.display.width = GetMonitorWidth(display);
    _g.display.height = GetMonitorHeight(display);


    //SetExitKey(KEY_NULL); // disables ESC key from closing the window

    DisableCursor(); // Limit cursor to relative movement inside the window

    SetTargetFPS(60);

    if (!IsWindowFullscreen()) {

        // ToggleFullscreen does not scale the window to fit the monitor, it changes the monitor resolution to match the window.
        ToggleFullscreen();
    }

    memset(&_passwd, 0, sizeof(_passwd));

    _g.grid.cell_size = 30;
    _g.grid.width = ((1 + (_g.display.width / _g.grid.cell_size)) * _g.grid.cell_size);
    _g.grid.height = ((0 + (_g.display.height / _g.grid.cell_size)) * _g.grid.cell_size);
    _g.grid.nlines_h = 1 + (_g.grid.height / _g.grid.cell_size);
    _g.grid.nlines_v = 1 + (_g.grid.width / _g.grid.cell_size);
}

void slock_raylib_run(void) {
    // sets camera mid screen
    Camera2D cam = { 0 };
    cam.target = (Vector2){ _g.display.width/2.0f, _g.display.height/2.0f };
    cam.offset = cam.target;
    cam.rotation = 0.0f;
    cam.zoom = 1.10f;

    while (!WindowShouldClose()) {   // Detect window close button or ESC key
        BeginDrawing();

        BeginMode2D(cam);

        ClearBackground(RAYWHITE);

        // draw grid
        for (int i = 0; i < _g.grid.nlines_h; i++) {
            DrawLineV((Vector2){0, _g.grid.cell_size * i}, (Vector2){_g.grid.width, _g.grid.cell_size * i}, LIGHTGRAY);
        }
        for (int i = 0; i < _g.grid.nlines_v; i++) {
            DrawLineV((Vector2){_g.grid.cell_size * i, 0}, (Vector2){_g.grid.cell_size * i, _g.grid.height}, LIGHTGRAY);
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
