#include "calculate.h"

float clamp(float value, float min, float max) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

float normalize(float value, float old_min, float old_max, float new_min,
                float new_max) {
  return (value - old_min) * (new_max - new_min) / (old_max - old_min) +
         new_min;
}

float ease_water(float t) { return t * t * (3 - 2 * t); }
