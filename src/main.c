#include "defines.h"
#include "physics.h"
#include "raylib.h"
#include "render.h"

int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Waves");
  SetTargetFPS(TARGET_FPS);

  float grid_prev[GRID_WIDTH][GRID_HEIGHT] = {0};
  float grid[GRID_WIDTH][GRID_HEIGHT] = {0};

  float accumulator = 0.0f;
  while (!WindowShouldClose()) {
    const float frameTime = GetFrameTime();
    accumulator += frameTime;

    while (accumulator >= DT) {
      progress_wave(grid_prev, grid);
      accumulator -= DT;
    }

    BeginDrawing();
    ClearBackground(BACKGROUND);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      Vector2 mousePosition = GetMousePosition();
      int grid_x = mousePosition.x / CELL_SIZE;
      int grid_y = mousePosition.y / CELL_SIZE;

      if (grid_x > 0 && grid_x < GRID_WIDTH - 1 && grid_y > 0 &&
          grid_y < GRID_HEIGHT - 1) {
        place_circle(grid_prev, grid, grid_x, grid_y, POINTER_SIZE / CELL_SIZE,
                     1.5f);
      }
    }

    render_grid(grid);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
