/*
 * Controls.h
 * 
 * Management of relays and climate control logic for heating, cooling,
 * humidity control, and lighting based on sensor readings and user settings.
 * 
 * Written by Guillaume Ste-Marie
 * GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
 */

#ifndef CONTROLS_H
#define CONTROLS_H

#include "Config.h"
#include <RTClib.h>

// Initialize relay pins
void initializeRelays();

// Update all relay states based on current sensor values and thresholds
void updateRelays(float temperature, float humidity, DateTime now);

// Individual climate control functions
void controlHeater(float temperature);     // Manage heating system based on temperature
void controlCooler(float temperature);     // Manage cooling system based on temperature
void controlHumidifier(float humidity);    // Manage humidifier based on humidity level
void controlDehumidifier(float humidity);  // Manage dehumidifier based on humidity level
void controlLights(DateTime now);          // Control lighting based on time schedule

// Convert time to an integer format (HHMM) for efficient comparison
inline int timeToInt(int hour, int minute);

// Update all relays
bool updateAllControls();

#endif // CONTROLS_H
