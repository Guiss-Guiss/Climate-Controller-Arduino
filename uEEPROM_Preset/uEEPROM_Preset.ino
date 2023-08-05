#include "Arduino.h"
#include "Wire.h"
#include "uEEPROMLib.h"

// uEEPROMLib eeprom;
uEEPROMLib eeprom(0x57);
void setup() {

  int langChoice = 0;
  int lowTemp_C = 15;
  int highTemp_C = 25;
  int lowTemp_F = 59;
  int highTemp_F = 77;
  int lowHumidity = 40;
  int highHumidity = 60;
  int startTime = 6;
  int stopTime = 20;
  int unitChoice = 0;
  Serial.begin(9600);
  delay(2500);
  Wire.begin();
  eeprom.eeprom_write(0, langChoice);
  eeprom.eeprom_write(2, lowTemp_C);
  eeprom.eeprom_write(4, highTemp_C);
  eeprom.eeprom_write(6, lowTemp_F);
  eeprom.eeprom_write(8, highTemp_F);
  eeprom.eeprom_write(10, lowHumidity);
  eeprom.eeprom_write(12, highHumidity);
  eeprom.eeprom_write(14, startTime);
  eeprom.eeprom_write(16, stopTime);
  eeprom.eeprom_write(18, unitChoice);
  eeprom.eeprom_read(0, &langChoice);
  eeprom.eeprom_read(2, &lowTemp_C);
  eeprom.eeprom_read(4, &highTemp_C);
  eeprom.eeprom_read(6, &lowTemp_F);
  eeprom.eeprom_read(8, &highTemp_F);
  eeprom.eeprom_read(10, &lowHumidity);
  eeprom.eeprom_read(12, &highHumidity);
  eeprom.eeprom_read(14, &startTime);
  eeprom.eeprom_read(16, &stopTime);
  eeprom.eeprom_read(18, &unitChoice);
  Serial.println(langChoice);
  Serial.println(lowTemp_C);
  Serial.println(highTemp_C);
  Serial.println(lowTemp_F);
  Serial.println(highTemp_F);
  Serial.println(lowHumidity);
  Serial.println(highHumidity);
  Serial.println(startTime);
  Serial.println(stopTime);
  Serial.println(unitChoice);
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
}