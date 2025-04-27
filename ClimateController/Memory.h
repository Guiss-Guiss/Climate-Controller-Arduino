/*
 * Memory.h
 * 
 * Management of EEPROM memory and persistent user settings.
 * Handles loading and saving of configuration parameters to non-volatile memory.
 * 
 * Written by Guillaume Ste-Marie
 * GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
 */

#ifndef MEMORY_H
#define MEMORY_H

#include "Config.h"

// EEPROM memory address map for all persistent settings
// Each address is offset by 4 bytes to accommodate different variable types
enum EepromAddresses {
  // User interface settings
  ADDR_LANG_CHOICE = 0,     // Language selection (0=French, 1=English)
  ADDR_UNIT_CHOICE = 36,    // Temperature unit (0=Celsius, 1=Fahrenheit)
  
  // Temperature thresholds in both units
  ADDR_LOW_TEMP_C = 4,      // Minimum temperature in Celsius
  ADDR_HIGH_TEMP_C = 8,     // Maximum temperature in Celsius
  ADDR_LOW_TEMP_F = 12,     // Minimum temperature in Fahrenheit
  ADDR_HIGH_TEMP_F = 16,    // Maximum temperature in Fahrenheit
  
  // Humidity thresholds
  ADDR_LOW_HUMIDITY = 20,   // Minimum humidity threshold (%)
  ADDR_HIGH_HUMIDITY = 24,  // Maximum humidity threshold (%)
  
  // Light timer settings
  ADDR_START_HOUR = 28,     // Hour when lights turn on
  ADDR_START_MINUTE = 40,   // Minute when lights turn on
  ADDR_STOP_HOUR = 32,      // Hour when lights turn off
  ADDR_STOP_MINUTE = 44     // Minute when lights turn off
};

// Main EEPROM functions to load and save all settings at once
void loadAllSettings();  // Load all settings from EEPROM during initialization
void saveAllSettings();  // Save all settings to EEPROM (rarely used - individual saves preferred)

// Specialized functions for managing user interface preferences
void loadLanguageChoice();  // Load language setting (English/French) from EEPROM
void saveLanguageChoice();  // Save current language setting to EEPROM
void loadUnitChoice();      // Load temperature unit (Celsius/Fahrenheit) from EEPROM
void saveUnitChoice();      // Save current temperature unit to EEPROM

// Specialized functions for managing temperature settings
void loadTemperatureSettings();  // Load min/max temperature thresholds from EEPROM
void saveTemperatureSettings();  // Save current temperature thresholds to EEPROM

// Specialized functions for managing humidity settings
void loadHumiditySettings();  // Load humidity thresholds from EEPROM
void saveHumiditySettings();  // Save current humidity thresholds to EEPROM

// Specialized functions for managing light timer settings
void loadTimeSettings();  // Load lighting schedule from EEPROM
void saveTimeSettings();  // Save current lighting schedule to EEPROM

#endif // MEMORY_H
