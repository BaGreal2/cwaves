#include "render.h"
#include "calculate.h"
#include "colors.h"
#include "raylib.h"

void place_circle(float grid_prev[GRID_WIDTH][GRID_HEIGHT],
                  float grid[GRID_WIDTH][GRID_HEIGHT], int x, int y, int radius,
                  float force) {
  for (int i = -radius; i <= radius; i++) {
    for (int j = -radius; j <= radius; j++) {
      if (i * i + j * j <= radius * radius) {
        int gridX = x + i;
        int gridY = y + j;
        if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 &&
            gridY < GRID_HEIGHT) {
          grid_prev[gridX][gridY] = force;
          grid[gridX][gridY] = force;
        }
      }
    }
  }
}

void render_grid(float grid[GRID_WIDTH][GRID_HEIGHT]) {
  for (int x = 0; x < WINDOW_WIDTH / CELL_SIZE; x++) {
    for (int y = 0; y < WINDOW_HEIGHT / CELL_SIZE; y++) {
      float d = grid[x][y];
      float norm = normalize(d, -VISUAL_CLAMP, VISUAL_CLAMP, 0, 1);
      float brightness = ease_water(norm);
      Color base = interpolate_color_hsv(DEEP_WATER, BRIGHT_WATER,
                                         clamp(brightness, 0, 1));
      HSV base_hsv = rgb_to_hsv(base);

      base_hsv.s = clamp(base_hsv.s + brightness * 0.3f, 0, 1);
      base_hsv.v = clamp(base_hsv.v + brightness * 0.2f, 0, 1);

      base = hsv_to_rgb(base_hsv);

      base.r = clamp(base.r + brightness * 5, 0, 170);
      base.g = clamp(base.g + brightness * 10, 0, 170);
      base.b = clamp(base.b + brightness * 50, 80, 200);

      DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, base);
    }
  }
}
