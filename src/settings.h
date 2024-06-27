#ifndef WORDCLOCKBERNDEUTSCH_SETTINGS_H
#define WORDCLOCKBERNDEUTSCH_SETTINGS_H

//////////////////////////////////////
///   Settings for the wordclock   ///
//////////////////////////////////////

#define WORDCLOCK_NEOPIXEL_PIN D7

// Comment out the next line if you don't want to use the brightness sensor
#define WORDCLOCK_BRIGHTNESS_SENSOR

#ifndef WORDCLOCK_BRIGHTNESS_SENSOR
#define PIXEL_BRIGHTNESS 100
#endif

// How many loop cycles to wait before updating the display/brightness
#define WORDCLOCK_REFRESH_RATE 10

// Uncomment next line to serial debug to print time as well as temperature to serial
// #define WORDCLOCK_SERIAL_DEBUG

#endif //WORDCLOCKBERNDEUTSCH_SETTINGS_H
