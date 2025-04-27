/*
 * Config.h
 * 
 * Central configuration for the Climate Controller.
 * Contains all constants, pin definitions, and global variables.
 * 
 * Written by Guillaume Ste-Marie
 * GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
 */

#ifndef CONFIG_H
#define CONFIG_H

/* Required Arduino Libraries - Install from Library Manager:
 * 1. DHT sensor library by Adafruit (for DHT temperature/humidity sensor)
 *    - Search for "DHT sensor library" by Adafruit
 *    - Also install the required dependency: "Adafruit Unified Sensor"
 * 
 * 2. RTClib by Adafruit (for DS3231 real-time clock)
 *    - Search for "RTClib" by Adafruit
 * 
 * 3. LiquidCrystal_I2C by Frank de Brabander (for I2C LCD display)
 *    - Search for "LiquidCrystal I2C" in Library Manager
 * 
 * 4. uEEPROMLib by Naguissa (for EEPROM memory management)
 *    - Search for "uEEPROMLib" 
 * 
 * 5. Bounce2 by Thomas O Fredericks (for button debouncing)
 *    - Search for "Bounce 2"
 *
 * 6. OneWire by Jim Studt, Tom Pollard, et al. (required by DHT)
 *    - Search for "OneWire"
 */
#include <Arduino.h>
#include <OneWire.h>
#include <DHT.h>
#include <RTClib.h>
#include <Wire.h>
#include <uEEPROMLib.h>
#include <Bounce2.h>

// Include LCD library
#include <LiquidCrystal_I2C.h>

// Defines
#define DHTPIN 7        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22 (AM2302), AM2321

// Constants - Pins
extern const int setswitch;    // Set" momentary switch
extern const int upswitch;     // Up" momentary switch
extern const int downswitch;   // Down" momentary switch
extern const int leftswitch;   // Left" momentary switch
extern const int rightswitch;  // Right" momentary switch
extern const int heat;         // Output pin for heater relay
extern const int cool;         // Output pin for Exhaust fan relay
extern const int humLow;       // Output pin for humidifier relay
extern const int humHigh;      // Output pin for dehumidifier relay
extern const int lights;       // Output pin for lighting relay

// Constants - Timings
extern const unsigned long LOOP_INTERVAL;    // Interval for reading temperature and humidity
extern const unsigned long BUTTON_INTERVAL;  // Interval for reading buttons
extern const unsigned long DEBOUNCE_INTERVAL; // Interval for debouncing buttons
extern const unsigned long RESET_THRESHOLD;   // Threshold for millis() overflow reset

// Global Objects
extern uEEPROMLib ueeprom;         // EEPROM memory 
extern RTC_DS3231 rtc;             // Real Time Clock
extern LiquidCrystal_I2C lcd;      // I2C LCD display
extern DHT dht;                    // DHT sensor

// Button Objects
extern Bounce setbutton;
extern Bounce upbutton;
extern Bounce downbutton;
extern Bounce leftbutton;
extern Bounce rightbutton;

// String buffer for temporary text storage (used by many functions)
extern char buffer[40];

// Global variables
extern int menu;                  // Current menu
extern bool hasrun;               // Variable to store if the program has run

extern bool langChoice;            // Language choice (0=French, 1=English)
extern bool unitChoice;            // Unit choice (0=Celsius, 1=Fahrenheit)
extern float humidity;             // Current humidity
extern float temperatureCelcius;   // Current temperature in Celsius
extern float temperatureFahrenheit; // Current temperature in Fahrenheit

extern int lowTempCelcius;         // Low temperature in Celsius
extern int highTempCelcius;        // High temperature in Celsius
extern int lowTempFahrenheit;      // Low temperature in Fahrenheit
extern int highTempFahrenheit;     // High temperature in Fahrenheit
extern int lowHumidity;            // Low humidity
extern int highHumidity;           // High humidity
extern int startHour;              // Start hour for lights
extern int stopHour;               // Stop hour for lights
extern int startMinute;            // Start minute for lights
extern int stopMinute;             // Stop minute for lights
extern int hourValue;              // Current hour value for time setting
extern int minuteValue;            // Current minute value for time setting

// External custom characters
extern uint8_t deg[];
extern uint8_t up[];
extern uint8_t down[];

// Timing
extern unsigned long previousSensorTime;  // Previous time for sensor readings
extern unsigned long lastDebounceTime;    // Last debounce time



#endif // CONFIG_H
