////////////////////////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////////

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

int hourWordclock;
int minuteWordclock;
int counter = 0;

boolean showMinuteTicks = true;

unsigned int localPort = 2390;            // local port to listen for UDP packets

IPAddress timeServerIP;                   // time.nist.gov NTP server address
const char *ntpServerName = "time.nist.gov";

const int WORDCLOCK_NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[WORDCLOCK_NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

//Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};    //Central European Summer Time
TimeChangeRule CET = {"CET", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone CE(CEST, CET);
time_t timeWithDST;

WiFiUDP ntpUDP;

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
int WordMinEins[] = {113, -1};                         // *
int WordMinZwei[] = {113, 114, -1};                    // **
int WordMinDrei[] = {113, 114, 116, -1};               // ***
int WordMinVier[] = {113, 114, 116, 117, -1};          // ****

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
uint32_t Black = pixels.Color(0, 0, 0);
uint32_t White = pixels.Color(49, 52, 34);
uint32_t Green = pixels.Color(10, 90, 0);
uint32_t Red = pixels.Color(90, 0, 0);
uint32_t Blue = pixels.Color(0, 20, 85);

uint32_t foreground = White;
uint32_t background = Black;


int WortWifi[] = {49, 60, 59, 73, -1};
int WortConnect[] = {21, 19, 18, 36, 35, 34, 52, 55, -1};


void blank() {
  //clear the decks
  for (int x = 0; x < NUMPIXELS; ++x) {
    pixels.setPixelColor(x, background);
  }
}

void showConnectWLAN() {
  blank();
  for (int wifiindex = 0; wifiindex < 5; wifiindex++) {
    if(WortWifi[wifiindex] == -1) {
      break;
    }
    Serial.print("Wifibuchstaben: ");
    Serial.println(wifiindex);
    pixels.setPixelColor(WortWifi[wifiindex], Blue);
    pixels.show();
    delay(250);
  }
  for (int connectindex = 0; connectindex < 9; connectindex++) {
    if(WortConnect[connectindex] == -1) {
      break;
    }
    Serial.print("Connectbuchstaben: ");
    Serial.println(connectindex);
    pixels.setPixelColor(WortConnect[connectindex], White);
    pixels.show();
    delay(250);
  }
}

void setup() {
  pixels.begin();
  blank();
  Serial.begin(115200);

  wifi_station_set_hostname("wordclock");

  // Displays Wifi Connect screen
  showConnectWLAN();
  
  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  // Uncomment and run it once, if you want to erase all the stored information
  //wifiManager.resetSettings();

  // set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));


  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "WordClock"
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("wordclock");
  // or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();

  // if you get here you have connected to the WiFi
  Serial.println("Connected.");

  Serial.println("Starting UDP");
  ntpUDP.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(ntpUDP.localPort());

  chase(pixels.Color(0, 100, 0)); // run basic screen test
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress &address) {
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, WORDCLOCK_NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  ntpUDP.beginPacket(address, 123); //NTP requests are to port 123
  ntpUDP.write(packetBuffer, WORDCLOCK_NTP_PACKET_SIZE);
  ntpUDP.endPacket();
}

void getTimefromNTP() {
  WiFi.hostByName(ntpServerName, timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);

  int cb = ntpUDP.parsePacket();
  if (!cb) {
    Serial.println("no packet yet");
  } else {
    Serial.print("packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    ntpUDP.read(packetBuffer, WORDCLOCK_NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    //Serial.print("Seconds since Jan 1 1900 = " );
    //Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    //Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    //Serial.println(epoch);

    TimeChangeRule *tcr;
    time_t utc;
    utc = epoch;

    timeWithDST = CE.toLocal(utc, &tcr);

    getGlobalTime();
  }
}

void getGlobalTime() {
  minuteWordclock = minute(timeWithDST);
  hourWordclock = hour(timeWithDST);
}


void lightup(int Word[], uint32_t Colour) {
  for (int x = 0; x < pixels.numPixels() + 1; x++) {
    if (Word[x] == -1) {
      break;
    } //end of if loop
    else {
      pixels.setPixelColor(Word[x], Colour);
    } // end of else loop
  } //end of for loop
}

// Basic Strip test
static void chase(uint32_t c) {
  for (uint16_t i = 0; i < pixels.numPixels() + 4; i++) {
    pixels.setPixelColor(i, c); // Draw new pixel
    pixels.setPixelColor(i - 4, 0); // Erase pixel a few steps back
    pixels.show();
    delay(25);
  }
}

void printHour() {
  if (minuteWordclock < 25) {
    lightup(WordStunden[hourWordclock % 12], foreground);
  } else {
    lightup(WordStunden[(hourWordclock % 12) + 1], foreground);
  }
}

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

void loop() {
  if (counter == 0) {

    getTimefromNTP();
  } else {
    if (counter < 250) {
      counter++;
    } else {
      counter = 0;
      getTimefromNTP();
    }
  }

  displayTime(); // display the real-time clock data on the Serial Monitor and the LEDS,

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

void displayTime() {
  getGlobalTime();

  Serial.print(hourWordclock);
  Serial.print(":");
  Serial.println(minuteWordclock);
  Serial.println("");
  counter++;
  Serial.print("Counter: ");
  Serial.println(counter);
  Serial.println("");
  delay(200);
}
