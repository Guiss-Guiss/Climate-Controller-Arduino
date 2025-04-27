/*
 * ClimateController.ino
 * 
 * Main program file for the Climate Controller system.
 * Controls temperature, humidity, and lighting in an enclosed environment.
 * Modularized version with optimized memory usage and response time.
 * 
 * Written by Guillaume Ste-Marie
 * GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
 */

// Include modular components in dependency order
#include "Config.h"     // Global configuration and variables
#include "Memory.h"     // EEPROM storage management
#include "Sensors.h"    // Temperature and humidity sensing
#include "Controls.h"   // Relay control for climate systems
#include "Display.h"    // LCD interface and text management
#include "MenuSystem.h" // User interface and menu navigation

/**
 * System initialization function - runs once at startup
 * Sets up all hardware components and loads saved settings
 */
void setup() {
  // Initialize Serial communication for debugging
  Serial.begin(9600);
  Serial.println(F("Climate Controller initializing..."));
  
  // Initialize hardware interfaces in proper sequence
  initializeDisplay();      // Setup LCD display (20x4 character display)
  initializeMenuSystem();   // Configure button inputs and menu structures
  initializeRelays();       // Configure output pins for relay control
  dht.begin();              // Start DHT temperature/humidity sensor
  
  // Initialize and verify real-time clock
  if (!rtc.begin()) {
    // Critical error - system cannot function without time tracking
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Error: RTC not found"));
    lcd.setCursor(0, 1);
    lcd.print(F("Check connections"));
    // Halt system until reset
    while (1) { delay(100); }
  }
  
  // Update RTC time if power was lost
  if (rtc.lostPower()) {
    // Use compilation timestamp as fallback time source
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println(F("RTC power loss detected - time reset"));
  }
  
  // Load all user settings from EEPROM memory
  loadAllSettings();
  
  // Show welcome message
  displayWelcomeScreen();
  
  Serial.println(F("Initialization complete."));
}

/**
 * Main program loop - runs continuously after setup
 * Manages menu system, sensor readings, and system control
 */
void loop() {
  // Process current menu state and user interactions
  // This function handles all UI logic and system control
  handleMenuState();
  
  // Note: All sensor reading and control logic is called from
  // within handleMenuState() as needed based on timing intervals
}
