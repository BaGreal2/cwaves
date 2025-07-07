#include "physics.h"
#include "defines.h"
#include <math.h>

void progress_wave(float grid_prev[GRID_WIDTH][GRID_HEIGHT],
                   float grid[GRID_WIDTH][GRID_HEIGHT]) {
  float grid_next[GRID_WIDTH][GRID_HEIGHT] = {0};

  for (int y = 1; y < GRID_HEIGHT - 1; ++y) {
    for (int x = 1; x < GRID_WIDTH - 1; ++x) {
      grid_next[x][y] = 2.f * grid[x][y] - grid_prev[x][y] +
                        R * (grid[x + 1][y] + grid[x - 1][y] + grid[x][y + 1] +
                             grid[x][y - 1] - 4.f * grid[x][y]);
      grid_next[x][y] *= DAMPING;
    }
  }

  for (int y = 0; y < GRID_HEIGHT; ++y) {
    for (int x = 0; x < GRID_WIDTH; ++x) {
      grid_prev[x][y] = grid[x][y];
    }
  }

  for (int y = 0; y < GRID_HEIGHT; ++y) {
    for (int x = 0; x < GRID_WIDTH; ++x) {
      grid[x][y] = grid_next[x][y];
    }
  }
}
