#pragma once

#include "defines.h"

void place_circle(float grid_prev[GRID_WIDTH][GRID_HEIGHT],
                  float grid[GRID_WIDTH][GRID_HEIGHT], int x, int y, int radius,
                  float force);

void render_grid(float grid[GRID_WIDTH][GRID_HEIGHT]);
