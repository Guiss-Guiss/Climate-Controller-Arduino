/*
 * MenuSystem.h
 * 
 * Menu system and navigation management.
 * 
 * Written by Guillaume Ste-Marie
 * GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
 */

#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include "Config.h"

// Menu enumeration for clarity
enum MenuState {
  MENU_MAIN = 0,
  MENU_LANGUAGE = 1,
  MENU_SET_HOUR = 2,
  MENU_SET_MINUTES = 3,
  MENU_SAVING = 4,
  MENU_SET_UNITS = 5,
  MENU_SET_LOW_TEMP = 6,
  MENU_SET_HIGH_TEMP = 7,
  MENU_SET_LOW_HUMIDITY = 8,
  MENU_SET_HIGH_HUMIDITY = 9,
  MENU_SET_START_HOUR = 10,
  MENU_SET_START_MINUTE = 11,
  MENU_SET_STOP_HOUR = 12,
  MENU_SET_STOP_MINUTE = 13
};

// Initialize the menu system
void initializeMenuSystem();

// Update button states
void updateButtons();

// Check button interactions on the main screen
void checkMainMenuButtons();

// Handle the current screen based on the selected menu
void handleMenuState();

// Generic menu helper functions
typedef enum {
  VALUE_TYPE_HOUR,
  VALUE_TYPE_MINUTE,
  VALUE_TYPE_TEMP_C,
  VALUE_TYPE_TEMP_F,
  VALUE_TYPE_HUMIDITY,
  VALUE_TYPE_BOOLEAN
} ValueType;

// Generic function to handle numeric value menus (temperature, humidity, time)
void handleGenericValueMenu(
  int& value,             // Value to modify
  int minValue,           // Minimum allowed value
  int maxValue,           // Maximum allowed value
  ValueType valueType,    // Type of the value being edited
  int titleStringIndex,   // String table index for menu title
  int valueStringIndex,   // String table index for value label
  int nextMenu,           // Next menu to navigate to when set is pressed
  void (*saveFunction)()  // Function to call to save the value
);

// Functions to handle different menus
void showMainMenu();
void showLanguageMenu();
void showHourMenu();
void showMinutesMenu();
void showSavingMenu();
void showUnitsMenu();
void showLowTempMenu();
void showHighTempMenu();
void showLowHumidityMenu();
void showHighHumidityMenu();
void showStartHourMenu();
void showStartMinuteMenu();
void showStopHourMenu();
void showStopMinuteMenu();

#endif // MENUSYSTEM_H
