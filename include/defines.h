#pragma once

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TARGET_FPS 60

#define CELL_SIZE 4
#define POINTER_SIZE 40
#define GRID_WIDTH WINDOW_WIDTH / CELL_SIZE
#define GRID_HEIGHT WINDOW_HEIGHT / CELL_SIZE
#define VISUAL_CLAMP 0.5f

#define C_PIXELS 200.0f
#define SIGMA 0.5f

#define DX ((float)CELL_SIZE)
#define DT (SIGMA * DX / C_PIXELS)
#define R (SIGMA * SIGMA)
#define DAMPING_PER_SECOND 0.2f
#define DAMPING powf(DAMPING_PER_SECOND, DT)

#define BACKGROUND (Color){0x18, 0x18, 0x18, 255}
#define FOREGROUND (Color){0xf5, 0xf5, 0xf5, 255}
#define DEEP_WATER (Color){0x0a, 0x1e, 0x3c, 255}
#define BRIGHT_WATER (Color){0x19, 0x3c, 0x6e, 255}
