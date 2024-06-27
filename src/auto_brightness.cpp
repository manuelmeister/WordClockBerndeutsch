#include <Arduino.h>
#include "auto_brightness.h"

const byte PROGMEM gamma8[] = {1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 9, 9, 10, 10,
                              11, 11, 12, 12, 13, 13, 14, 15, 15, 16, 16, 17, 18, 18, 19, 19, 20, 21, 21, 22, 23, 23,
                              24, 25, 25, 26, 27, 27, 28, 29, 30, 30, 31, 32, 33, 33, 34, 35, 36, 36, 37, 38, 39, 40,
                              40, 41, 42, 43, 44, 44, 45, 46, 47, 48, 49, 50, 50, 51, 52, 53, 54, 55, 56, 57, 58, 58,
                              59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
                              81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 97, 98, 99, 100, 101, 102,
                              103, 104, 105, 106, 108, 109, 110, 111, 112, 113, 114, 116, 117, 118, 119, 120, 121, 123,
                              124, 125, 126, 127, 128, 130, 131, 132, 133, 134, 136, 137, 138, 139, 141, 142, 143, 144,
                              145, 147, 148, 149, 150, 152, 153, 154, 156, 157, 158, 159, 161, 162, 163, 165, 166, 167,
                              168, 170, 171, 172, 174, 175, 176, 178, 179, 180, 182, 183, 184, 186, 187, 188, 190, 191,
                              192, 194, 195, 197, 198, 199, 201, 202, 203, 205, 206, 208, 209, 210, 212, 213, 215, 216,
                              218, 219, 220, 222, 223, 225, 226, 228, 229, 230, 232, 233, 235, 236, 238, 239, 241, 242,
                              244, 245, 247, 248, 250, 251, 253, 254, 255};

int AutoBrightness::refresh(int light_sensor) {
  this->light_values[this->light_index] = light_sensor;
  this->light_index = (this->light_index + 1) % 30;
  this->indexed_light = min(30, this->indexed_light + 1);

  return scaled_brightness();
}

float AutoBrightness::averarge_light() {
  int brightness_sum = 0;
  for (int i = 0; i < this->indexed_light; i++) {
    brightness_sum += this->light_values[i];
  }

  return float(brightness_sum) / float(this->indexed_light);
}

float clamp(float x, float a, float b) {
  return max(a, min(b, x));
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  x = clamp(x, in_min, in_max);
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int AutoBrightness::scaled_brightness() {
  float brightness_frac = clamp(averarge_light(), 0.0f, 1024.0f);
  int resbright = int(mapf(brightness_frac, 0.0f, 1024.0f, 0.0f, 255.0f));
  int brightness_scaled = max(1, min(255, 255 - int(pgm_read_byte_inlined(&gamma8[255 - resbright]))));

  return brightness_scaled;
}

