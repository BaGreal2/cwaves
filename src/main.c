#include "defines.h"
// #include "physics.h"
#include "raylib.h"
// #include "render.h"

#define GLSL_VERSION 330

int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Waves");
  SetTargetFPS(TARGET_FPS);

  Image imBlank = GenImageColor(1024, 1024, BLANK);
  Texture2D texture = LoadTextureFromImage(imBlank);
  UnloadImage(imBlank);

  Shader shader = LoadShader(0, TextFormat("shaders/ex.fs", GLSL_VERSION));

  float time = 0.0f;
  int timeLoc = GetShaderLocation(shader, "uTime");
  SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);

  float grid_prev[GRID_WIDTH][GRID_HEIGHT] = {0};
  float grid[GRID_WIDTH][GRID_HEIGHT] = {0};

  float accumulator = 0.0f;
  while (!WindowShouldClose()) {
    time = (float)GetTime();
    SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);

    // const float frameTime = GetFrameTime();
    // accumulator += frameTime;
    //
    // while (accumulator >= DT) {
    //   progress_wave(grid_prev, grid);
    //   accumulator -= DT;
    // }

    BeginDrawing();
    ClearBackground(BACKGROUND);

    BeginShaderMode(shader);
    DrawTexture(texture, 0, 0, WHITE);
    EndShaderMode();

    DrawText("BACKGROUND is PAINTED and ANIMATED on SHADER!", 10, 10, 20,
             MAROON);

    // if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    //   Vector2 mousePosition = GetMousePosition();
    //   int grid_x = mousePosition.x / CELL_SIZE;
    //   int grid_y = mousePosition.y / CELL_SIZE;
    //
    //   if (grid_x > 0 && grid_x < GRID_WIDTH - 1 && grid_y > 0 &&
    //       grid_y < GRID_HEIGHT - 1) {
    //     place_circle(grid_prev, grid, grid_x, grid_y, POINTER_SIZE /
    //     CELL_SIZE,
    //                  1.5f);
    //   }
    // }

    // render_grid(grid);

    EndDrawing();
  }

  UnloadShader(shader);
  UnloadTexture(texture);

  CloseWindow();
  return 0;
}
