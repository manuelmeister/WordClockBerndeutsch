#include <Arduino.h>
#include <string>
#include "settings.h"
#include "layout.h"

/*
  E	  S 	F 	I 	S 	C 	H 	O 	Ö	  P	  P	  E
  G	  L 	Y 	N 	E 	I 	V 	I 	E	  R	  T	  U
  E	  I 	S 	Ä 	C 	H 	S 	I 	B	  E	  U	  F
  Z	  W 	Ö 	U 	F 	Ü 	F 	L 	A	  C	  H	  T
  🐈  M 	E 	I 	S 	T 	E 	R 	.I  O	  / 	Z
  D	  R 	Ü 	N 	E 	Z 	W 	Ä 	N	  Z	  G	  Y
  N	  Ü 	N 	Z 	Ä 	J 	A 	B 	V	  O	  R	  T
  H	  A 	U 	B 	I 	Q 	Z 	W 	Ö	  U	  F	  I
  V	  I 	E 	R 	I 	K 	F 	Ü 	F	  I	  X	  📶
  🐘  E 	I 	S 	Ä 	C 	H 	S 	I	  B	  N	  I
  D	  R 	Ü 	E 	U 	F 	I 	A 	C	  H	  T	  I
  N	  Ü 	N 	I 	Z 	Ä 	N 	I 	D	  G	  S	  Y
*/

// String to test output in Serial
static std::string wordstring = "ESFISCHOoPPEUTREIVIENYLGEISaCHSIBEUFTHCALFuFUoWZ2MEISTERIO/ZYGZNaWZENuRDNuNZaJABVORTIFUoWZQIBUAHVIERIKFuFIX0INBISHCaSIE1DRuEUFIACHTIYSGDINaZINuN";

// Which version of the time-display-methods should be used
int exact = 1;

// Don't light up WiFi, only on requests
uint32_t wifiColor = Black;

// How long this state should be displayed
int wifiWait = 0;

int maxPowerdraw = 1200;

int mapToPowerdraw(uint32_t color) {
  long r = map(long((color >> 16) & 0xFF), 0, 255, 0, 20);
  long g = map(long((color >> 8) & 0xFF), 0, 255, 0, 20);
  long b = map(long(color & 0xFF), 0, 255, 0, 20);
  return int(r+g+b);
}

void WordLayout::chase(uint32_t color) {
  for (uint16_t i = 0; i < this->pixels.numPixels() + 4; i++) {
    this->pixels.setPixelColor(i, color); // Draw new pixel
    this->pixels.setPixelColor(i - 4, this->backgroundColor); // Erase pixel a few steps back
    this->pixels.show();
    delay(10);
  }
}

void WordLayout::showLogo(uint32_t color) {
  for (auto &j: WordURL) {
    if (j == WordFix) {
      delay(450);
    }
    for (int i = 0; i < this->pixels.numPixels(); i++) {
      if (j[i] == -1) {
        break;
      } else {
        if (j[i] == SymbolWifi[0]) {
          this->pixels.setPixelColor(j[i], Blue); // Erase pixel a few steps back
        } else {
          this->pixels.setPixelColor(j[i], White); // Erase pixel a few steps back
        }
      }
      this->pixels.show();
      delay(25);
    }
    this->pixels.show();
    delay(250);
  }
}

void WordLayout::blank(bool skip) {
  #ifdef WORDCLOCK_BRIGHTNESS_SENSOR
    this->powerdraw = 0;
  #endif
  for (int x = 0; x < this->pixels.numPixels(); ++x) {
    #ifdef WORDCLOCK_BRIGHTNESS_SENSOR
      this->powerdraw += mapToPowerdraw(this->backgroundColor);
    #endif
    if (!skip) {
      this->pixels.setPixelColor(x, this->backgroundColor);
    }
  }
}

void WordLayout::wipe() {
  blank();
  this->pixels.show();
}

void WordLayout::showWifiSuccess() {
  lightup(WordFix, this->backgroundColor);
  lightup(SymbolWifi, Green);
  this->pixels.show();
}

void WordLayout::connectWLAN() {
  lightup(WordFix, White);
  lightup(SymbolWifi, Blue);
  this->pixels.show();
}

/**
 * Sets the pixels for the hour
 */
void WordLayout::showHour(bool skip) {
  if (minute < 25) {
    // show this hour if we are before 25 minutes past
    lightup(WordStunden[hour % 12], this->foregroundColor, skip);
  } else {
    // show next hour
    lightup(WordStunden[(hour % 12) + 1], this->foregroundColor, skip);
  }
}

/**
 * Sets the pixels for the hour
 */
void WordLayout::showHourSemiExact(bool skip) {
  if (minute < 23) {
    // show this hour if we are before 23 minutes past
    lightup(WordStunden[hour % 12], this->foregroundColor, skip);
  } else {
    // show next hour
    lightup(WordStunden[(hour % 12) + 1], this->foregroundColor, skip);
  }
}

/**
 * Sets pixels for the current minutes values
 */
void WordLayout::showMinuteExact(bool skip) {
  if (minute != 0) {
    if (minute < 30) {
      lightup(WordMinutenExact[minute - 1], this->foregroundColor, skip);
    } else if (minute > 30) {
      lightup(WordMinutenExact[29 - (minute % 30)], this->foregroundColor, skip);
    }
    if ((minute > 0 && minute < 25) || (minute < 40 && minute > 30)) {
      lightup(WordNach, this->foregroundColor, skip);
    }
    if (minute >= 40 || (minute >= 25 && minute < 30)) {
      lightup(WordVor, this->foregroundColor, skip);
    }
    if (minute >= 25 && minute <= 35) {
      lightup(WordHalb, this->foregroundColor, skip);
    }
  }
}

/**
 * Sets pixels for the current minutes values
 */
void WordLayout::showMinuteSemiExact(bool skip) {
  if (minute != 0) {
    for (int i = 0; i < 5; ++i) {
      if (WordMinutenSemiExact[minute - 1][i] == WordMinLeer) {
        break;
      } else {
        lightup(WordMinutenSemiExact[minute - 1][i], this->foregroundColor, skip);
      }
    }
    if ((minute > 0 && minute < 23) || (minute <= 37 && minute > 30)) {
      lightup(WordNach, this->foregroundColor, skip);
    }
    if (minute > 37 || (minute >= 23 && minute < 30)) {
      lightup(WordVor, this->foregroundColor, skip);
    }
    if (minute >= 23 && minute <= 37) {
      lightup(WordHalb, this->foregroundColor, skip);
    }
  }
}

/**
 * Sets pixels for the current minutes values
 */
void WordLayout::showMinuteLocatorSemiExact(bool skip) {
  if ((minute > 15 && minute < 18) ||
      (minute > 20 && minute < 23) ||
      (minute > 35 && minute < 38) ||
      (minute > 40 && minute < 43) ||
      (minute > 45 && minute < 48)) {
    lightup(WordGewesen, this->foregroundColor, skip);
  }
}

void WordLayout::update() {
  #ifdef WORDCLOCK_BRIGHTNESS_SENSOR
    this->powerdraw = 0;
    this->setPixels(true);

    int light_sensor = analogRead(A0);
    byte brightness = byte(this->autoBrightness.refresh(light_sensor) * 1.4);
    byte maxed_brightness = byte(float(brightness) / (max(float(maxPowerdraw), float(this->powerdraw)) / float(maxPowerdraw)));
    #ifdef WORDCLOCK_SERIAL_DEBUG
    Serial.print("| LIGHT: ");
    Serial.print(light_sensor);
    Serial.print(" | POWER: ");
    Serial.print(this->powerdraw);
    Serial.print(" | AVG: ");
    Serial.print(brightness);
    Serial.print(" | LUM: ");
    Serial.print(maxed_brightness);
    #endif
    this->setPixels();
    this->pixels.setBrightness(maxed_brightness);
  #endif
  #ifndef WORDCLOCK_BRIGHTNESS_SENSOR
    this->pixels.setBrightness(PIXEL_BRIGHTNESS);
  #endif
  this->pixels.show();
}

void WordLayout::setPixels(bool skip) {
  blank(skip);

  // light up "it's" it stays on
  lightup(WordEs, this->foregroundColor, skip);
  lightup(WordIst, this->foregroundColor, skip);

  // Light up minutes
  if (exact == 1) {
    showMinuteSemiExact(skip);
  } else if (exact == 2) {
    showMinuteExact(skip);
  }

  // light up hours
  if (exact != 1) {
    showHour(skip);
  } else {
    showHourSemiExact(skip);
  }

  if (hour % 12 >= 6 && hour % 12 < 9) {
    lightup(SymbolElephant, MediumBlue, skip);
  } else if (hour > 20 || hour < 6) {
    lightup(SymbolCat, MediumBlue, skip);
  }

  if (exact == 1) {
    showMinuteLocatorSemiExact(skip);
  }
}

void WordLayout::setupDisplay() {
  pixels.begin();
  backgroundColor = Black;
  //foregroundColor = DarkerWhite;
  //foregroundColor = BrightWhite;
  foregroundColor = BrightWhite;
  wipe();
}

void WordLayout::lightup(int *Word, uint32_t Color, bool skip) {
  for (int x = 0; x < pixels.numPixels() + 1; x++) {
    #ifdef WORDCLOCK_BRIGHTNESS_SENSOR
    this->powerdraw += mapToPowerdraw(Color);
    #endif
    if (Word[x] == -1) {
      #ifdef WORDCLOCK_SERIAL_DEBUG
      Serial.print(" ");
      #endif
      break;
    } else {
      #ifdef WORDCLOCK_SERIAL_DEBUG
      Serial.print(wordstring.at(Word[x]));
      #endif
      if(!skip) {
        this->pixels.setPixelColor(Word[x], Color);
      }
    }
  }
}

/**
 * Shows the status of the WiFI connection
 * @param Color
 * @param duration
 */
void WordLayout::setWifiStatus(uint32_t Color, int duration) {
  wifiWait = duration;
  wifiColor = Color;
  lightup(SymbolWifi, wifiColor);
  this->pixels.show();
}