/////////////////////////////////////////////
//
// v2.0.0
//
// LOLIN (WEMOS) D1 mini Lite (ESP8266) Wordclock Program
// Based on scripts and snippets from:
// Rui Santos http://randomnerdtutorials.com
// neotrace https://www.instructables.com/id/WORK-IN-PROGRESS-Ribba-Word-Clock-With-Wemos-D1-Mi/
// and others
//
// Manuel Meister, 2024-06-27
// Thanks to Kurt Meister for introducing me to the Arduino universe.
//
/////////////////////////////////////////////

#include <Arduino.h>
#include <WiFiManager.h>
#include <Timezone.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <RTClib.h>

#include "settings.h"
#include "layout.h"

int lastMinuteWordClock = 61;
int lastSecondWordClock = 61;

RTC_DS3231 rtc;

void formatDigits(int digits);

void serialTime();

//Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule CET = {"CET", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone CE(CEST, CET);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.ch.pool.ntp.org");
WordLayout wl = WordLayout();

/**
 * Print time via serial output
 */
void serialTime() {
  // digital clock display of the time
  formatDigits(wl.hour);
  Serial.print(":");
  formatDigits(wl.minute);
  Serial.print(":");
  formatDigits(wl.second);
}

/**
 * utility for digital clock display: prints preceding colon and leading 0
 * @param digits
 */
void formatDigits(int digits) {
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

/**
 * Updates the global time values with the time from the NTPClient
 */
void getLocalTime() {
  TimeChangeRule *tcr;
  time_t utc = rtc.now().secondstime();
  time_t timeWithDST = CE.toLocal(utc, &tcr);

  // global time values
  wl.minute = minute(timeWithDST);
  wl.hour = hour(timeWithDST);
  wl.second = second(timeWithDST);
}

void updateFromNTP() {
  if (timeClient.forceUpdate()) {
    rtc.adjust(DateTime(timeClient.getEpochTime()));
    getLocalTime();
  }
}

/**
 * Initializes timeClient so it queries the NTP server
 * also makes first update to sync the time
 */
void setupTime() {
  timeClient.begin();
  updateFromNTP();
  timeClient.end();
}

/**
 * Sets up wifi
 */
bool setupWifi() {
  wifi_station_set_hostname("wordclock");

  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  // Uncomment and run it once, if you want to erase all the stored information
  //wifiManager.resetSettings();

  // set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  // Displays Wifi Connect screen
  wl.connectWLAN();

  wifiManager.setConfigPortalTimeout(60);

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "WordClock"
  // and goes into a blocking loop awaiting configuration
  bool connected = wifiManager.autoConnect("wordclock");
  // or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();

  if (connected) {
    // if you get here you have connected to the WiFi
    Serial.println("Connected.");
  }
  return connected;
}

void setup() {
  Serial.begin(115200);
  Serial.println("");
  wl.setupDisplay();

  wl.chase(DarkestWhite); // run basic screen test and show success
  wl.showLogo(White);

  rtc.begin();
  if (setupWifi()) {
    setupTime();
    wl.showWifiSuccess();
  }
}

int currentTick = 1;

/**
 * Displays current time if minute changed
 */
void loop() {
  getLocalTime();
  if (lastMinuteWordClock != wl.minute) { //update the display only if time has changed
    #ifdef WORDCLOCK_SERIAL_DEBUG
    serialTime();
    Serial.print(" - ");
    wl.update();
    Serial.print(" | TEMP: ");
    Serial.println(rtc.getTemperature());
    #endif
    #ifndef WORDCLOCK_SERIAL_DEBUG
    wl.update();
    Serial.println();
    #endif
    lastMinuteWordClock = wl.minute;
    lastSecondWordClock = wl.second;
  } else if (lastSecondWordClock != wl.second) {
    #ifdef WORDCLOCK_SERIAL_DEBUG
    serialTime();
    Serial.print(" - ");
    wl.update();
    Serial.print(" | TEMP: ");
    Serial.println(rtc.getTemperature());
    #endif
    #ifndef WORDCLOCK_SERIAL_DEBUG
    wl.update();
    Serial.println();
    #endif
    lastSecondWordClock = wl.second;
  } else {
    if (currentTick == 0) {
      wl.update();
    }
    currentTick = (currentTick + 1) % WORDCLOCK_REFRESH_RATE;
  }
}
