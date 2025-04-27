/*
 * MenuSystem.cpp
 * 
 * Implementation of menu system functions.
 * 
 * Written by Guillaume Ste-Marie
 * GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
 */

#include "MenuSystem.h"
#include "Display.h"
#include "Memory.h"
#include "Sensors.h"
#include "Controls.h"

// Initialize the menu system
void initializeMenuSystem() {
  // Configure buttons as inputs with internal pull-up resistors
  pinMode(setswitch, INPUT_PULLUP);
  pinMode(upswitch, INPUT_PULLUP);
  pinMode(downswitch, INPUT_PULLUP);
  pinMode(leftswitch, INPUT_PULLUP);
  pinMode(rightswitch, INPUT_PULLUP);
  
  // Attach Bounce objects to button pins
  setbutton.attach(setswitch);
  upbutton.attach(upswitch);
  downbutton.attach(downswitch);
  leftbutton.attach(leftswitch);
  rightbutton.attach(rightswitch);
  
  // Configure debounce time for each button
  setbutton.interval(DEBOUNCE_INTERVAL);
  upbutton.interval(DEBOUNCE_INTERVAL);
  downbutton.interval(DEBOUNCE_INTERVAL);
  leftbutton.interval(DEBOUNCE_INTERVAL);
  rightbutton.interval(DEBOUNCE_INTERVAL);
}

// Update button states
void updateButtons() {
  setbutton.update();
  upbutton.update();
  downbutton.update();
  leftbutton.update();
  rightbutton.update();
}

// Handle navigation from main menu when buttons are pressed
void checkMainMenuButtons() {
  updateButtons();
  
  // Check all navigation buttons and set appropriate menu
  bool buttonPressed = false;
  MenuState newMenu = MENU_MAIN;
  
  if (upbutton.fell()) {
    newMenu = MENU_LANGUAGE;
    buttonPressed = true;
  }
  else if (rightbutton.fell()) {
    newMenu = MENU_SET_HOUR;
    buttonPressed = true;
  }
  else if (downbutton.fell()) {
    newMenu = MENU_SET_UNITS;
    buttonPressed = true;
  }
  else if (leftbutton.fell()) {
    newMenu = MENU_SET_LOW_TEMP;
    buttonPressed = true;
  }
  
  // Update menu state if a button was pressed
  if (buttonPressed) {
    menu = newMenu;
    hasrun = false;
  }
}

// Forward declarations of all menu handler functions
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

// Array of menu handling function pointers to replace switch statement
typedef void (*MenuHandler)();
const MenuHandler menuHandlers[] = {
  showMainMenu,             // MENU_MAIN = 0
  showLanguageMenu,         // MENU_LANGUAGE = 1
  showHourMenu,             // MENU_SET_HOUR = 2
  showMinutesMenu,          // MENU_SET_MINUTES = 3
  showSavingMenu,           // MENU_SAVING = 4
  showUnitsMenu,            // MENU_SET_UNITS = 5
  showLowTempMenu,          // MENU_SET_LOW_TEMP = 6
  showHighTempMenu,         // MENU_SET_HIGH_TEMP = 7
  showLowHumidityMenu,      // MENU_SET_LOW_HUMIDITY = 8
  showHighHumidityMenu,     // MENU_SET_HIGH_HUMIDITY = 9
  showStartHourMenu,        // MENU_SET_START_HOUR = 10
  showStartMinuteMenu,      // MENU_SET_START_MINUTE = 11
  showStopHourMenu,         // MENU_SET_STOP_HOUR = 12
  showStopMinuteMenu        // MENU_SET_STOP_MINUTE = 13
};

// Handle the current screen based on the selected menu
void handleMenuState() {
  // Validate menu value to prevent array access errors
  if (menu >= 0 && menu < sizeof(menuHandlers) / sizeof(MenuHandler)) {
    // Call the appropriate menu handler function
    menuHandlers[menu]();
  } else {
    // Fallback to main menu if invalid menu value
    menu = MENU_MAIN;
    showMainMenu();
  }
}

// Implementation of the generic menu function to reduce redundancy
void handleGenericValueMenu(
  int& value,             // Value to modify
  int minValue,           // Minimum allowed value
  int maxValue,           // Maximum allowed value
  ValueType valueType,    // Type of the value being edited
  int titleStringIndex,   // String table index for menu title
  int valueStringIndex,   // String table index for value label
  int nextMenu,           // Next menu to navigate to when set is pressed
  void (*saveFunction)()  // Function to call to save the value
) {
  if (hasrun == false) {
    // Initialize menu state
    hasrun = true;
    lcd.clear();
  }
  
  updateButtons();
  
  // Check if enough time has passed since last button press
  bool canProcessButton = (millis() - lastDebounceTime > BUTTON_INTERVAL);
  
  // Handle up button - increment value (with wraparound)
  if (upbutton.read() == LOW && canProcessButton) {
    value = (value == maxValue) ? minValue : value + 1;
    lastDebounceTime = millis();
    lcd.clear();
  }
  
  // Handle down button - decrement value (with wraparound) 
  if (downbutton.read() == LOW && canProcessButton) {
    value = (value == minValue) ? maxValue : value - 1;
    lastDebounceTime = millis();
    lcd.clear();
  }
  
  // Handle set button - save value and move to next menu
  if (setbutton.fell()) {
    saveFunction();
    menu = nextMenu;
    hasrun = false;
    if (nextMenu == MENU_MAIN) {
      displaySavedMessage();
    }
  }
  
  // Display title
  lcd.setCursor(0, 0);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? titleStringIndex : titleStringIndex + 1])));
  lcd.print(buffer);
  
  // Display up/down instructions
  lcd.setCursor(0, 1);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 23 : 24])));
  lcd.print(buffer);
  lcd.write(1); // Up arrow
  
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 21 : 22])));
  lcd.print(buffer);
  lcd.write(2); // Down arrow
  
  // Display value
  lcd.setCursor(0, 2);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? valueStringIndex : valueStringIndex + 1])));
  lcd.print(buffer);
  
  // Display appropriate formatting based on value type
  switch(valueType) {
    case VALUE_TYPE_TEMP_C:
      lcd.print(value);
      lcd.write(0); // Degree symbol
      lcd.print("C");
      break;
    case VALUE_TYPE_TEMP_F:
      lcd.print(value);
      lcd.write(0); // Degree symbol
      lcd.print("F");
      break;
    case VALUE_TYPE_HUMIDITY:
      lcd.print(value);
      lcd.print(" %");
      break;
    case VALUE_TYPE_HOUR:
    case VALUE_TYPE_MINUTE:
    default:
      lcd.print(value);
      break;
  }
  
  // Display save instructions
  lcd.setCursor(0, 3);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 26 : 27])));
  lcd.print(buffer);
}

// Functions to handle different menus
void showMainMenu() {
  if (hasrun == false) {
    loadAllSettings();
    hasrun = true;
    lcd.clear();
  }
  
  checkMainMenuButtons();
  
  // Update sensors and controls
  if (updateSensorReadings()) {
    updateAllControls();
    
    // Display current values
    DateTime now = rtc.now();
    displayCurrentValues(unitChoice ? temperatureFahrenheit : temperatureCelcius, humidity, now);
    displayUptime(0);
  } else {
    displaySensorError();
  }
}

// Function to display the language selection menu
void showLanguageMenu() {
  // Initialization
  if (hasrun == false) {
    loadLanguageChoice();
    hasrun = true;
    lcd.clear();
  }
  
  updateButtons();
  
  // Toggle between languages with up/down buttons
  if (upbutton.fell() || downbutton.fell()) {
    langChoice = !langChoice;
    lcd.clear();
  }
  
  // Save choice with set button
  if (setbutton.fell()) {
    saveLanguageChoice();
    hasrun = false;
    menu = MENU_MAIN;
    displaySavedMessage();
    return;
  }
  
  // Display title
  lcd.setCursor(0, 0);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 2 : 3])));
  lcd.print(buffer);
  
  // Display instructions
  lcd.setCursor(0, 1);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 4 : 5])));
  lcd.print(buffer);
  lcd.write(1);  // Up arrow
  
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 21 : 22])));
  lcd.print(buffer);
  lcd.write(2);  // Down arrow
  
  // Display current language
  lcd.setCursor(0, 2);
  lcd.print(langChoice ? F("English") : F("Francais"));
  
  // Display save instructions
  lcd.setCursor(0, 3);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 26 : 27])));
  lcd.print(buffer);
}


// Custom save function for hour that adjusts the RTC
void saveHourSetting() {
  DateTime now = rtc.now();
  rtc.adjust(DateTime(now.year(), now.month(), now.day(), hourValue, now.minute(), now.second()));
}

// Custom load function for hour that reads from RTC
void loadHourSetting() {
  loadAllSettings();
  hourValue = rtc.now().hour();
}

void showHourMenu() {
  // Use generic menu function for hour setting
  handleGenericValueMenu(
    hourValue,           // Value to modify
    0,                  // Minimum value
    23,                 // Maximum value
    VALUE_TYPE_HOUR,    // Type of value
    6,                  // Title string index
    8,                  // Value label string index
    MENU_SET_MINUTES,   // Next menu
    saveHourSetting     // Save function
  );
}

// Custom save function for minute that adjusts the RTC
void saveMinuteSetting() {
  DateTime now = rtc.now();
  rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), minuteValue, now.second()));
}

// Custom load function for minute that reads from RTC
void loadMinuteSetting() {
  loadAllSettings();
  minuteValue = rtc.now().minute();
}

void showMinutesMenu() {
  // Use generic menu function for minute setting
  handleGenericValueMenu(
    minuteValue,         // Value to modify
    0,                  // Minimum value
    59,                 // Maximum value
    VALUE_TYPE_MINUTE,  // Type of value
    6,                  // Title string index
    30,                 // Value label string index
    MENU_SAVING,        // Next menu
    saveMinuteSetting   // Save function
  );
}

// Display saving message and return to main menu
void showSavingMenu() {
  lcd.clear();
  saveAllSettings();
  
  // Display the appropriate saving message based on language
  lcd.setCursor(0, 1);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 28 : 29])));
  lcd.print(buffer);
  
  // Show message briefly before returning to main menu
  delay(1000);
  lcd.clear();
  hasrun = false;
  menu = MENU_MAIN;
}

// Optimized function to select temperature units
void showUnitsMenu() {
  // Initialization
  if (hasrun == false) {
    loadUnitChoice();
    hasrun = true;
    lcd.clear();
  }
  
  updateButtons();
  
  // Toggle between Celsius and Fahrenheit with up/down buttons
  if (upbutton.fell() || downbutton.fell()) {
    unitChoice = !unitChoice;
    lcd.clear();
  }
  
  // Save choice with set button
  if (setbutton.fell()) {
    saveUnitChoice();
    menu = MENU_MAIN;
    hasrun = false;
    displaySavedMessage();
    return;
  }
  
  // Display title
  lcd.setCursor(0, 0);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 32 : 33])));
  lcd.print(buffer);
  
  // Display instructions
  lcd.setCursor(0, 1);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 23 : 24])));
  lcd.print(buffer);
  lcd.write(1);  // Up arrow
  
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 21 : 22])));
  lcd.print(buffer);
  lcd.write(2);  // Down arrow
  
  // Display current unit (Celsius or Fahrenheit)
  lcd.setCursor(0, 2);
  // Different indices based on language and unit
  int stringIndex = 54 + (unitChoice * 2) + (langChoice ? 0 : 1);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[stringIndex])));
  lcd.print(buffer);
  
  // Display save instructions
  lcd.setCursor(0, 3);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 26 : 27])));
  lcd.print(buffer);
}

// Wrapper function to load temperature settings
void loadTempSettingsWrapper() {
  loadTemperatureSettings();
}

void showLowTempMenu() {
  // Check temperature limits
  bool limitError = (unitChoice == 0 && lowTempCelcius >= highTempCelcius) || 
                    (unitChoice == 1 && lowTempFahrenheit >= highTempFahrenheit);
                    
  if (limitError) {
    // Display error if min temp >= max temp
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(" Min >= Max = Error ");
    delay(2000);
    
    // Fix the value - reduce low temp by 1 from high temp
    if (unitChoice == 0) {
      lowTempCelcius = highTempCelcius - 1;
    } else {
      lowTempFahrenheit = highTempFahrenheit - 1;
    }
    lcd.clear();
    hasrun = false;
  }
  
  // Select correct parameters based on temperature unit
  int& value = unitChoice ? lowTempFahrenheit : lowTempCelcius;
  int minValue = unitChoice ? 32 : 0;
  int maxValue = unitChoice ? 122 : 50;
  ValueType valType = unitChoice ? VALUE_TYPE_TEMP_F : VALUE_TYPE_TEMP_C;
  
  // Use generic menu function
  handleGenericValueMenu(
    value,                 // Value to modify
    minValue,              // Minimum value
    maxValue,              // Maximum value
    valType,               // Type of value
    40,                    // Title string index
    18,                    // Value label string index
    MENU_SET_HIGH_TEMP,    // Next menu
    saveTemperatureSettings // Save function
  );
}

void showHighTempMenu() {
  // Check temperature limits
  bool limitError = (unitChoice == 0 && lowTempCelcius >= highTempCelcius) || 
                    (unitChoice == 1 && lowTempFahrenheit >= highTempFahrenheit);
                    
  if (limitError) {
    // Display error if max temp <= min temp
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(" Max <= Min = Error ");
    delay(2000);
    
    // Fix the value - increase high temp by 1 from low temp
    if (unitChoice == 0) {
      highTempCelcius = lowTempCelcius + 1;
    } else {
      highTempFahrenheit = lowTempFahrenheit + 1;
    }
    lcd.clear();
    hasrun = false;
  }
  
  // Select correct parameters based on temperature unit
  int& value = unitChoice ? highTempFahrenheit : highTempCelcius;
  int minValue = unitChoice ? 32 : 0;
  int maxValue = unitChoice ? 122 : 50;
  ValueType valType = unitChoice ? VALUE_TYPE_TEMP_F : VALUE_TYPE_TEMP_C;
  
  // Use generic menu function
  handleGenericValueMenu(
    value,                  // Value to modify
    minValue,               // Minimum value
    maxValue,               // Maximum value
    valType,                // Type of value
    42,                     // Title string index
    20,                     // Value label string index
    MENU_SET_LOW_HUMIDITY,  // Next menu
    saveTemperatureSettings // Save function
  );
}

// Wrapper function to load humidity settings
void loadHumiditySettingsWrapper() {
  loadHumiditySettings();
}

void showLowHumidityMenu() {
  // Check humidity limits
  bool limitError = lowHumidity >= highHumidity;
                    
  if (limitError) {
    // Display error if min humidity >= max humidity
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(" Min >= Max = Error ");
    delay(2000);
    
    // Fix the value - reduce low humidity by 1 from high humidity
    lowHumidity = highHumidity - 1;
    lcd.clear();
    hasrun = false;
  }
  
  // Use generic menu function with standard parameters
  handleGenericValueMenu(
    lowHumidity,           // Value to modify
    0,                     // Minimum value
    100,                   // Maximum value
    VALUE_TYPE_HUMIDITY,   // Type of value
    38,                    // Title string index
    14,                    // Value label string index
    MENU_SET_HIGH_HUMIDITY, // Next menu
    saveHumiditySettings   // Save function
  );
}

void showHighHumidityMenu() {
  // Check humidity limits
  bool limitError = lowHumidity >= highHumidity;
  
  if (limitError) {
    // Display error if min humidity >= max humidity
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(" Min >= Max = Error ");
    delay(2000);
    
    // Fix the value - increase high humidity by 1 from low humidity
    highHumidity = lowHumidity + 1;
    lcd.clear();
    hasrun = false;
  }
  
  // Use generic menu function with standard parameters
  handleGenericValueMenu(
    highHumidity,          // Value to modify
    0,                     // Minimum value
    100,                   // Maximum value
    VALUE_TYPE_HUMIDITY,   // Type of value
    34,                    // Title string index
    12,                    // Value label string index
    MENU_SET_START_HOUR,   // Next menu
    saveHumiditySettings   // Save function
  );
}

// Wrapper function for loading time settings
void loadTimeSettingsWrapper() {
  loadTimeSettings();
}

// Display start hour settings menu for light timer
void showStartHourMenu() {
  // Use generic menu function with standard hour parameters
  handleGenericValueMenu(
    startHour,              // Value to modify (light ON hour)
    0,                      // Minimum value (24-hour format)
    23,                     // Maximum value (24-hour format)
    VALUE_TYPE_HOUR,        // Type of value (for formatting)
    46,                     // Title string index
    42,                     // Value label string index
    MENU_SET_START_MINUTE,  // Next menu - go to start minute setting
    saveTimeSettings        // Save function for all timing settings
  );
}

// Display start minute settings menu for light timer
void showStartMinuteMenu() {
  // Use generic menu function with standard minute parameters
  handleGenericValueMenu(
    startMinute,           // Value to modify (light ON minute)
    0,                     // Minimum value 
    59,                    // Maximum value
    VALUE_TYPE_MINUTE,     // Type of value (for formatting)
    0,                     // Title string index
    50,                    // Value label string index
    MENU_SET_STOP_HOUR,    // Next menu - proceed to stop hour setting
    saveTimeSettings       // Save function for all timing settings
  );
}

// Display stop hour settings menu for light timer
void showStopHourMenu() {
  // Use generic menu function with standard hour parameters
  handleGenericValueMenu(
    stopHour,              // Value to modify (light OFF hour)
    0,                     // Minimum value (24-hour format)
    23,                    // Maximum value (24-hour format)
    VALUE_TYPE_HOUR,       // Type of value (for formatting)
    48,                    // Title string index
    44,                    // Value label string index
    MENU_SET_STOP_MINUTE,  // Next menu - go to stop minute setting
    saveTimeSettings       // Save function for all timing settings
  );
}

// Display stop minute settings menu for light timer
void showStopMinuteMenu() {
  // Use generic menu function with standard minute parameters
  handleGenericValueMenu(
    stopMinute,           // Value to modify (light OFF minute)
    0,                    // Minimum value
    59,                   // Maximum value
    VALUE_TYPE_MINUTE,    // Type of value (for formatting)
    1,                    // Title string index
    52,                   // Value label string index
    MENU_MAIN,            // Next menu - return to main menu (end of sequence)
    saveTimeSettings      // Save function for all timing settings
  );
}
