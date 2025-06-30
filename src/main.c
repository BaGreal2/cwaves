#include "raylib.h"
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TARGET_FPS 60

#define CELL_SIZE 10
#define GRID_WIDTH WINDOW_WIDTH / CELL_SIZE
#define GRID_HEIGHT WINDOW_HEIGHT / CELL_SIZE

#define BACKGROUND (Color){0x18, 0x18, 0x18, 255}
#define FOREGROUND (Color){0xf5, 0xf5, 0xf5, 255}
#define LINES_COLOR (Color){0x55, 0x55, 0x55, 255}

void progress_sand(int grid[GRID_WIDTH][GRID_HEIGHT]) {
  int new_grid[GRID_WIDTH][GRID_HEIGHT] = {0};
  for (int y = 0; y < GRID_HEIGHT; ++y) {
    for (int x = 0; x < GRID_WIDTH; ++x) {
      int cell = grid[x][y];
      if (cell) {
        if (y + 1 < GRID_HEIGHT) {
          if (y + 1 < GRID_HEIGHT - 1 && x + 1 < GRID_WIDTH - 1 &&
              grid[x][y + 1] && grid[x][y + 2] && !grid[x + 1][y + 1] &&
              !grid[x - 1][y + 1]) {

            int go_right = GetRandomValue(0, 1);
            new_grid[x][y] = 0;
            if (go_right) {
              new_grid[x + 1][y + 1] = 1;
            } else {
              new_grid[x - 1][y + 1] = 1;
            }
          } else if (y + 1 < GRID_HEIGHT - 1 && x + 1 < GRID_WIDTH - 1 &&
                     grid[x][y + 1] && grid[x][y + 2] && !grid[x + 1][y + 1]) {
            new_grid[x][y] = 0;
            new_grid[x + 1][y + 1] = 1;
          } else if (y + 1 < GRID_HEIGHT - 1 && x + 1 < GRID_WIDTH - 1 &&
                     grid[x][y + 1] && grid[x][y + 2] && !grid[x - 1][y + 1]) {
            new_grid[x][y] = 0;
            new_grid[x - 1][y + 1] = 1;
          } else if (grid[x][y + 1]) {
            new_grid[x][y] = 1;
          } else {
            new_grid[x][y] = 0;
            new_grid[x][y + 1] = 1;
          }
        } else {
          new_grid[x][y] = 1;
        }
        if (x + 1 == GRID_WIDTH || x == 0) {
          new_grid[x][y] = 1;
        }
      }
    }
  }

  for (int y = 0; y < GRID_HEIGHT; ++y) {
    for (int x = 0; x < GRID_WIDTH; ++x) {
      grid[x][y] = new_grid[x][y];
    }
  }
}

int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Waves");
  SetTargetFPS(TARGET_FPS);

  int grid[GRID_WIDTH][GRID_HEIGHT] = {0};

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BACKGROUND);

    progress_sand(grid);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      Vector2 mousePosition = GetMousePosition();
      int gridX = mousePosition.x / CELL_SIZE;
      int gridY = mousePosition.y / CELL_SIZE;

      if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 &&
          gridY < GRID_HEIGHT) {
        grid[gridX][gridY] = 1;
      }
    }

    for (int y = 0; y < WINDOW_HEIGHT; y += CELL_SIZE) {
      DrawLine(0, y, WINDOW_WIDTH, y, LINES_COLOR);
    }
    for (int x = 0; x < WINDOW_WIDTH; x += CELL_SIZE) {
      DrawLine(x, 0, x, WINDOW_HEIGHT, LINES_COLOR);
    }
    for (int x = 0; x < WINDOW_WIDTH / CELL_SIZE; x++) {
      for (int y = 0; y < WINDOW_HEIGHT / CELL_SIZE; y++) {
        if (grid[x][y] == 1) {
          DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE,
                        FOREGROUND);
        }
      }
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
