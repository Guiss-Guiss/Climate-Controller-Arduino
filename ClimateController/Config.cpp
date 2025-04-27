/*
 * Config.cpp
 * 
 * Implementation of constants and global variables.
 * 
 * Written by Guillaume Ste-Marie
 * GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
 */

#include "Config.h"

// Pin Assignments - Input Buttons
const int setswitch = 8;       // Set/confirm button for menu selections
const int upswitch = 9;        // Up button for increasing values
const int downswitch = 10;     // Down button for decreasing values
const int leftswitch = 11;     // Left button for menu navigation
const int rightswitch = 12;    // Right button for menu navigation

// Pin Assignments - Output Relays
const int heat = 2;            // Heating system relay control
const int cool = 3;            // Cooling/exhaust fan relay control
const int humLow = 4;          // Humidifier relay (increases humidity)
const int humHigh = 5;         // Dehumidifier relay (decreases humidity)
const int lights = 6;          // Lighting system relay control

// Timing Constants (in milliseconds unless noted)
const unsigned long LOOP_INTERVAL = 10000;      // Sensor reading interval (10 seconds)
const unsigned long BUTTON_INTERVAL = 200;      // Minimum time between button actions for responsiveness
const unsigned long DEBOUNCE_INTERVAL = 10;     // Button debounce time to filter noise
const unsigned long RESET_THRESHOLD = 0xF0000000; // ~4 billion ms threshold to reset timers (prevent overflow)

// Hardware Interface Objects
uEEPROMLib ueeprom(0x57);                   // EEPROM at I2C address 0x57 for persistent storage
RTC_DS3231 rtc;                             // DS3231 Real-Time Clock for time tracking
// LCD with I2C address 0x27 - standard PCF8574 I2C expander for most common 20x4 displays
LiquidCrystal_I2C lcd(0x27, 20, 4);         // 20x4 character LCD display at I2C address 0x27
DHT dht(DHTPIN, DHTTYPE);                   // DHT22 temperature and humidity sensor

// Button Objects
Bounce setbutton = Bounce();
Bounce upbutton = Bounce();
Bounce downbutton = Bounce();
Bounce leftbutton = Bounce();
Bounce rightbutton = Bounce();

// String buffer for temporary text storage
char buffer[40];

// Menu System State Variables
int menu = 0;                    // Current active menu (see MenuSystem.h for values)
bool hasrun = false;             // Tracks initialization state of current menu

// User Preference Variables
bool langChoice = 0;             // Language selection (0=French, 1=English)
bool unitChoice = 0;             // Temperature unit (0=Celsius, 1=Fahrenheit)

// Sensor Reading Variables
float humidity = 0.0;            // Current relative humidity reading (%)
float temperatureCelcius = 0.0;    // Current temperature in Celsius
float temperatureFahrenheit = 0.0; // Current temperature in Fahrenheit

// Temperature Control Thresholds (default values, loaded from EEPROM in normal operation)
int lowTempCelcius = 20;       // Below this temperature (°C), heating activates
int highTempCelcius = 25;      // Above this temperature (°C), cooling activates
int lowTempFahrenheit = 68;    // Below this temperature (°F), heating activates
int highTempFahrenheit = 77;   // Above this temperature (°F), cooling activates

// Humidity Control Thresholds (default values, loaded from EEPROM in normal operation)
int lowHumidity = 40;          // Below this humidity (%), humidifier activates
int highHumidity = 60;         // Above this humidity (%), dehumidifier activates

// Light Timer Settings (default values, loaded from EEPROM in normal operation)
int startHour = 8;             // Hour when lights should turn on (24-hour format)
int startMinute = 0;           // Minute when lights should turn on
int stopHour = 20;             // Hour when lights should turn off (24-hour format)
int stopMinute = 0;            // Minute when lights should turn off

// Time Variables
int hourValue = 0;             // Current hour value from RTC
int minuteValue = 0;           // Current minute value from RTC

// Custom LCD Character Definitions (8x8 bitmap patterns)
uint8_t deg[8] = {             // Degree symbol (°) for temperature display
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
};

uint8_t up[8] = {              // Up arrow symbol (↑) for menu navigation
  B00100,
  B01110,
  B11111,
  B00000,
  B00000,
  B00100,
  B01110,
  B11111
};

uint8_t down[8] = {            // Down arrow symbol (↓) for menu navigation
  B11111,
  B01110,
  B00100,
  B00000,
  B00000,
  B11111,
  B01110,
  B00100
};

// Timing Tracking Variables
unsigned long previousSensorTime = 0;  // Last time sensors were read (for interval management)
unsigned long lastDebounceTime = 0;    // Last time a button was pressed (for debounce management)

// Buffer is already declared above on line 45
