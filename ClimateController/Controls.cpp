/*
 * Controls.cpp
 * 
 * Implementation of relay control functions.
 * 
 * Written by Guillaume Ste-Marie
 * GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
 */

#include "Controls.h"

// Set up relay pins as outputs and ensure all relays are off at startup
void initializeRelays() {
  // Configure all relay control pins as outputs
  pinMode(heat, OUTPUT);       // Heating relay
  pinMode(cool, OUTPUT);       // Cooling relay
  pinMode(humLow, OUTPUT);     // Humidifier relay
  pinMode(humHigh, OUTPUT);    // Dehumidifier relay
  pinMode(lights, OUTPUT);     // Lighting relay
  
  // Initialize all relays to OFF state (LOW signal)
  digitalWrite(heat, LOW);
  digitalWrite(cool, LOW);
  digitalWrite(humLow, LOW);
  digitalWrite(humHigh, LOW);
  digitalWrite(lights, LOW);
}

// Apply control logic to all relays based on current sensor readings and time
void updateRelays(float temperature, float humidity, DateTime now) {
  // Update all relay states using their respective control functions
  controlHeater(temperature);        // Manage heating system
  controlCooler(temperature);        // Manage cooling system
  controlHumidifier(humidity);       // Manage humidifier 
  controlDehumidifier(humidity);     // Manage dehumidifier
  controlLights(now);                // Control lighting based on time
}

// Activate heater when temperature falls below set threshold
void controlHeater(float temperature) {
  // Select appropriate temperature threshold based on current unit setting
  float lowTemp = unitChoice ? lowTempFahrenheit : lowTempCelcius;
  
  // Turn heater ON (HIGH) when below threshold, otherwise OFF (LOW)
  digitalWrite(heat, temperature < lowTemp ? HIGH : LOW);
}

// Activate cooling system when temperature rises above set threshold
void controlCooler(float temperature) {
  // Select appropriate temperature threshold based on current unit setting
  float highTemp = unitChoice ? highTempFahrenheit : highTempCelcius;
  
  // Turn cooling ON (HIGH) when above threshold, otherwise OFF (LOW)
  digitalWrite(cool, temperature > highTemp ? HIGH : LOW);
}

// Activate humidifier when humidity falls below minimum threshold
void controlHumidifier(float humidity) {
  // Turn humidifier ON (HIGH) when below minimum, otherwise OFF (LOW)
  digitalWrite(humLow, humidity < lowHumidity ? HIGH : LOW);
}

// Activate dehumidifier when humidity rises above maximum threshold
void controlDehumidifier(float humidity) {
  // Turn dehumidifier ON (HIGH) when above maximum, otherwise OFF (LOW)
  digitalWrite(humHigh, humidity > highHumidity ? HIGH : LOW);
}

// Convert time to an integer format (HHMM) for efficient comparison
inline int timeToInt(int hour, int minute) {
  // Convert hours and minutes to a single integer (e.g., 9:30 becomes 930)
  return (hour * 100) + minute;  // Hours * 100 + minutes gives time as integer
}

// Control lighting based on scheduled start and stop times
void controlLights(DateTime now) {
  // Convert times to integers for straightforward comparison
  int nowTime = timeToInt(now.hour(), now.minute());
  int startTime = timeToInt(startHour, startMinute);
  int stopTime = timeToInt(stopHour, stopMinute);
  
  // Handle special case where schedule spans midnight
  bool lightsOn;
  if (startTime < stopTime) {
    // Normal case: start time is before stop time (e.g., 8:00 to 20:00)
    lightsOn = (nowTime >= startTime && nowTime < stopTime);
  } else {
    // Schedule spans midnight (e.g., 20:00 to 8:00)
    lightsOn = (nowTime >= startTime || nowTime < stopTime);
  }
  
  // Use direct ternary operator for digital write
  digitalWrite(lights, lightsOn ? HIGH : LOW);
}

// Main control function that updates all control systems based on current conditions
bool updateAllControls() {
  // Get current time from real-time clock only once
  DateTime now = rtc.now();
  
  // Select correct temperature based on current unit setting
  // Using the selected unit from settings to determine which temperature to use
  float temperature = unitChoice ? temperatureFahrenheit : temperatureCelcius;
  
  // Update all relays according to current sensor readings and time
  updateRelays(temperature, humidity, now);
  
  return true; // Indicate successful control system update
}
