#include <Arduino.h>
#include "auto_brightness.h"

const int PROGMEM gamma8[] = {1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 9, 9, 10, 10,
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

int AutoBrightness::refresh() {
  int light_sensor = analogRead(A0);

  this->light_vector.push_back(light_sensor);
  if (this->light_vector.size() > 30) this->light_vector.erase(this->light_vector.begin());

  return scaled_brightness();
}

double AutoBrightness::averarge_light() {
  double brightness_sum = 0.0;
  for (int brightness: this->light_vector) {
    brightness_sum += brightness;
  }

  return brightness_sum / this->light_vector.size();
}


int AutoBrightness::scaled_brightness() {
  double brightness_frac = max(0.0,min(1.0, averarge_light() / 1024.0));
  double resbright = 255.0 * brightness_frac;
  int brightness_scaled = max(1, min(80, int(map(256 - int(pgm_read_byte_inlined(&gamma8[255 - int(resbright)])),1,125,1,255))));

  return int(brightness_scaled);
}

