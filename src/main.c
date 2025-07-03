#include "raylib.h"
#include <math.h>

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

float clamp(float value, float min, float max) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

typedef struct {
  float h;
  float s;
  float v;
} HSV;

HSV rgb_to_hsv(Color c) {
  float r = c.r / 255.0f;
  float g = c.g / 255.0f;
  float b = c.b / 255.0f;

  float max = fmaxf(r, fmaxf(g, b));
  float min = fminf(r, fminf(g, b));
  float delta = max - min;

  HSV hsv = {0};

  if (delta > 0.00001f) {
    if (max == r) {
      hsv.h = 60.0f * fmodf(((g - b) / delta), 6.0f);
    } else if (max == g) {
      hsv.h = 60.0f * (((b - r) / delta) + 2.0f);
    } else if (max == b) {
      hsv.h = 60.0f * (((r - g) / delta) + 4.0f);
    }
  } else {
    hsv.h = 0.0f;
  }

  if (hsv.h < 0.0f)
    hsv.h += 360.0f;

  hsv.s = (max <= 0.0f) ? 0.0f : (delta / max);

  hsv.v = max;

  return hsv;
}

Color hsv_to_rgb(HSV hsv) {
  float c = hsv.v * hsv.s;
  float x = c * (1.0f - fabsf(fmodf(hsv.h / 60.0f, 2.0f) - 1.0f));
  float m = hsv.v - c;

  float r, g, b;

  if (hsv.h < 60) {
    r = c;
    g = x;
    b = 0;
  } else if (hsv.h < 120) {
    r = x;
    g = c;
    b = 0;
  } else if (hsv.h < 180) {
    r = 0;
    g = c;
    b = x;
  } else if (hsv.h < 240) {
    r = 0;
    g = x;
    b = c;
  } else if (hsv.h < 300) {
    r = x;
    g = 0;
    b = c;
  } else {
    r = c;
    g = 0;
    b = x;
  }

  return (Color){.r = (unsigned char)clamp((r + m) * 255.0f, 0, 255),
                 .g = (unsigned char)clamp((g + m) * 255.0f, 0, 255),
                 .b = (unsigned char)clamp((b + m) * 255.0f, 0, 255),
                 .a = 255};
}

Color interpolate_color_hsv(Color start, Color end, float t) {
  HSV a = rgb_to_hsv(start);
  HSV b = rgb_to_hsv(end);

  float h_diff = fmodf(b.h - a.h + 360.0f, 360.0f);
  if (h_diff > 180.0f)
    h_diff -= 360.0f;

  HSV interpolated = {
      .h = fmodf(a.h + t * h_diff + 360.0f, 360.0f),
      .s = a.s + t * (b.s - a.s),
      .v = a.v + t * (b.v - a.v),
  };
  return hsv_to_rgb(interpolated);
}

float ease_water(float t) { return t * t * (3 - 2 * t); }

float normalize(float value, float old_min, float old_max, float new_min,
                float new_max) {
  return (value - old_min) * (new_max - new_min) / (old_max - old_min) +
         new_min;
}

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

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
