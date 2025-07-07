#pragma once

#include "calculate.h"
#include "raylib.h"
#include <math.h>

typedef struct {
  float h;
  float s;
  float v;
} HSV;

HSV rgb_to_hsv(Color c);
Color hsv_to_rgb(HSV hsv);
Color interpolate_color_hsv(Color start, Color end, float t);
