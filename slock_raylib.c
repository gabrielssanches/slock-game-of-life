#include "slock_raylib.h"

#include "raylib.h"
#include <string.h>
#include <stdint.h>

struct gol {
    int height;
    int width;
    int ncell_h;
    int ncell_v;
    int cell_size;
    unsigned int* cells;
};

struct gamectx {
    struct gol gol;
    struct {
        int width;
        int height;
    } display;
};
static struct gamectx _g;

static char _passwd[32];

#define CELL_ALIVE  (1 << 0)
#define CELL_BORN   (1 << 1)
#define CELL_KILL   (1 << 2)
#define CELL_GET_AGE(x)  (((x) & 0xFFFF0000) >> 4)

static unsigned int* __gol_create(struconst int n_horizontal, const int n_vertical) {
    _g.gol.cell_size = 30;
    _g.gol.width = ((0 + (_g.display.width / _g.gol.cell_size)) * _g.gol.cell_size);
    _g.gol.height = ((0 + (_g.display.height / _g.gol.cell_size)) * _g.gol.cell_size);
    _g.gol.ncell_v = _g.gol.height / _g.gol.cell_size;
    _g.gol.ncell_h = _g.gol.width / _g.gol.cell_size;
    unsigned int* cells = malloc(sizeof(unsigned int) * n_horizontal * n_vertical);
    memset(cells, 0, sizeof(unsigned int) * n_horizontal * n_vertical);
    return cells;
}

#if 0
static int __gol_neighbors_get(int i) {
    int line = i / _g.gol.ncell_h;
    int col = (i % _g.gol.ncell_h);

    int ngi = 0;
    if (col > 0) { ng[ngi++] = i - 1; }
    if (col > 0) { ng[ngi++] = i - 1; }

}

static __gol_solve() {
    // game of life rules https://en.wikipedia.org/wiki/Conway's_Game_of_Life#Rules

    for (int i = 0; i < max; i++) {
        int line = i / _g.gol.ncell_h;
        int col = (i % _g.gol.ncell_h);

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

    __gol_create(&_g.gol, _g.display.width, _g.display.height, 30);

    _g.gol.cells[0] = CELL_ALIVE;
    _g.gol.cells[_g.gol.ncell_h -1] = CELL_ALIVE;
    _g.gol.cells[((_g.gol.ncell_v -1) * _g.gol.ncell_h)] = CELL_ALIVE;
    _g.gol.cells[(_g.gol.ncell_v * _g.gol.ncell_h) - 1] = CELL_ALIVE;
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

        //__gol_solve(_g.gol.cells, _g.gol.ncell_h, _g.gol.ncell_v);

        BeginDrawing();

        BeginMode2D(cam);

        ClearBackground(RAYWHITE);

        // draw gol
        for (int i = 0; i < _g.gol.ncell_v + 1; i++) {
            DrawLineV((Vector2){0, _g.gol.cell_size * i}, (Vector2){_g.gol.width, _g.gol.cell_size * i}, LIGHTGRAY);
        }
        for (int i = 0; i < _g.gol.ncell_h + 1; i++) {
            DrawLineV((Vector2){_g.gol.cell_size * i, 0}, (Vector2){_g.gol.cell_size * i, _g.gol.height}, LIGHTGRAY);
        }

        // draw life
        for (int i = 0; i < (_g.gol.ncell_v * _g.gol.ncell_h); i++) {
            int line = i / _g.gol.ncell_h;
            int col = (i % _g.gol.ncell_h);
            if ((_g.gol.cells[i] & CELL_ALIVE) != 0) {
                Rectangle life = { _g.gol.cell_size * col, _g.gol.cell_size * line, _g.gol.cell_size, _g.gol.cell_size };
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
