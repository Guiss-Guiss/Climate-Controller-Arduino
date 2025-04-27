/*
 * Sensors.h
 * 
 * Management of temperature and humidity sensor readings.
 * Handles sensor communication, data validation, and timing of readings.
 * 
 * Written by Guillaume Ste-Marie
 * GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
 */

#ifndef SENSORS_H
#define SENSORS_H

#include "Config.h"

// Read temperature and humidity data from DHT22 sensor and store in global variables
bool readSensorData();

// Validate sensor data by checking if any reading returned NaN
bool areSensorReadingsValid();

// Update sensor readings if the interval has elapsed since last reading
// Returns true if readings are valid, false otherwise
bool updateSensorReadings();

#endif // SENSORS_H
