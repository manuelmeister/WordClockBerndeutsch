/**
 *
 * v1.1.1
 *
 * LOLIN (WEMOS) D1 mini Lite (ESP8266) Wordclock Program
 * Based on sripts and snippets from:
 * Rui Santos http://randomnerdtutorials.com
 * neotrace https://www.instructables.com/id/WORK-IN-PROGRESS-Ribba-Word-Clock-With-Wemos-D1-Mi/
 * and others
 *
 * Kurt Meister, 2018-12-24
 * Thanks to Manuel Meister for refactoring and adding automated summertime conversion.
 *
 * Changelog:
 * 2019-01-07 Changed to PaulStoffregen TimeLib Code because NTPClient seems to run into long time errors
 *
 */

#include <Arduino.h>
#include <ESP8266WiFi.h> // v2.4.2
#include <WiFiManager.h> // v0.14.0
#include <WiFiUdp.h>
#include <TimeLib.h> // v1.5
#include <Timezone.h> // v1.2.2
#include <Adafruit_NeoPixel.h> // v1.1.7

// Defines some variables
#define PIN            D8
#define NUMPIXELS     121

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DEBUG_ON

int secondWordclock = 0;
int minuteWordclock = 0;
int hourWordclock = 0;
int lastMinuteWordClock = 61;

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
static const char ntpServerName[] = "0.ch.pool.ntp.org";

const int timeZone = 0;     // Central European Time

bool showMinuteTicks = true;


// function definitions
time_t getNtpTime();

void formatDigits(int digits);

void serialTime();

void sendNTPpacket(IPAddress &address);

//Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule CET = {"CET", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone CE(CEST, CET);
time_t timeWithDST;

WiFiUDP ntpUDP;
unsigned int localPort = 8888;  // local port to listen for UDP packets

/*
| Ä | S |   | I | S | C | H |   | F | Ü | F |
| V | I | E | R | T | U |   |   | Z | Ä |   |
| Z | W | Ä | N | Z | G |   |   | V | O | R |
|   | A | B |   | H | A | U | B | I |   |   |
| E | I | S |   | Z | W | Ö | I | D | R | Ü |
|   | V | I | E | R | I | F | Ü | F | I |   |
| S | Ä | C | H | S | I | S | I | B | N | I |
|   | A | C | H | T | I | N | Ü | N | I |   |
|   | Z | Ä | N | I | E | U | F | I |   |   |
|   |   |   | Z | W | Ö | U | F | I |   |   |
|   |   |   | * | * | * | * |   |   |   |   |
 */

//Actual words as array variables
int WordEs[] = {0, 1, -1};                             // ÄS
int WordIst[] = {3, 4, 5, 6, -1};                      // ISCH
int WordMinFuenf[] = {8, 9, 10, -1};                   // FÜF
int WordViertel[] = {21, 20, 19, 18, 17, 16, -1};      // VIERTU
int WordMinZehn[] = {13, 12, -1};                      // ZÄ
int WordZwanzig[] = {22, 23, 24, 25, 26, 27, -1};      // ZWÄNZG
int WordVor[] = {30, 31, 32, -1};                      // VOR
int WordNach[] = {42, 41, -1};                         // AB
int WordHalb[] = {39, 38, 37, 36, 35, -1};             // HAUBI
int WordEins[] = {44, 45, 46, -1};                     // EIS
int WordZwei[] = {48, 49, 50, 51, -1};                 // ZWÖI
int WordDrei[] = {52, 53, 54, -1};                     // DRÜ
int WordVier[] = {64, 63, 62, 61, 60, -1};             // VIERI
int WordFuenf[] = {59, 58, 57, 56, -1};                // FÜFI
int WordSechs[] = {66, 67, 68, 69, 70, 71, -1};        // SÄCHSI
int WordSieben[] = {72, 73, 74, 75, 76, -1};           // SIBNI
int WordAcht[] = {86, 85, 84, 83, 82, -1};             // ACHTI
int WordNeun[] = {81, 80, 79, 78, -1};                 // NÜNI
int WordZehn[] = {89, 90, 91, 92, -1};                 // ZÄNI
int WordElf[] = {93, 94, 95, 96, -1};                  // EUFI
int WordZwoelf[] = {106, 105, 104, 103, 102, 101, -1}; // ZWÖUFI
int WordMinEins[] = {113, -1};                    // *
int WordMinZwei[] = {113, 114, -1};               // **
int WordMinDrei[] = {113, 114, 116, -1};          // ***
int WordMinVier[] = {113, 114, 116, 117, -1};     // ****

int *WordStunden[] = {
  WordZwoelf,
  WordEins,
  WordZwei,
  WordDrei,
  WordVier,
  WordFuenf,
  WordSechs,
  WordSieben,
  WordAcht,
  WordNeun,
  WordZehn,
  WordElf
};

int *WordMinuten[] = {
  WordMinFuenf,
  WordMinZehn,
  WordViertel,
  WordZwanzig,
  WordMinFuenf
};

//define colours
uint32_t Black = Adafruit_NeoPixel::Color(0, 0, 0);
uint32_t White = Adafruit_NeoPixel::Color(49, 52, 34);
uint32_t Green = Adafruit_NeoPixel::Color(10, 90, 0);
uint32_t Red = Adafruit_NeoPixel::Color(90, 0, 0);
uint32_t Blue = Adafruit_NeoPixel::Color(0, 20, 85);

uint32_t foreground = White;
uint32_t background = Black;


/**
 * Print time via serial output
 */
void serialTime() {
  // digital clock display of the time
  formatDigits(hourWordclock);
  Serial.print(":");
  formatDigits(minuteWordclock);
  Serial.print(":");
  formatDigits(secondWordclock);
  Serial.println();
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
 * Sets array of pixels to a specific color
 * @param Word
 * @param Colour
 */
void lightup(int Word[], uint32_t Colour) {
  for (int x = 0; x < pixels.numPixels() + 1; x++) {
    if (Word[x] == -1) {
      break;
    } else {
      pixels.setPixelColor(Word[x], Colour);
    }
  }
}

/**
 * Sets all pixels to the background
 */
void blank() {
  for (int x = 0; x < NUMPIXELS; ++x) {
    pixels.setPixelColor(x, background);
  }
}

/**
 * Sets all pixels to the background and displays it
 */
void wipe() {
  blank();
  pixels.show();
}

/**
 * Snail runs throug all pixels
 * @param color Color of the snail
 */
static void chase(uint32_t color) {
  for (uint16_t i = 0; i < pixels.numPixels() + 4; i++) {
    pixels.setPixelColor(i, color); // Draw new pixel
    pixels.setPixelColor(i - 4, background); // Erase pixel a few steps back
    pixels.show();
    delay(25);
  }
}

/**
 * Displays the prompt to connect to wifi
 */
static void connectWLAN() {
  blank();

  int WortWifi[] = {49, 60, 59, 73};
  int WortConnect[] = {21, 19, 18, 36, 35, 34, 52, 55};

  for (int i : WortWifi) {
    pixels.setPixelColor(i, Blue);
    pixels.show();
    delay(250);
  }
  for (int i : WortConnect) {
    pixels.setPixelColor(i, White);
    pixels.show();
    delay(250);
  }
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
  connectWLAN();

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

time_t getNtpTime() {
  IPAddress ntpServerIP; // NTP server's ip address

  while (ntpUDP.parsePacket() > 0); // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 2500) {
    int size = ntpUDP.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response...");
      ntpUDP.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 = (unsigned long) packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long) packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long) packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long) packetBuffer[43];
      Serial.print("Received: ");
      Serial.println(secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR);
      setSyncInterval(300);
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time (Timelib just queries again)
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
  setSyncProvider(getNtpTime);
  setSyncInterval(10);
  serialTime();
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
  minuteWordclock = minute(timeWithDST);
  hourWordclock = hour(timeWithDST);
  secondWordclock = second(timeWithDST);
}

/**
 * Sets the pixels for the hour
 */
void printHour() {
  if (minuteWordclock < 25) {
    // show this hour if we are before 25 minutes past
    lightup(WordStunden[hourWordclock % 12], foreground);
  } else {
    // show next hour
    lightup(WordStunden[(hourWordclock % 12) + 1], foreground);
  }
}

/**
 * Sets pixels for the current minutes values
 */
void printMinute() {
  if (minuteWordclock != 0) {
    if (minuteWordclock >= 5 && minuteWordclock < 30) {
      lightup(WordMinuten[(minuteWordclock / 5) - 1], foreground);
    } else if (minuteWordclock >= 35) {
      lightup(WordMinuten[5 - ((minuteWordclock - 30) / 5)], foreground);
    }
    if (minuteWordclock >= 5 && minuteWordclock < 25 || minuteWordclock < 40 && minuteWordclock >= 35) {
      lightup(WordNach, foreground);
    }
    if (minuteWordclock >= 40 || minuteWordclock >= 25 && minuteWordclock < 30) {
      lightup(WordVor, foreground);
    }
    if (minuteWordclock >= 25 && minuteWordclock < 40) {
      lightup(WordHalb, foreground);
    }
  }
}

/**
 * Displays the current time
 */
void displayTime() {
  blank();

  // light up "it's" it stays on
  lightup(WordEs, foreground);
  lightup(WordIst, foreground);

  // light up hours
  printHour();

  // Light up minutes
  printMinute();

  // Light up subfive minutes
  if (showMinuteTicks) {
    if (minuteWordclock % 5 == 1) {
      lightup(WordMinEins, foreground);
    }
    if (minuteWordclock % 5 == 2) {
      lightup(WordMinZwei, foreground);
    }
    if (minuteWordclock % 5 == 3) {
      lightup(WordMinDrei, foreground);
    }
    if (minuteWordclock % 5 == 4) {
      lightup(WordMinVier, foreground);
    }
  }

  pixels.show();
}

void setup() {
  pixels.begin();
  wipe();
  Serial.begin(115200);

  setupWifi();
  setupTime();

  chase(Green); // run basic screen test and show success
}

/**
 * Shows current time
 * To be in sync it updates the timeClient only after the minute change
 */
void loop() {
  if (timeStatus() != timeNotSet) {
    if (lastMinuteWordClock != minuteWordclock) { //update the display only if time has changed
      serialTime();
      displayTime();
      getLocalTime();
      lastMinuteWordClock = minuteWordclock;
    } else {
      getLocalTime();
    }
  }
}
