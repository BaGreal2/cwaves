#pragma once

float clamp(float value, float min, float max);
float normalize(float value, float old_min, float old_max, float new_min,
                float new_max);
float ease_water(float t);
