#include "Arduino.h"
#include "Wire.h"
#include "uEEPROMLib.h"
#include <TimeLib.h>
#include <DS1307RTC.h>

uEEPROMLib eeprom(0x57);

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

tmElements_t tm;

void setup() {

  bool parse=false;
  bool config=false;

  // get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      config = true;
    }
  }
  Serial.begin(9600);

  byte langChoice = false;
  int lowTempCelcius = 15;
  int highTempCelcius = 25;
  int lowTempFahrenheit = 59;
  int highTempFahrenheit = 77;
  int lowHumidity = 40;
  int highHumidity = 60;
  int startHour = 6;
  int stopHour = 20;
  byte unitChoice = false;
  int startMinute = 0;
  int stopMinute = 0;


  while (!Serial) ; // wait for Arduino Serial Monitor
  delay(200);
  if (parse && config) {
    Serial.print("DS3231 configured Time=");
    Serial.print(__TIME__);
    Serial.print(", Date=");
    Serial.println(__DATE__);
  } else if (parse) {
    Serial.println("DS3231 Communication Error :-{");
    Serial.println("Please check your circuitry");
  } else {
    Serial.print("Could not parse info from the compiler, Time=\"");
    Serial.print(__TIME__);
    Serial.print("\", Date=\"");
    Serial.print(__DATE__);
    Serial.println("\"");
  }

  Wire.begin();

  eeprom.eeprom_write(0, langChoice);
  eeprom.eeprom_write(4, lowTempCelcius);
  eeprom.eeprom_write(8, highTempCelcius);
  eeprom.eeprom_write(12, lowTempFahrenheit);
  eeprom.eeprom_write(16, highTempFahrenheit);
  eeprom.eeprom_write(20, lowHumidity);
  eeprom.eeprom_write(24, highHumidity);
  eeprom.eeprom_write(28, startHour);
  eeprom.eeprom_write(32, stopHour);
  eeprom.eeprom_write(36, unitChoice);
  eeprom.eeprom_write(40, startMinute);
  eeprom.eeprom_write(44, stopMinute);

  eeprom.eeprom_read(0, &langChoice);
  eeprom.eeprom_read(4, &lowTempCelcius);
  eeprom.eeprom_read(8, &highTempCelcius);
  eeprom.eeprom_read(12, &lowTempFahrenheit);
  eeprom.eeprom_read(16, &highTempFahrenheit);
  eeprom.eeprom_read(20, &lowHumidity);
  eeprom.eeprom_read(24, &highHumidity);
  eeprom.eeprom_read(28, &startHour);
  eeprom.eeprom_read(32, &stopHour);
  eeprom.eeprom_read(36, &unitChoice);
  eeprom.eeprom_read(40, &startMinute);
  eeprom.eeprom_read(44, &stopMinute);


  Serial.println(langChoice);
  Serial.println(lowTempCelcius);
  Serial.println(highTempCelcius);
  Serial.println(lowTempFahrenheit);
  Serial.println(highTempFahrenheit);
  Serial.println(lowHumidity);
  Serial.println(highHumidity);
  Serial.println(startHour);
  Serial.println(stopHour);
  Serial.println(unitChoice);
  Serial.println(startMinute);
  Serial.println(stopMinute);


  delay(5000);
}

void loop(){}

bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}