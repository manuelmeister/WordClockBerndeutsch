/////////////////////////////////////////////
//
// v1.1.0
//
// LOLIN (WEMOS) D1 mini Lite (ESP8266) Wordclock Program
// Based on sripts and snippets from:
// Rui Santos http://randomnerdtutorials.com
// neotrace https://www.instructables.com/id/WORK-IN-PROGRESS-Ribba-Word-Clock-With-Wemos-D1-Mi/
// and others
//
// Kurt Meister, 2018-12-24
// Thanks to Manuel Meister for refactoring and adding automated summertime conversion.
//
/////////////////////////////////////////////

#include <Arduino.h>
#include <ESP8266WiFi.h> // v2.4.2
#include <WiFiManager.h> // v0.14.0
#include <WiFiUdp.h>
#include <TimeLib.h> // v1.5
#include <Timezone.h> // v1.2.2

#include "layout.h"

int lastMinuteWordClock = 61;
int failedRequests = 0;

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
static const char ntpServerName[] = "0.ch.pool.ntp.org";

const int timeZone = 0;     // Central European Time

// function definitions
time_t getNtpTime();

void formatDigits(int digits);

void serialTime();

void sendNTPpacket(IPAddress &address);

IPAddress ntpServerIP;

time_t extractNtpTime();

void resolveNtpServerAddress();

unsigned long requestNtpTime();

//Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule CET = {"CET", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone CE(CEST, CET);
time_t timeWithDST;

WiFiUDP ntpUDP;
unsigned int localPort = 8888;  // local port to listen for UDP packets

WordLayout wl = WordLayout();

/**
 * Print time via serial output
 */
void serialTime() {
  // digital clock display of the time
  Serial.println("");
  formatDigits(wl.hour);
  Serial.print(":");
  formatDigits(wl.minute);
  Serial.print(" - ");
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
  time_t utc;
  utc = now();
  timeWithDST = CE.toLocal(utc, &tcr);

  // global time values
  wl.minute = minute(timeWithDST);
  wl.hour = hour(timeWithDST);
}

time_t getNtpTime() {
  wl.setWifiStatus(DarkestBlue);
  unsigned long seconds = requestNtpTime();
  if (seconds > 0) {
    wl.setWifiStatus(DarkestGreen);
    Serial.print("Received: ");
    Serial.println(seconds);
    failedRequests = 0;
  } else {
    Serial.println("No NTP Response :-(");
    if (failedRequests++ > 10) {
      Serial.println("NTP Request failed 10 times in a row");
      wl.setWifiStatus(Red, 500000000);
      failedRequests = 0;
    }
  }
  return seconds;
}

unsigned long requestNtpTime() {
  unsigned long seconds = 0; // return 0 if unable to get the time (Timelib just queries again)
  Serial.println("Transmit NTP Request");
  while (ntpUDP.parsePacket() > 0); // discard any previously received packets
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 2500) {
    int size = ntpUDP.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      seconds = extractNtpTime();
    }
  }
  return seconds;
}

void resolveNtpServerAddress() {// get a random server from the pool
  Serial.println("Resolve ip of the servername");
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
}

time_t extractNtpTime() {
  Serial.println("Receive NTP Response");
  ntpUDP.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
  unsigned long secsSince1900;
  // convert four bytes starting at location 40 to a long integer
  secsSince1900 = (unsigned long) packetBuffer[40] << 24;
  secsSince1900 |= (unsigned long) packetBuffer[41] << 16;
  secsSince1900 |= (unsigned long) packetBuffer[42] << 8;
  secsSince1900 |= (unsigned long) packetBuffer[43];
  return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 0x49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 0x49;
  packetBuffer[15] = 0x52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  ntpUDP.beginPacket(address, 123); //NTP requests are to port 123
  ntpUDP.write(packetBuffer, NTP_PACKET_SIZE);
  ntpUDP.endPacket();
}

/**
 * Initializes timeClient so it queries the NTP server
 * also makes first update to sync the time
 */
void setupTime() {
  Serial.println("Starting UDP");
  ntpUDP.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(ntpUDP.localPort());
  Serial.println("waiting for sync");
  resolveNtpServerAddress();
  setSyncProvider(getNtpTime);
  setSyncInterval(720);
}

/**
 * Sets up wifi
 */
void setupWifi() {
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

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "WordClock"
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("wordclock");
  // or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();

  // if you get here you have connected to the WiFi
  Serial.println("Connected.");
}

void setup() {
  Serial.begin(115200);
  Serial.println("");
  wl.setupDisplay();

  wl.chase(DarkestWhite); // run basic screen test and show success
  wl.showLogo(White);

  setupWifi();
  setupTime();

  wl.showWifiSuccess();
}

/**
 * Displays current time if minute changed
 */
void loop() {
  if (timeStatus() != timeNotSet) {
    if (lastMinuteWordClock != wl.minute) { //update the display only if time has changed
      getLocalTime();
      serialTime();
      wl.displayTime();
      lastMinuteWordClock = wl.minute;
    } else {
      wl.displayWifiStatus();
      getLocalTime();
    }
    if (wl.hour == 3 && wl.minute == 14) {
      resolveNtpServerAddress();
    }
  }
}
