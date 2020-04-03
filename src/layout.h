#include <Adafruit_NeoPixel.h>

#ifndef WordLayout_h
#define WordLayout_h


static uint32_t Black = Adafruit_NeoPixel::Color(0, 0, 0);
static uint32_t White = Adafruit_NeoPixel::Color(49, 52, 34);
static uint32_t DarkWhite = Adafruit_NeoPixel::Color(36, 39, 25);
static uint32_t DarkerWhite = Adafruit_NeoPixel::Color(24, 26, 17);
static uint32_t DarkestWhite = Adafruit_NeoPixel::Color(12, 13, 8);
static uint32_t Green = Adafruit_NeoPixel::Color(10, 90, 0);
static uint32_t DarkGreen = Adafruit_NeoPixel::Color(5, 45, 0);
static uint32_t DarkestGreen = Adafruit_NeoPixel::Color(1, 9, 0);
static uint32_t Red = Adafruit_NeoPixel::Color(90, 0, 0);
static uint32_t Blue = Adafruit_NeoPixel::Color(0, 20, 85);
static uint32_t DarkBlue = Adafruit_NeoPixel::Color(0, 10, 40);
static uint32_t DarkestBlue = Adafruit_NeoPixel::Color(0, 8, 20);



static int WordEs[] = {0, 1, -1};
static int WordIst[] = {3, 4, 5, 6, -1};
static int WordBereits[] = {4, 5, 6, 7, -1};
static int WordUngefaehr[] = {8, 9, 10, 11, -1};
static int WordHalb[] = {95, 94, 93, 92, 91, -1};
static int WordMeister[] = {49, 50, 51, 52, 53, 54, 55, -1};
static int WordIO[] = {56, 57, -1};
static int WordSlash[] = {58, -1};
static int WordZeit[] = {59, 60, 83, -1};


static int WordFix[] = {104, 105, 106, -1};
static int WordNach[] = {78, 79, -1};
static int WordVor[] = {80, 81, 82, -1};
static int WordGewesen[] = {134, 133, 132, -1};
static int WordBald[] = {23, 22, 21, -1};
static int WordNein[] = {20, 19, 18, -1};
static int WordJa[] = {77, 78, -1};
static int WordAeuae[] = {115, 124, 138, -1};

static int SymbolWifi[] = {107, -1};
static int SymbolElephant[] = {119, -1};
static int SymbolCat[] = {48, -1};

static int *WordURL[] = {WordMeister, WordIO, WordSlash, WordZeit, WordFix, SymbolWifi};

// Stunde
static int WordStundeEins[] = {118, 117, 116, -1};
static int WordStundeZwei[] = {89, 88, 87, 105, -1};
static int WordStundeDrei[] = {120, 121, 122, -1};
static int WordStundeVier[] = {96, 97, 98, 99, 100, -1};
static int WordStundeFunf[] = {102, 103, 104, 105, -1};
static int WordStundeSechs[] = {116, 115, 114, 113, 112, 111, -1};
static int WordStundeSieben[] = {112, 111, 110, 109, 108, -1};
static int WordStundeAcht[] = {127, 128, 129, 130, 131, -1};
static int WordStundeNeun[] = {143, 142, 141, 140, -1};
static int WordStundeZehn[] = {139, 138, 137, 136, -1};
static int WordStundeElf[] = {123, 124, 125, 126, -1};
static int WordStundeZwolf[] = {89, 88, 87, 86, 85, 84, -1};

static int *WordStunden[] = {WordStundeZwolf, WordStundeEins, WordStundeZwei, WordStundeDrei, WordStundeVier,
                      WordStundeFunf, WordStundeSechs, WordStundeSieben, WordStundeAcht, WordStundeNeun,
                      WordStundeZehn, WordStundeElf, WordStundeZwolf
};

// Minute
static int WordMinLeer[] = {-1};
static int WordMinEins[] = {24, 25, 26, -1};
static int WordMinZwei[] = {47, 46, 45, 51, -1};
static int WordMinDrei[] = {71, 70, 69, -1};
static int WordMinVier[] = {17, 16, 15, 14, -1};
static int WordMinFunf[] = {43, 42, 41, -1};
static int WordMinSechs[] = {26, 27, 28, 29, 30, -1};
static int WordMinSieben[] = {30, 31, 32, 33, -1};
static int WordMinAcht[] = {39, 38, 37, 36, -1};
static int WordMinNeun[] = {72, 73, 74, -1};
static int WordMinZehn[] = {75, 76, -1};
static int WordMinElf[] = {33, 34, 35, -1};
static int WordMinZwolf[] = {47, 46, 45, 44, 43, -1};
static int WordMinDreizehn[] = {71, 70, 51, 75, 76, -1};
static int WordMinVierzehn[] = {17, 16, 15, 14, 75, 76, -1};
static int WordMinFuenfzehn[] = {43, 42, 41, 75, 76, -1};
static int WordMinViertel[] = {17, 16, 15, 14, 13, 12, -1};
static int WordMinSechzehn[] = {75, 76, -1};
static int WordMinSiebzehn[] = {75, 76, -1};
static int WordMinAchtzehn[] = {39, 38, 37, 36, 75, 76, -1};
static int WordMinNeunzehn[] = {72, 73, 74, 75, 76, -1};
static int WordMinZwanzig[] = {66, 65, 64, 63, 62, 61, -1};
static int WordMinEinunzwanzig[] = {24, 25, 68, 67, 66, 65, 64, 63, 62, 61, -1};
static int WordMinZweinunzwanzig[] = {47, 46, 45, 51, 67, 66, 65, 64, 63, 62, 61, -1};
static int WordMinDreiunzwanzig[] = {71, 70, 69, 67, 66, 65, 64, 63, 62, 61, -1};
static int WordMinVierunzwanzig[] = {17, 16, 15, 14, 67, 66, 65, 64, 63, 62, 61, -1};
static int WordMinFuenfunzwanzig[] = {43, 42, 41, 67, 66, 65, 64, 63, 62, 61, -1};
static int WordMinSechsunzwanzig[] = {26, 27, 28, 29, 30, 67, 66, 65, 64, 63, 62, 61, -1};
static int WordMinSiebenunzwanzig[] = {30, 31, 32, 33, 68, 67, 66, 65, 64, 63, 62, 61, -1};
static int WordMinAchtunzwanzig[] = {39, 38, 37, 36, 67, 66, 65, 64, 63, 62, 61, -1};
static int WordMinNeununzwanzig[] = {72, 73, 74, 67, 66, 65, 64, 63, 62, 61, -1};

static int *WordMinuten[] = {WordMinFunf, WordMinZehn, WordMinViertel, WordMinZwanzig, WordMinFunf};


static int *WordMinutenSemiExact[][59] = {
  {WordMinEins,    WordMinLeer},
  {WordMinZwei,    WordMinLeer},
  {WordMinDrei,    WordMinLeer},
  {WordMinVier,    WordMinLeer},
  {WordMinFunf,    WordMinLeer},
  {WordMinSechs,   WordMinLeer},
  {WordMinSieben,  WordMinLeer},
  {WordMinAcht,    WordMinLeer},
  {WordMinNeun,    WordMinLeer},
  {WordMinZehn,    WordMinLeer},
  {WordMinElf,     WordMinLeer},
  {WordMinZwolf,   WordMinLeer},
  {WordBald,       WordMinViertel, WordMinLeer},
  {WordUngefaehr,  WordMinViertel, WordMinLeer},
  {WordMinViertel, WordMinLeer},
  {WordMinViertel, WordGewesen,    WordMinLeer},
  {WordUngefaehr,  WordMinViertel, WordGewesen, WordMinLeer},
  {WordBald,       WordMinZwanzig, WordMinLeer},
  {WordUngefaehr,  WordMinZwanzig, WordMinLeer},
  {WordMinZwanzig, WordMinLeer},
  {WordMinZwanzig, WordGewesen,    WordMinLeer},
  {WordUngefaehr,  WordMinZwanzig, WordGewesen, WordMinLeer},
  {WordBald,       WordMinFunf,    WordMinLeer},
  {WordUngefaehr,  WordMinFunf,    WordMinLeer},
  {WordMinFunf,    WordMinLeer},
  {WordMinVier,    WordMinLeer},
  {WordMinDrei,    WordMinLeer},
  {WordMinZwei,    WordMinLeer},
  {WordMinEins,    WordMinLeer},
  {WordMinLeer},
  {WordMinEins,    WordMinLeer},
  {WordMinZwei,    WordMinLeer},
  {WordMinDrei,    WordMinLeer},
  {WordMinVier,    WordMinLeer},
  {WordMinFunf,    WordMinLeer},
  {WordMinFunf,    WordGewesen,    WordMinLeer},
  {WordUngefaehr,  WordMinFunf,    WordGewesen, WordMinLeer},
  {WordBald,       WordMinZwanzig, WordMinLeer},
  {WordUngefaehr,  WordMinZwanzig, WordMinLeer},
  {WordMinZwanzig, WordMinLeer},
  {WordMinZwanzig, WordGewesen,    WordMinLeer},
  {WordUngefaehr,  WordMinZwanzig, WordGewesen, WordMinLeer},
  {WordBald,       WordMinViertel, WordMinLeer},
  {WordUngefaehr,  WordMinViertel, WordMinLeer},
  {WordMinViertel, WordMinLeer},
  {WordMinViertel, WordGewesen,    WordMinLeer},
  {WordUngefaehr,  WordMinViertel, WordGewesen, WordMinLeer},
  {WordMinZwolf,   WordMinLeer},
  {WordMinElf,     WordMinLeer},
  {WordMinZehn,    WordMinLeer},
  {WordMinNeun,    WordMinLeer},
  {WordMinAcht,    WordMinLeer},
  {WordMinSieben,  WordMinLeer},
  {WordMinSechs,   WordMinLeer},
  {WordMinFunf,    WordMinLeer},
  {WordMinVier,    WordMinLeer},
  {WordMinDrei,    WordMinLeer},
  {WordMinZwei,    WordMinLeer},
  {WordMinEins,    WordMinLeer}
};

static int *WordMinutenExact[] = {WordMinEins, WordMinZwei, WordMinDrei, WordMinVier, WordMinFunf, WordMinSechs,
                           WordMinSieben, WordMinAcht, WordMinNeun, WordMinZehn, WordMinElf, WordMinZwolf,
                           WordMinDreizehn, WordMinVierzehn, WordMinViertel, WordMinSechzehn, WordMinSiebzehn,
                           WordMinAchtzehn, WordMinNeunzehn, WordMinZwanzig, WordMinEinunzwanzig,
                           WordMinZweinunzwanzig, WordMinDreiunzwanzig, WordMinVierunzwanzig, WordMinFunf,
                           WordMinVier, WordMinDrei, WordMinZwei, WordMinEins
};

class WordLayout {
public:
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(144, D8, NEO_GRB + NEO_KHZ800);
    uint32_t foregroundColor;
    uint32_t backgroundColor;
    int hour;
    int minute;
    void setupDisplay();
    void connectWLAN();
    void displayTime();
    /**
     * Runs through all pixels
     * @param color
     */
    void chase(uint32_t color);

    /**
     * Runs through all pixels
     * @param color
     */
    void showLogo(uint32_t color);

    /**
     * Checks if pixel should be hidden
     * @param color
     */
    bool hidePixel(int pixel);
    void hidePixelRec(int index, int depth);
    void showWifiSuccess();

    /**
     * Sets array of pixels to a specific color
     * @param Word array of the id of the pixel
     * @param Color
     */
    void lightup(int *Word, uint32_t Color);
    void setWifiStatus(uint32_t Color, int time = 1250);
    void displayWifiStatus();

    /**
     * Sets all pixels to the background
     */
    void blank();

    /**
     * Sets all pixels to the background and displays it
     */
    void wipe();
private:
    void showMinute();
    void showMinuteSemiExact();
    void showMinuteExact();
    void showHour();
    void showHourSemiExact();
};

#endif