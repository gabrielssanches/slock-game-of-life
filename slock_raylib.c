#include "slock_raylib.h"

#include "raylib.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

struct gol {
    int cell_nh;
    int cell_nv;
    unsigned int* cell_array;
};

struct gamectx {
    struct gol gol;
    struct {
        int width;
        int height;
    } display;
    struct {
        int size;
        int nh;
        int nv;
    } grid;
};
static struct gamectx _g;

static char _passwd[32];

#define CELL_ALIVE  (1 << 0)
#define CELL_BORN   (1 << 1)
#define CELL_KILL   (1 << 2)
#define CELL_GET_AGE(x)  (((x) & 0xFFFF0000) >> 4)

static void gol_create(struct gol* gol, const int ncells_horizontal, const int ncells_vertical) {
    gol->cell_nv = ncells_vertical;
    gol->cell_nh = ncells_horizontal;
    int ncells = gol->cell_nh * gol->cell_nv;
    gol->cell_array = malloc(sizeof(unsigned int) * ncells);
    memset(gol->cell_array, 0, sizeof(unsigned int) * ncells);

    gol->cell_array[0] = CELL_ALIVE;
    gol->cell_array[gol->cell_nh -1] = CELL_ALIVE;
    gol->cell_array[((gol->cell_nv -1) * gol->cell_nh)] = CELL_ALIVE;
    gol->cell_array[(gol->cell_nv * gol->cell_nh) - 1] = CELL_ALIVE;
}

static bool gol_cell_is_alive(struct gol* gol, const int col, const int line) {
    bool alive = false;
    int i = col + (line * gol->cell_nh);
    if ((gol->cell_array[i] & CELL_ALIVE) != 0) {
        alive = true;
    }
    return alive;
}

#if 0
static int __gol_neighbors_get(int i) {
    int line = i / _g.gol.cell_nh;
    int col = (i % _g.gol.cell_nh);

    int ngi = 0;
    if (col > 0) { ng[ngi++] = i - 1; }
    if (col > 0) { ng[ngi++] = i - 1; }

}

static __gol_solve() {
    // game of life rules https://en.wikipedia.org/wiki/Conway's_Game_of_Life#Rules

    for (int i = 0; i < max; i++) {
        int line = i / _g.gol.cell_nh;
        int col = (i % _g.gol.cell_nh);

        int neighbors_alive = 0;
        // 1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
        if ((_g.gol.cells[i] & CELL_ALIVE) != 0) {
    //
    // 2. Any live cell with two or three live neighbours lives on to the next generation.
    // 3. Any live cell with more than three live neighbours dies, as if by overpopulation.
    // 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
#endif


void slock_raylib_init(void) {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(0, 0, "screen-lock");

    int display = GetCurrentMonitor();
    _g.display.width = GetMonitorWidth(display);
    _g.display.height = GetMonitorHeight(display);


    //SetExitKey(KEY_NULL); // disables ESC key from closing the window

    DisableCursor(); // Limit cursor to relative movement inside the window

    SetTargetFPS(60);

#if 0
    if (!IsWindowFullscreen()) {

        // ToggleFullscreen does not scale the window to fit the monitor, it changes the monitor resolution to match the window.
        ToggleFullscreen();
    }
#endif
    memset(&_passwd, 0, sizeof(_passwd));

    _g.grid.size = 30;
    _g.grid.nh = _g.display.width / _g.grid.size;
    _g.grid.nv = _g.display.height / _g.grid.size;
    gol_create(&_g.gol, _g.grid.nh, _g.grid.nv);
}

void slock_raylib_run(void) {
    // sets camera mid screen
    Camera2D cam = { 0 };
    cam.target = (Vector2){ _g.display.width/2.0f, _g.display.height/2.0f };
    cam.offset = cam.target;
    cam.rotation = 0.0f;
    cam.zoom = 1.0f;

    while (!WindowShouldClose()) {   // Detect window close button or ESC key
        // camera movements
        cam.zoom += ((float)GetMouseWheelMove()*0.05f);
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            Vector2 mouse_delta = GetMouseDelta();
            cam.offset.x += mouse_delta.x;
            cam.offset.y += mouse_delta.y;
        }

        //__gol_solve(_g.gol.cells, _g.gol.cell_nh, _g.gol.cell_nv);

        BeginDrawing();

        BeginMode2D(cam);

        ClearBackground(RAYWHITE);

        // draw gol
        for (int i = 0; i < _g.gol.cell_nv + 1; i++) {
            DrawLineV((Vector2){0, _g.grid.size * i}, (Vector2){_g.display.width, _g.grid.size * i}, LIGHTGRAY);
        }
        for (int i = 0; i < _g.gol.cell_nh + 1; i++) {
            DrawLineV((Vector2){_g.grid.size * i, 0}, (Vector2){_g.grid.size * i, _g.display.height}, LIGHTGRAY);
        }

        // draw life
        for (int i = 0; i < (_g.gol.cell_nv * _g.gol.cell_nh); i++) {
            int line = i / _g.gol.cell_nh;
            int col = (i % _g.gol.cell_nh);
            if (gol_cell_is_alive(&_g.gol, col, line)) {
                Rectangle life = { _g.grid.size * col, _g.grid.size * line, _g.grid.size, _g.grid.size };
                DrawRectangleRec(life, RED);
            }
        }

        int key = GetKeyPressed();
        if (key == KEY_ENTER) {
            break;
        }

        DrawText(TextFormat("Key pressed: %i", key), 110, 110, 20, BLACK);

        EndMode2D();
    
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
}
