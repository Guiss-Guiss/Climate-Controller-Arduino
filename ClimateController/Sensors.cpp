/*
 * Sensors.cpp
 * 
 * Implementation of sensor management functions.
 * 
 * Written by Guillaume Ste-Marie
 * GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
 */

#include "Sensors.h"

// Read temperature and humidity data from DHT22 sensor and store in global variables
bool readSensorData() {
  // Read all values in one batch to minimize sensor communication overhead
  // This is more efficient than making separate calls
  humidity = dht.readHumidity();
  temperatureCelcius = dht.readTemperature(false);  // Explicit parameter for clarity
  temperatureFahrenheit = dht.readTemperature(true);
  
  // Check if any reading failed and return status
  return areSensorReadingsValid();
}

// Validate sensor data by checking if any reading returned NaN
bool areSensorReadingsValid() {
  // All three readings must be non-NaN values to be considered valid
  return !isnan(humidity) && !isnan(temperatureCelcius) && !isnan(temperatureFahrenheit);
}

// Update sensor readings if the interval has elapsed since last reading
bool updateSensorReadings() {
  unsigned long currentTime = millis();
  
  // Handle millis() overflow - automatically reset timers when threshold is reached
  // This prevents issues after ~49 days of continuous operation
  if (currentTime > RESET_THRESHOLD) {
    previousSensorTime = 0;
    currentTime = 0;
  }
  
  // Use elapsed time logic to determine if update is needed
  // This handles cases where millis() has wrapped around
  unsigned long elapsedTime = currentTime - previousSensorTime;
  if (elapsedTime >= LOOP_INTERVAL) {
    previousSensorTime = currentTime;
    return readSensorData();
  }
  
  // If not time to update yet, return current validity state without re-reading
  return areSensorReadingsValid();
}
