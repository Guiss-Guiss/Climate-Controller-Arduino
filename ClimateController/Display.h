/*
 * Display.h
 * 
 * Management of LCD display, text strings, and user interface elements.
 * Handles language localization and formatted text display.
 * 
 * Written by Guillaume Ste-Marie
 * GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "Config.h"
#include <RTClib.h>

// External declaration of string table for localizable text
extern const char* const string_table[] PROGMEM;

// Initialize LCD display and custom characters
void initializeDisplay();

// Display localized welcome message at startup
void displayWelcomeScreen();

// Display current sensor readings and time on the LCD
void displayCurrentValues(float temperature, float humidity, DateTime now);

// Display time elapsed since last reset (days, hours, minutes, seconds)
void displayUptime(unsigned long startTime);

// Display error message when sensors fail to provide valid readings
void displaySensorError();

// Display a confirmation message when settings are saved
void displaySavedMessage();

// Display a message in the currently selected language
void displayLocalizedMessage(const char* englishMessage, const char* frenchMessage);

// Display lighting start and end times
void displayLightingTimes();

// Create custom characters
void createCustomCharacters();

// Display a message indicating that a value has been saved
void displaySavedMessage();

// Display a message in the selected language
void displayLocalizedMessage(const char* englishMessage, const char* frenchMessage);

#endif // DISPLAY_H
