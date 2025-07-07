#include "colors.h"

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
