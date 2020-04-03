#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "layout.h"

/*
  E	  S 	F 	I 	S 	C 	H 	O 	Ö	  P	  P	  E
  G	  L 	Y 	N 	E 	I 	V 	I 	E	  R	  T	  U
  E	  I 	S 	Ä 	C 	H 	S 	I 	B	  E	  U	  F
  Z	  W 	Ö 	U 	F 	Ü 	F 	L 	A	  C	  H	  T
  -	  M 	E 	I 	S 	T 	E 	R 	.I  O	  / 	Z
  D	  R 	Ü 	N 	E 	Z 	W 	Ä 	N	  Z	  G	  Y
  N	  Ü 	N 	Z 	Ä 	J 	A 	B 	V	  O	  R	  T
  H	  A 	U 	B 	I 	Q 	Z 	W 	Ö	  U	  F	  I
  V	  I 	E 	R 	I 	K 	F 	Ü 	F	  I	  X	  -
  -	  E 	I 	S 	Ä 	C 	H 	S 	I	  B	  N	  I
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


void WordLayout::chase(uint32_t color) {
  for (uint16_t i = 0; i < this->pixels.numPixels() + 4; i++) {
    this->pixels.setPixelColor(i, color); // Draw new pixel
    this->pixels.setPixelColor(i - 4, this->backgroundColor); // Erase pixel a few steps back
    this->pixels.show();
    delay(10);
  }
}

void WordLayout::showLogo(uint32_t color) {
  for (int j = 0; j < 6; j++) {
    if (WordURL[j] == WordFix) {
      delay(450);
    }
    for (int i = 0; i < this->pixels.numPixels(); i++) {
      if (WordURL[j][i] == -1) {
        break;
      } else {
        if(WordURL[j][i] == SymbolWifi[0]){
          this->pixels.setPixelColor(WordURL[j][i], Blue); // Erase pixel a few steps back
        } else {
          this->pixels.setPixelColor(WordURL[j][i], White); // Erase pixel a few steps back
        }
      }
      this->pixels.show();
      delay(25);
    }
    this->pixels.show();
    delay(250);
  }
}

void WordLayout::blank() {
  for (int x = 0; x < this->pixels.numPixels(); ++x) {
    this->pixels.setPixelColor(x, this->backgroundColor);
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
void WordLayout::showHour() {
  if (minute < 25) {
    // show this hour if we are before 25 minutes past
    lightup(WordStunden[hour % 12], this->foregroundColor);
  } else {
    // show next hour
    lightup(WordStunden[(hour % 12) + 1], this->foregroundColor);
  }
}

/**
 * Sets the pixels for the hour
 */
void WordLayout::showHourSemiExact() {
  if (minute < 23) {
    // show this hour if we are before 23 minutes past
    lightup(WordStunden[hour % 12], this->foregroundColor);
  } else {
    // show next hour
    lightup(WordStunden[(hour % 12) + 1], this->foregroundColor);
  }
}

/**
 * Sets pixels for the current minutes values
 */
void WordLayout::showMinute() {
  if (minute != 0) {
    if (minute >= 3 && minute < 30) {
      lightup(WordMinuten[round(minute / 5.0) - 1], this->foregroundColor);
    } else if (minute >= 35) {
      lightup(WordMinuten[5 - round((minute - 30) / 5.0)], this->foregroundColor);
    }
    if (std::fmod(minute / 5.0, 1.0) != 0.0) {
      lightup(WordUngefaehr, this->foregroundColor);
    }
    if ((minute >= 5 && minute < 25) || (minute < 40 && minute >= 35)) {
      lightup(WordNach, this->foregroundColor);
    }
    if (minute >= 40 || (minute >= 25 && minute < 30)) {
      lightup(WordVor, this->foregroundColor);
    }
    if (minute >= 25 && minute < 40) {
      lightup(WordHalb, this->foregroundColor);
    }
  }
}

/**
 * Sets pixels for the current minutes values
 */
void WordLayout::showMinuteExact() {
  if (minute != 0) {
    if (minute < 30) {
      lightup(WordMinutenExact[minute - 1], this->foregroundColor);
    } else if (minute > 30) {
      lightup(WordMinutenExact[29 - (minute % 30)], this->foregroundColor);
    }
    if ((minute > 0 && minute < 25) || (minute < 40 && minute > 30)) {
      lightup(WordNach, this->foregroundColor);
    }
    if (minute >= 40 || (minute >= 25 && minute < 30)) {
      lightup(WordVor, this->foregroundColor);
    }
    if (minute >= 25 && minute <= 35) {
      lightup(WordHalb, this->foregroundColor);
    }
  }
}

/**
 * Sets pixels for the current minutes values
 */
void WordLayout::showMinuteSemiExact() {
  if (minute != 0) {
    for (int i = 0; i < 5; ++i) {
      if (WordMinutenSemiExact[minute - 1][i] == WordMinLeer) {
        break;
      } else {
        lightup(WordMinutenSemiExact[minute - 1][i], this->foregroundColor);
      }
    }
    if ((minute > 0 && minute < 23) || (minute <= 37 && minute > 30)) {
      lightup(WordNach, this->foregroundColor);
    }
    if (minute > 37 || (minute >= 23 && minute < 30)) {
      lightup(WordVor, this->foregroundColor);
    }
    if (minute >= 23 && minute <= 37) {
      lightup(WordHalb, this->foregroundColor);
    }
  }
}

void WordLayout::displayTime() {
  blank();

  // light up "it's" it stays on
  lightup(WordEs, this->foregroundColor);
  lightup(WordIst, this->foregroundColor);

  // Light up minutes
  if (exact == 0) {
    showMinute();
  } else if (exact == 1) {
    showMinuteSemiExact();
  } else if (exact == 2) {
    showMinuteExact();
  }

  // light up hours
  if (exact != 1) {
    showHour();
  } else {
    showHourSemiExact();
  }

  if (hour % 12 >= 6 && hour % 12 < 9) {
    lightup(SymbolElephant, DarkestBlue);
  } else if (hour > 20 || hour < 6) {
    lightup(SymbolCat, DarkestBlue);
  }
  displayWifiStatus();

  pixels.show();
}

void WordLayout::setupDisplay() {
  pixels.begin();
  backgroundColor = Black;
  foregroundColor = DarkerWhite;
  wipe();
}


void WordLayout::lightup(int *Word, uint32_t Color) {
  for (int x = 0; x < pixels.numPixels() + 1; x++) {
    if (Word[x] == -1) {
      Serial.print(" ");
      break;
    } else {
      this->pixels.setPixelColor(Word[x], Color);
      Serial.print(wordstring.at(Word[x]));
    }
  }
}

/**
 * Zeigt den Status des
 * @param Color
 * @param duration
 */
void WordLayout::setWifiStatus(uint32_t Color, int duration) {
  wifiWait = duration;
  wifiColor = Color;
  lightup(SymbolWifi, wifiColor);
  this->pixels.show();
}

void WordLayout::displayWifiStatus() {
  if(wifiWait > 0){
    wifiWait--;
  } else if(wifiWait == 0) {
    wifiColor = Black;
    wifiWait = -1;
    lightup(SymbolWifi, wifiColor);
    this->pixels.show();
  }
}