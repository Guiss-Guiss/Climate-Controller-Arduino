/*
 * Memory.cpp
 * 
 * Implementation of EEPROM memory management functions.
 * 
 * Written by Guillaume Ste-Marie
 * GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
 */

#include "Memory.h"

// Read all persistent settings from EEPROM memory into global variables
void loadAllSettings() {
  // User interface settings
  ueeprom.eeprom_read(ADDR_LANG_CHOICE, &langChoice);    // Language selection (0=French, 1=English)
  ueeprom.eeprom_read(ADDR_UNIT_CHOICE, &unitChoice);    // Temperature unit (0=Celsius, 1=Fahrenheit)
  
  // Temperature thresholds in both units
  ueeprom.eeprom_read(ADDR_LOW_TEMP_C, &lowTempCelcius);
  ueeprom.eeprom_read(ADDR_HIGH_TEMP_C, &highTempCelcius);
  ueeprom.eeprom_read(ADDR_LOW_TEMP_F, &lowTempFahrenheit);
  ueeprom.eeprom_read(ADDR_HIGH_TEMP_F, &highTempFahrenheit);
  
  // Humidity thresholds
  ueeprom.eeprom_read(ADDR_LOW_HUMIDITY, &lowHumidity);
  ueeprom.eeprom_read(ADDR_HIGH_HUMIDITY, &highHumidity);
  
  // Light timer settings (start/stop)
  ueeprom.eeprom_read(ADDR_START_HOUR, &startHour);
  ueeprom.eeprom_read(ADDR_START_MINUTE, &startMinute);
  ueeprom.eeprom_read(ADDR_STOP_HOUR, &stopHour);
  ueeprom.eeprom_read(ADDR_STOP_MINUTE, &stopMinute);
}

// Persist all current settings to non-volatile EEPROM memory
void saveAllSettings() {
  // User interface settings
  ueeprom.eeprom_write(ADDR_LANG_CHOICE, langChoice);    // Language selection
  ueeprom.eeprom_write(ADDR_UNIT_CHOICE, unitChoice);    // Temperature unit
  
  // Temperature thresholds in both units
  ueeprom.eeprom_write(ADDR_LOW_TEMP_C, lowTempCelcius);
  ueeprom.eeprom_write(ADDR_HIGH_TEMP_C, highTempCelcius);
  ueeprom.eeprom_write(ADDR_LOW_TEMP_F, lowTempFahrenheit);
  ueeprom.eeprom_write(ADDR_HIGH_TEMP_F, highTempFahrenheit);
  
  // Humidity thresholds
  ueeprom.eeprom_write(ADDR_LOW_HUMIDITY, lowHumidity);
  ueeprom.eeprom_write(ADDR_HIGH_HUMIDITY, highHumidity);
  
  // Light timer settings (start/stop)
  ueeprom.eeprom_write(ADDR_START_HOUR, startHour);
  ueeprom.eeprom_write(ADDR_START_MINUTE, startMinute);
  ueeprom.eeprom_write(ADDR_STOP_HOUR, stopHour);
  ueeprom.eeprom_write(ADDR_STOP_MINUTE, stopMinute);
}

// Load language preference from EEPROM
void loadLanguageChoice() {
  // Read language setting (0=French, 1=English)
  ueeprom.eeprom_read(ADDR_LANG_CHOICE, &langChoice);
}

// Save current language preference to EEPROM
void saveLanguageChoice() {
  // Write current language setting to persistent storage
  ueeprom.eeprom_write(ADDR_LANG_CHOICE, langChoice);
}

// Load temperature thresholds from EEPROM in both Celsius and Fahrenheit
void loadTemperatureSettings() {
  // Read temperature thresholds in Celsius
  ueeprom.eeprom_read(ADDR_LOW_TEMP_C, &lowTempCelcius);
  ueeprom.eeprom_read(ADDR_HIGH_TEMP_C, &highTempCelcius);
  
  // Read temperature thresholds in Fahrenheit
  ueeprom.eeprom_read(ADDR_LOW_TEMP_F, &lowTempFahrenheit);
  ueeprom.eeprom_read(ADDR_HIGH_TEMP_F, &highTempFahrenheit);
}

// Save current temperature thresholds to EEPROM in both units
void saveTemperatureSettings() {
  // Store temperature thresholds in Celsius
  ueeprom.eeprom_write(ADDR_LOW_TEMP_C, lowTempCelcius);
  ueeprom.eeprom_write(ADDR_HIGH_TEMP_C, highTempCelcius);
  
  // Store temperature thresholds in Fahrenheit
  ueeprom.eeprom_write(ADDR_LOW_TEMP_F, lowTempFahrenheit);
  ueeprom.eeprom_write(ADDR_HIGH_TEMP_F, highTempFahrenheit);
}

// Load humidity thresholds from EEPROM (minimum and maximum)
void loadHumiditySettings() {
  // Read the low (activation) and high (deactivation) humidity thresholds
  ueeprom.eeprom_read(ADDR_LOW_HUMIDITY, &lowHumidity);   // For humidifier control
  ueeprom.eeprom_read(ADDR_HIGH_HUMIDITY, &highHumidity); // For dehumidifier control
}

// Save current humidity thresholds to EEPROM
void saveHumiditySettings() {
  // Store both humidifier and dehumidifier activation thresholds
  ueeprom.eeprom_write(ADDR_LOW_HUMIDITY, lowHumidity);   // Below this level, humidifier activates
  ueeprom.eeprom_write(ADDR_HIGH_HUMIDITY, highHumidity); // Above this level, dehumidifier activates
}

// Load lighting schedule from EEPROM (hours and minutes for ON/OFF times)
void loadTimeSettings() {
  // Read light ON time settings (when lights should turn on)
  ueeprom.eeprom_read(ADDR_START_HOUR, &startHour);    // Hour (0-23)
  ueeprom.eeprom_read(ADDR_START_MINUTE, &startMinute); // Minute (0-59)
  
  // Read light OFF time settings (when lights should turn off)
  ueeprom.eeprom_read(ADDR_STOP_HOUR, &stopHour);      // Hour (0-23)
  ueeprom.eeprom_read(ADDR_STOP_MINUTE, &stopMinute);  // Minute (0-59)
}

// Save current lighting schedule to EEPROM
void saveTimeSettings() {
  // Store light ON time settings
  ueeprom.eeprom_write(ADDR_START_HOUR, startHour);    // When lights should turn on (hour)
  ueeprom.eeprom_write(ADDR_START_MINUTE, startMinute); // When lights should turn on (minute)
  
  // Store light OFF time settings
  ueeprom.eeprom_write(ADDR_STOP_HOUR, stopHour);      // When lights should turn off (hour)
  ueeprom.eeprom_write(ADDR_STOP_MINUTE, stopMinute);  // When lights should turn off (minute)
}

// Load temperature unit preference from EEPROM (Celsius or Fahrenheit)
void loadUnitChoice() {
  // Read temperature unit setting
  // unitChoice: 0=Celsius, 1=Fahrenheit
  ueeprom.eeprom_read(ADDR_UNIT_CHOICE, &unitChoice);
}

// Save temperature unit preference to EEPROM (Celsius or Fahrenheit)
void saveUnitChoice() {
  // Store current temperature unit preference for future use
  // unitChoice: 0=Celsius, 1=Fahrenheit
  ueeprom.eeprom_write(ADDR_UNIT_CHOICE, unitChoice);
}
