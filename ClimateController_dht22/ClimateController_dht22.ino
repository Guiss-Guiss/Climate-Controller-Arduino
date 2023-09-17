

/*
Written by Guillaume Ste-Marie
GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
This sketch allows to control the climate in a room, a greenhouse, a grow tent,
an aquariun, a vivarium, or any other enclosure. It can also be used to control
the fermentation temperature of beer or wine. It can be used to control your world.
It is using a variety of sensing and controlling devices.
It sets up the necessary pins, libraries, and variables to read the
temperature, humidity, and control the relays to power the heater,
exhaust fan or cooling, humidifier, dehumidifier, and lights.
The user can access a menu to set the desired temperature, humidity,
and lighting times. It uses the EEPROM memory of the DS3231 RTC to store the settings,
a DS3231 RTC module to keep track of time, a dht22 temperature and humidity sensor
to read temperature and relative humidity. It uses a 20x4 LCD screen to display the current time.
It uses 5 momentary switches to navigate the menu and set the desired values.
It uses 5 relays to control the heater, exhaust fan or cooling, humidifier, dehumidifier, and lights.
Visit https://www.guillaumeguiss.com or email at controller@guillaumeguiss.com for more information.
Tested on Arduino Uno, Arduino Mega, and Arduino Nano.
*/

// Libraries
#include <Arduino.h>           // Built-in Arduino library.
#include <OneWire.h>           // Library for OneWire communication protocol.
#include <DHT.h>               // Library for DHT sensor by Adafruit.
#include <RTClib.h>            // Library for RTC module by Adafruit.
#include <LiquidCrystal_I2C.h> // Library for I2C LCD driver by Marco Schwartz.
#include <Wire.h>              // Built-in I2C library.
#include <uEEPROMLib.h>        // Library for EEPROM memory by Naguissa.
#include <Bounce2.h>           // Library for debouncing switches by Thomas O Fredericks.

// Defines
#define DHTPIN 7        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Addressing
uEEPROMLib ueeprom(0x57);                   // EEPROM memory address.
RTC_DS3231 rtc;                             // Real Time Clock address.
LiquidCrystal_I2C lcd(0x27, 20, 4);         // I2C LCD address.
DHT dht(DHTPIN, DHTTYPE);                   // DHT sensor address.

// Constants
const int setswitch = 8;                    // Set" momentary switch.
const int upswitch = 9;                     // Up" momentary switch.
const int downswitch = 10;                  // Down" momentary switch.
const int leftswitch = 11;                  // Left" momentary switch.
const int rightswitch = 12;                 // Right" momentary switch.
const int heat = 2;                         // Output pin for heater relay.
const int cool = 3;                         // Output pin for Exhaust fan relay.
const int humLow = 4;                       // Output pin for humidifier relay.
const int humHigh = 5;                      // Output pin for dehumidifier relay.
const int lights = 6;                       // Output pin for lighting relay.

const unsigned long loopInterval = 10000;   // Interval for reading temperature and humidity.
const unsigned long buttonInterval = 500;   // Interval for reading buttons.

// Variables
bool  langChoice ;            // Variable to store the language choice.
bool  unitChoice;             // Variable to store the unit choice.  
bool  langChoiceState;        // Variable to store the language choice state.
bool  unitChoiceState;        // Variable to store the unit choice state.
bool  hasrun = false;         // Variable to store if the program has run.

float humidity;               // Variable to store the current humidity.
float temperatureCelcius;     // Variable to store the current temperature in Celsius.
float temperatureFahrenheit;  // Variable to store the current temperature in Fahrenheit.

int   lowTempCelcius;         // Variable to store the low temperature in Celsius.
int   highTempCelcius;        // Variable to store the high temperature in Celsius.
int   lowTempFahrenheit;      // Variable to store the low temperature in Fahrenheit.
int   highTempFahrenheit;     // Variable to store the high temperature in Fahrenheit.
int   lowHumidity;            // Variable to store the low humidity.
int   highHumidity;           // Variable to store the high humidity.
int   startHour;              // Variable to store the start time.
int   startMinute;            // Variable to store the start time.
int   stopHour;               // Variable to store the stop time.
int   stopMinute;             // Variable to store the stop time.
int   menu = 0;               // Current menu selection.
int   hourValue;              // Variable to store the hour.
int   minuteValue;            // Variable to store the minute.
int   changeValue = 0;        // Variable to store the change value.

unsigned long previousTime = -10000;  // Variable to store the previous time.
unsigned long lastDebounceTime = 0;   // Variable to store the last debounce time.

// Bounce objects
Bounce setbutton    = Bounce();   // Bounce object for "Set" switch.
Bounce upbutton     = Bounce();   // Bounce object for "Up" switch.
Bounce downbutton   = Bounce();   // Bounce object for "Down" switch.
Bounce leftbutton   = Bounce();   // Bounce object for "Left" switch.
Bounce rightbutton  = Bounce();   // Bounce object for "Right" switch.

// LCD custom characters
byte deg[8]   = {B00110, B01001, B01001, B00110, B00000, B00000, B00000, B00000};   // LCD design for degree symbol.
byte up[8]    = {B00100, B01110, B11111, B00000, B00000, B00100, B01110, B11111};   // LCD design for up arrows symbol.
byte down[8]  = {B11111, B01110, B00100, B00000, B00000, B11111, B01110, B00100};   // LCD design for down arrows symbol.

// Strings
const char a00[] PROGMEM = "To set start minute ";  // English string for "Press >> to change".
const char a01[] PROGMEM = "To set Stop minute  ";  // French string for "Press >> to change".
const char a02[] PROGMEM = " English selected   ";  // English string for "in Celsius".
const char a03[] PROGMEM = "Francais selectionne";  // French string for "in Celsius".
const char a04[] PROGMEM = "Regler minute fin   ";  // English string for "in Fahrenheit".
const char a05[] PROGMEM = "Regler minute debut ";  // French string for "in Fahrenheit".
const char a06[] PROGMEM = "       Time: ";         // English string for "Time: ".
const char a07[] PROGMEM = "      Heure: ";         // French string for "Time: ".
const char a08[] PROGMEM = "Hour = ";               // English string for "Hour = ".
const char a09[] PROGMEM = "Heure = ";              // French string for "Hour = ".
const char a10[] PROGMEM = "   Humidity: ";         // English string for "Humidity: ".
const char a11[] PROGMEM = "   Humidite: ";         // French string for "Humidity: ".
const char a12[] PROGMEM = "Max Humi = ";           // English string for "Max Humi = ".
const char a13[] PROGMEM = "Humi Max = ";           // French string for "Max Humi = ".
const char a14[] PROGMEM = "Max Temp = ";           // English string for "Max Temp = ".
const char a15[] PROGMEM = "Temp Max = ";           // French string for "Max Temp = ".
const char a16[] PROGMEM = "Min Humi = ";           // English string for "Min Humi = ".
const char a17[] PROGMEM = "Humi Min = ";           // French string for "Min Humi = ".
const char a18[] PROGMEM = "Min Temp = ";           // English string for "Min Temp = ".
const char a19[] PROGMEM = "Temp Min = ";           // French string for "Min Temp = ".
const char a20[] PROGMEM = "Minutes  = ";           // String for "Minutes  = ".   
const char a21[] PROGMEM = " or ";                  // English string for " or ".
const char a22[] PROGMEM = " ou ";                  // French string for " or ".
const char a23[] PROGMEM = "Press ";                // English string for "Press ".
const char a24[] PROGMEM = "Presser ";              // French string for "Press ".
const char a25[] PROGMEM = "! Sonde temperature ";  // English string for "  Press >> to Save  ".
const char a26[] PROGMEM = "! temperature sensor";  // French string for "  Press >> to Save  ".
const char a27[] PROGMEM = "Press Set to Save   ";  // English string for "  Press Set to Save ".
const char a28[] PROGMEM = "Sauvegarde = Set    ";  // French string for "  Press Set to Save ".
const char a29[] PROGMEM = "     Saving ...     ";  // English string for "   Saving ...   ".
const char a30[] PROGMEM = "   Sauvegarde ...   ";  // French string for "   Saving ...   ".
const char a31[] PROGMEM = "To Set Hours:       ";  // English string for "To Set Hours:       ".
const char a32[] PROGMEM = "Pour regler Heures: ";  // French string for "To Set Hours:       ".
const char a33[] PROGMEM = "To Set Max Humi:    ";  // English string for "To Set Max Humi:    ".
const char a34[] PROGMEM = "Regler Humi Max:    ";  // French string for "To Set Max Humi:    ".
const char a35[] PROGMEM = "To Set Max Temp:    ";  // English string for "To Set Max Temp:    ".
const char a36[] PROGMEM = "Ajuster Temp Max:   ";  // French string for "To Set Max Temp:    ".
const char a37[] PROGMEM = "To Set Min Humi:    ";  // English string for "To Set Min Humi:    ".
const char a38[] PROGMEM = "Regler Humi Min:    ";  // French string for "To Set Min Humi:    ".
const char a39[] PROGMEM = "To Set Min Temp:    ";  // English string for "To Set Min Temp:    ".
const char a40[] PROGMEM = "Ajuster Temp Min:   ";  // French string for "To Set Min Temp:    ".  
const char a41[] PROGMEM = "To Set Minutes:     ";  // English string for "To Set Minutes:     ".
const char a42[] PROGMEM = "Pour regler Minutes:";  // French string for "To Set Minutes:     ".
const char a43[] PROGMEM = "Lights On Time:     ";  // English string for "Lights On Time:     ".
const char a44[] PROGMEM = "Heure En Lumieres:  ";  // French string for "Lights On Time:     ".
const char a45[] PROGMEM = "Lights Off Time:   ";   // English string for "Lights Off Time:   ".
const char a46[] PROGMEM = "Heure Hors Lumieres:";  // French string for "Lights Off Time:   ". 
const char a47[] PROGMEM = "To Set Start Time:  ";  // English string for "To Set Start Time:  ".
const char a48[] PROGMEM = "Ajuster Heure Debut:";  // French string for "To Set Start Time:  ".
const char a49[] PROGMEM = "To Set Stop Time:   ";  // English string for "To Set Stop Time:   ".
const char a50[] PROGMEM = "Ajuster Heure Fin:  ";  // French string for "To Set Stop Time:   ".
const char a51[] PROGMEM = "Temperature: ";         // String for "Temperature: ".
const char a52[] PROGMEM = "  Celcius Selected  ";  // English string for "  Celcius Selected  ".
const char a53[] PROGMEM = "   Celcius Choisi   ";  // French string for "  Celcius Selected  ".
const char a54[] PROGMEM = "                    ";  // String for "                    ".
const char a55[] PROGMEM = "Fahrenheit Selected ";  // English string for " Fahrenheit Selected ".
const char a56[] PROGMEM = " Fahrenheit Choisi  ";  // French string for " Fahrenheit Selected ".

// Array of strings in program memory.
const char *const string_table[] PROGMEM = {
  a00, a01, a02, a03, a04, a05, a06, a07, a08, a09,
  a10, a11, a12, a13, a14, a15, a16, a17, a18, a19,
  a20, a21, a22, a23, a24, a25, a26, a27, a28, a29,
  a30, a31, a32, a33, a34, a35, a36, a37, a38, a39,
  a40, a41, a42, a43, a44, a45, a46, a47, a48, a49,
  a50, a51, a52, a53, a54, a55, a56};

char buffer[20];                        // Buffer for storing maximum length of strings.

void Language()
{
  lcd.clear();                            // Clears the LCD screen
  langChoice = !langChoice;               // Invert the value of langChoice
    
  ueeprom.eeprom_write(0, langChoice);  // Writes the value of langChoice to EEPROM memory at position 0

  if (langChoice == 0)
  {
    lcd.setCursor(0, 1);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[3])));  // Francais selectionne"
    lcd.print(buffer);
    lcd.setCursor(0, 2);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[30]))); // Sauvegarde ..."
    lcd.print(buffer);
  }

  else if (langChoice == 1)
  {
    lcd.setCursor(0, 1);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[2])));  // English selected"
    lcd.print(buffer);
    lcd.setCursor(0, 2);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[29]))); // Saving ..."
    lcd.print(buffer); 
  }

  delay(1000);
  lcd.clear();
  hasrun = false;
  menu = 0;
}

void Relays()
{
  if (hasrun == false)
  {
    ueeprom.eeprom_read(0, &langChoice);          // Read the value of hourValue from EEPROM memory at position 100
    ueeprom.eeprom_read(4, &lowTempCelcius);      // Read the value of lowTempCelcius from EEPROM memory at position 4
    ueeprom.eeprom_read(8, &highTempCelcius);     // Read the value of highTempCelcius from EEPROM memory at position 8
    ueeprom.eeprom_read(12, &lowTempFahrenheit);  // Read the value of lowTempFahrenheit from EEPROM memory at position 12
    ueeprom.eeprom_read(16, &highTempFahrenheit); // Read the value of highTempFahrenheit from EEPROM memory at position 16
    ueeprom.eeprom_read(20, &lowHumidity);        // Read the value of lowHumidity from EEPROM memory at position 20
    ueeprom.eeprom_read(24, &highHumidity);       // Read the value of highHumidity from EEPROM memory at position 24
    ueeprom.eeprom_read(28, &startHour);          // Read the value of startHour from EEPROM memory at position 28
    ueeprom.eeprom_read(32, &stopHour);           // Read the value of stopHour from EEPROM memory at position 32
    ueeprom.eeprom_read(36, &unitChoice);         // Read the value of unitChoice from EEPROM memory at position 36
    ueeprom.eeprom_read(40, &startMinute);        // Read the value of startMinute from EEPROM memory at position 40
    ueeprom.eeprom_read(44, &stopMinute);         // Read the value of stopMinute from EEPROM memory at position 44

    hasrun = true;
    lcd.clear();
  }
  setbutton.update();
  upbutton.update();    
  downbutton.update();
  leftbutton.update();
  rightbutton.update();

  if (upbutton.fell())            // Checks if the up switch is pressed
  {
    menu = 1;
    hasrun = false;
  }
  else if (rightbutton.fell())    // Checks if the right switch is pressed
  {
    hasrun = false;
    menu = 2;
  }
  else if (downbutton.fell())     // Checks if the down switch is pressed
  {
    menu = 5;
    hasrun = false;
  }
  else if (leftbutton.fell())     // Checks if the left switch is pressed
  {
    menu = 6;
    hasrun = false;
  }

  lcd.createChar(0, deg);                           // Creates the degree symbol
  char buf[20];                                     // Buffer for storing strings
  float humidity = dht.readHumidity();              // Read humidity
  float temperatureCelcius = dht.readTemperature(); // Read temperature as Celsius (the default)
  float temperatureFahrenheit = dht.readTemperature(true);  // Read temperature as Fahrenheit (isFahrenheit = true)
  DateTime now = rtc.now();                         // Gets the current time from the RTC module 
  char rightNow[10] = "hhmm";                       // Creates a string for the current time
  unsigned long currentTime = millis();             // Gets the current time in milliseconds
  unsigned long allSeconds = currentTime / 1000;    // Converts the current time to seconds
  int runDays = allSeconds / 86400;                 // There are 86400 seconds in a day
  allSeconds = allSeconds % 86400;                  // Remaining seconds after days are counted
  int runHours = allSeconds / 3600;                 // There are 3600 seconds in an hour
  int secsRemaining = allSeconds % 3600;            // Remaining seconds after hours are counted
  int runMinutes = secsRemaining / 60;              // There are 60 seconds in a minute
  int runSeconds = secsRemaining % 60;              // Remaining seconds after minutes are counted
  int currentHour = now.hour();                     // Get the current hour
  int currentMinute = now.minute();                 // Get the current minute
  int nowTime = (currentHour * 100) + currentMinute;  // Convert to numeric form

  if (isnan(humidity) || isnan(temperatureCelcius) || isnan(temperatureFahrenheit))   // Checks if the temperature and humidity values are not numbers
  {
    if (langChoice == 0) {
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[25]))); // Saving ..."
      lcd.print(buffer); 
      delay(2000);    // Delays the program for 2 seconds
      lcd.clear();
      return;         // Continues the program
    }
    else if (langChoice ==1) {
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[26]))); // Saving ..."
      lcd.print(buffer); 
      delay(2000);    // Delays the program for 2 seconds
      lcd.clear();
      return;         // Continues the program
    }
  }

  if (currentTime > 4000000000UL)   // Checks if the current time is greater than 46 days
  {
    void (*resetFunc)(void) = 0;    // Resets the Arduino board
    resetFunc();
  }

  lcd.setCursor(0, 0);

  if (langChoice == 1)
  {
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[6])));    // Time: "
    lcd.print(buffer);
  }
  else
  {
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[7])));    // Heure: "
    lcd.print(buffer);
  }

  if (now.hour() <= 9)      // Add leading zero if hour is less than 10 
  {
    lcd.print("0");
  }

  lcd.print(now.hour());    // Print hour
  hourValue = now.hour();   // Store hour value
  lcd.print(F(":"));        // Print colon

  if (now.minute() <= 9)    // Add leading zero if minute is less than 10
  {
    lcd.print("0");
  }

  lcd.print(now.minute());    // Print minute
  minuteValue = now.minute(); // Store minute value

  lcd.setCursor(0, 1);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[51])));   // Temperature: "
  lcd.print(buffer);

  if ((unitChoice) == 1)
  {
    lcd.print(temperatureFahrenheit);   // Print temperature in Fahrenheit
    lcd.write(0);               // Display 'degree' character
    lcd.print(F("F"));          // Print Fahrenheit symbol
  }
  else
  {
    lcd.print(temperatureCelcius);   // Print temperature in Celsius
    lcd.write(0);               // Display 'degree' character
    lcd.print(F("C"));          // Print Celsius symbol
  }

  lcd.setCursor(0, 2);

  if (langChoice == 1)    
  {
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[10])));   // Humidity: "
    lcd.print(buffer);
  }
  else
  {
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[11])));   // Humidite: "
    lcd.print(buffer);
  }

  lcd.print(humidity);    // Print humidity
  lcd.print(F(" %"));     // Print percent symbol
  lcd.setCursor(0, 3);

  if (langChoice == 1)
  {
    sprintf(buf, "Since: %02dd%02dh%02dm%02ds ", runDays, runHours, runMinutes, runSeconds);    // Print time since last reset in English
    lcd.print(buf);
  }
  else
  {
    sprintf(buf, "Depuis: %02dj%02dh%02dm%02ds ", runDays, runHours, runMinutes, runSeconds);   // Print time since last reset in French
    lcd.print(buf);
  }
  if (currentTime - previousTime >= loopInterval)    // Update relays every 10 seconds
  {
    if (unitChoice == 1)
    {
      if (temperatureFahrenheit < lowTempFahrenheit)
      {
        digitalWrite(heat, HIGH);
      }
      else
      {
        digitalWrite(heat, LOW);
      }
      if (temperatureFahrenheit > highTempFahrenheit)
      {
        digitalWrite(cool, HIGH);
      }
      else
      {
        digitalWrite(cool, LOW);
      }
      if (humidity < lowHumidity)
      {
        digitalWrite(humLow, HIGH);
      }
      else
      {
        digitalWrite(humLow, LOW);
      }
      if (humidity > highHumidity)
      {
        digitalWrite(humHigh, HIGH);
      }
      else
      {
        digitalWrite(humHigh, LOW);
      }
      if (nowTime >= ((startHour * 100) + startMinute) && (nowTime < (stopHour * 100) + stopMinute))
      {
        digitalWrite(lights, HIGH); // Turn on the light

      }
      else
      {
        digitalWrite(lights, LOW); // Turn off the light
      }
      previousTime = currentTime;
    }
    else if (unitChoice == 0)
    {
      if (temperatureCelcius < lowTempCelcius) 
      {
        digitalWrite(heat, HIGH);
      }
      else
       {
        digitalWrite(heat, LOW);
      }
      if (temperatureCelcius > highTempCelcius)
      {
        digitalWrite(cool, HIGH);
      }
      else
      {
        digitalWrite(cool, LOW);
      }
      if (humidity < lowHumidity)
      {
        digitalWrite(humLow, HIGH);
      }
      else
      {
        digitalWrite(humLow, LOW);
      }
      if (humidity > highHumidity)
      {
        digitalWrite(humHigh, HIGH);
      }
      else
      {
        digitalWrite(humHigh, LOW);
      }
      if (nowTime >= ((startHour * 100) + startMinute) && (nowTime < (stopHour * 100) + stopMinute))
      {
        digitalWrite(lights, HIGH); // Turn on the light

      }
      else
      {
        digitalWrite(lights, LOW); // Turn off the light
      }
      previousTime = currentTime;
    } 
  }
}

void SetHighHumidity()
{

  if (hasrun == false)
  {
    ueeprom.eeprom_read(0, &langChoice);   // Read the value of hourValue from EEPROM memory at position 100
    ueeprom.eeprom_read(20, &lowHumidity);  // Read the value of lowHumidity from EEPROM memory at position 20
    ueeprom.eeprom_read(24, &highHumidity); // Read the value of highHumidity from EEPROM memory at position 24
    hasrun = true;
    lcd.clear();
  }

  setbutton.update();
  upbutton.update();
  downbutton.update();

  lcd.createChar(1, up);    // Create custom character for up arrow
  lcd.createChar(2, down);  // Create custom character for down arrow


  if ((upbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    highHumidity = (highHumidity == 100) ? 0 : highHumidity + 1;    // Increase the low humidity value by 1
    lastDebounceTime = millis();
    lcd.clear();
  }
  if ((downbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    highHumidity = (highHumidity == 0) ? 100 : highHumidity - 1;    // Decrease the low humidity value by 1
    lastDebounceTime = millis();
    lcd.clear();
  }

  if (setbutton.fell())
  {
    ueeprom.eeprom_write(24, highHumidity);  // Write the high humidity to EEPROM
    lcd.clear();
    hasrun = false;   
    menu = 10;   // Switch to menu 10
  }
  if (lowHumidity < highHumidity)
  {
    if (langChoice == 1)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[33])));   // To set humidity max:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[23])));    // Press
      lcd.write(1);                                                   // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[21])));   // or
      lcd.print(buffer);
      lcd.write(2);                                                   // Display down arrow
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[12])));    // Max Humi =
      lcd.print(buffer);
      lcd.print(highHumidity);
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[27])));   // Press Set to Save"
      lcd.print(buffer);
    }
    else if (langChoice == 0)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[34])));   // Pour regler humidite max:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[24])));   // Presser"
      lcd.print(buffer);
      lcd.write(1);                                                   // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[22])));   // ou"
      lcd.print(buffer);
      lcd.write(2);                                                   // Display down arrow
      lcd.setCursor(0, 2);  
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[13])));   // Humidite max =
      lcd.print(buffer);
      lcd.print(highHumidity);
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[28])));   // Sauvegarde = Set"
      lcd.print(buffer);
    }
  }
   else
  {
    if (langChoice == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(" Max < Min = Erreur ");
      delay(1000);
      highHumidity = highHumidity + 2;
    }
    else if (langChoice == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(" Max < Min = Error  ");
      delay(1000);
      highHumidity = highHumidity + 2;
    }
  }
}

void SetHighTemp()
{
  if (hasrun == false)
  {
    ueeprom.eeprom_read(0, &langChoice);   // Read the value of hourValue from EEPROM memory at position 100
    ueeprom.eeprom_read(4, &lowTempCelcius);    // Read the value of lowTempCelcius from EEPROM memory at position 4
    ueeprom.eeprom_read(12, &lowTempFahrenheit);    // Read the value of lowTempFahrenheit from EEPROM memory at position 12
    ueeprom.eeprom_read(16, &highTempFahrenheit);   // Read the value of highTempFahrenheit from EEPROM memory at position 16
    ueeprom.eeprom_read(8, &highTempCelcius);   // Read the value of highTempCelcius from EEPROM memory at position 8
    ueeprom.eeprom_read(36, &unitChoice);  // Read the value of unitChoice from EEPROM memory at position 36
    hasrun = true;
    lcd.clear();
  }
  setbutton.update();
  upbutton.update();
  downbutton.update();

  lcd.createChar(1, up);    // Create custom character for up arrow
  lcd.createChar(2, down);  // Create custom character for down arrow

  if ((upbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    if (unitChoice == 1)
    {
      highTempFahrenheit = (highTempFahrenheit == 257) ? -67 : highTempFahrenheit + 1;    // Increase the high temperature value by 1 based on unit choice when the down switch is released
      lastDebounceTime = millis();
      lcd.clear();
    }
    else if (unitChoice == 0)
    {
      highTempCelcius = (highTempCelcius == 125) ? -55 : highTempCelcius + 1;    // Increase the high temperature value by 1 based on unit choice when the down switch is released
      lastDebounceTime = millis();
      lcd.clear();
    }
  }

  if (downbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval))
  {
    if (unitChoice == 1)
    {
      highTempFahrenheit = (highTempFahrenheit == -67) ? -257 : highTempFahrenheit - 1;     // Decrease the high temperature value by 1 based on unit choice when the down switch is released
      lastDebounceTime = millis();
      lcd.clear();
    }
    else if (unitChoice == 0)
    {
      highTempCelcius = (highTempCelcius == -55) ? 125 : highTempCelcius - 1;     // Decrease the high temperature value by 1 based on unit choice when the down switch is released
      lastDebounceTime = millis();
      lcd.clear();
    }
  }

  if (setbutton.fell())
  {
    ueeprom.eeprom_write(8, highTempCelcius);    // Write the high temperature in Celsius to EEPROM
    ueeprom.eeprom_write(16, highTempFahrenheit);    // Write the high temperature in Fahrenheit to EEPROM
    lcd.clear();
    menu = 8;   // Switch to menu 8
  }

  if (highTempCelcius > lowTempCelcius && highTempFahrenheit > lowTempFahrenheit)
  {

    if (unitChoice ==1 && langChoice ==1)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[35])));   // To set  maximum temperature:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[23])));    // Press"
      lcd.print(buffer);
      lcd.write(1);                                                   // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[21])));   // or"
      lcd.print(buffer);
      lcd.write(2);                                                   // Display down arrow
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[14])));    // Max Temp = "
      lcd.print(buffer);
      lcd.print(highTempFahrenheit);
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[27])));   // Press Set to Save"
      lcd.print(buffer);
    }
    else if (unitChoice == 0 && langChoice == 1)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[35])));   // To set  maximum temperature:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[23])));    // Press"
      lcd.print(buffer);
      lcd.write(1);                                                    // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[21])));    // or"
      lcd.print(buffer);
      lcd.write(2);                                                    // Display down arrow
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[14])));    // Max Temp = "
      lcd.print(buffer);
      lcd.print(highTempCelcius);
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[27])));   // Press Set to Save"
      lcd.print(buffer);
    }
    else if (unitChoice == 1 && langChoice ==0 )
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[36])));   // Pour regler temperature max:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[24])));   // Presser"
      lcd.print(buffer);
      lcd.write(1);                                                   // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[22])));   // ou"
      lcd.print(buffer);
      lcd.write(2);                                                   // Display down arrow
      lcd.setCursor(0, 2);  
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[15])));   // Temperature max =
      lcd.print(buffer);
      lcd.print(highTempFahrenheit);
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[28])));   // Sauvegarde = Set"
      lcd.print(buffer);
    }
    else if (unitChoice == 0 && langChoice == 0)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[36])));   // Pour regler temperature max:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[24])));   // Presser"
      lcd.print(buffer);
      lcd.write(1);                                                   // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[22])));   // ou"
      lcd.print(buffer);
      lcd.write(2);                                                   // Display down arrow
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[15])));   // Temperature max =
      lcd.print(buffer);
      lcd.print(highTempCelcius);
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[28])));   // Press Set to Save"
      lcd.print(buffer);
    }
  }
  
  else
  {
    if (langChoice == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(" Max < Min = Erreur ");
      delay(1500);
      highTempCelcius = highTempCelcius + 2;
      highTempFahrenheit = highTempFahrenheit + 2;
    }
    else if (langChoice == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(" High < Low = illegal  ");
      delay(1500);
      highTempCelcius = highTempCelcius + 2;
      highTempFahrenheit = highTempFahrenheit + 2;
    }
  }
}

void SetHour()
{
  if (hasrun == false)
  {
    ueeprom.eeprom_read(0, &langChoice);   // Read the value of hourValue from EEPROM memory at position 100
    hasrun = true;
    lcd.clear();
  }

  lcd.createChar(1, up);      // Create custom character for up arrow
  lcd.createChar(2, down);    // Create custom character for down arrow

  setbutton.update();
  upbutton.update();
  downbutton.update();

  if ((upbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    hourValue = (hourValue == 23) ? 0 : hourValue + 1; // Increase the hour value by 1
    lastDebounceTime = millis();
    lcd.clear();
  }

  if ((downbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    hourValue = (hourValue == 0) ? 23 : hourValue - 1;        // Decrease the hour value by 1
    lastDebounceTime = millis();
    lcd.clear();
  }

  if (setbutton.fell())
  {
    hasrun = false;
    menu = 3;                                     // Switch to menu 3
  }

  if (langChoice == 0)
  {
    lcd.setCursor(0, 0);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[32])));   // Pour regler Heures:
    lcd.print(buffer);
    lcd.setCursor(0, 1);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[24])));    // Presser
    lcd.print(buffer);
    lcd.write(1);                                                   // Display up arrow
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[22])));   // ou
    lcd.print(buffer);
    lcd.write(2);                                                   // Display down arrow
    lcd.setCursor(0, 2);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[9])));    // Heure =
    lcd.print(buffer);
    lcd.print(max(min(hourValue, 23), 0));                           // Display the current hour value
    lcd.setCursor(0, 3);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[28])));   // Press Set to Save"
    lcd.print(buffer);
  }
  else if (langChoice == 1)
  {
    lcd.setCursor(0, 0);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[31])));   // To Set Hours:
    lcd.print(buffer);
    lcd.setCursor(0, 1);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[23])));    // PresS"
    lcd.print(buffer);
    lcd.write(1);                                                    // Display up arrow
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[21])));    // or"
    lcd.print(buffer);
    lcd.write(2);                                                    // Display down arrow
    lcd.setCursor(0, 2);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[8])));    // Min Temp = "
    lcd.print(buffer);
    lcd.print(max(min(hourValue, 23), 0));                           // Display the current hour value
    lcd.setCursor(0, 3);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[27])));   // Press Set to Save"
    lcd.print(buffer);
  }
}

void SetLowHumidity()
{
  if (hasrun == false)
  {
    ueeprom.eeprom_read(0, &langChoice);   // Read the value of hourValue from EEPROM memory at position 100
    ueeprom.eeprom_read(24, &highHumidity); // Read the value of highHumidity from EEPROM memory at position 24
    ueeprom.eeprom_read(20, &lowHumidity);  // Read the value of lowHumidity from EEPROM memory at position 20
    hasrun = true;
    lcd.clear();
  }

  setbutton.update();
  upbutton.update();
  downbutton.update();

  lcd.createChar(1, up);    // Create custom character for up arrow
  lcd.createChar(2, down);  // Create custom character for down arrow

  if ((upbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    lowHumidity = (lowHumidity == 100) ? 0 : lowHumidity + 1;    // Increase the low humidity value by 1
    lastDebounceTime = millis();
    lcd.clear();
  }

  if ((downbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    lowHumidity = (lowHumidity == 0) ? 100 : lowHumidity - 1;    // Decrease the low humidity value by 1
    lastDebounceTime = millis();
    lcd.clear();
  }

  if (setbutton.fell())
  {
    ueeprom.eeprom_write(20, lowHumidity);   // Write the low humidity to EEPROM
    lcd.clear();
    menu = 9;   // Switch to menu 9
  }
  
  if (lowHumidity < highHumidity)
  {
    if (langChoice == 1)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[37])));   // To set humidity min:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[23])));    // Press
      lcd.print(buffer);
      lcd.write(1);                                                   // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[21])));   // or
      lcd.print(buffer);
      lcd.write(2);                                                   // Display down arrow
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[16])));    // Min Humi =
      lcd.print(buffer);
      lcd.print(lowHumidity);
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[27])));   // Press Set to Save"
      lcd.print(buffer);
    }
    else if (langChoice == 0)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[38])));   // Pour regler humidite min:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[24])));   // Presser"
      lcd.print(buffer);
      lcd.write(1);                                                   // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[22])));   // ou"
      lcd.print(buffer);
      lcd.write(2);                                                   // Display down arrow
      lcd.setCursor(0, 2);  
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[16])));   // Humidite min =
      lcd.print(buffer);
      lcd.print(lowHumidity);
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[28])));   // Sauvegarde = Set"
      lcd.print(buffer);
    }
  }
  else
  {
    if (langChoice == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(" Min > Max = Erreur");
      delay(1500);
      lowHumidity = lowHumidity - 2;
    }
    else if (langChoice == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(" Min > Max = Error");
      delay(1500);
      lowHumidity = lowHumidity - 2;
    }
  }
}

void SetLowTemp()
{

  if (hasrun == false)
  {
    ueeprom.eeprom_read(0, &langChoice);   // Read the value of hourValue from EEPROM memory at position 0
    ueeprom.eeprom_read(4, &lowTempCelcius);    // Read the value of lowTempCelcius from EEPROM memory at position 4
    ueeprom.eeprom_read(12, &lowTempFahrenheit);    // Read the value of lowTempFahrenheit from EEPROM memory at position 12
    ueeprom.eeprom_read(16, &highTempFahrenheit);   // Read the value of highTempFahrenheit from EEPROM memory at position 16
    ueeprom.eeprom_read(8, &highTempCelcius);   // Read the value of highTempCelcius from EEPROM memory at position 8
    ueeprom.eeprom_read(36, &unitChoice);  // Read the value of unitChoice from EEPROM memory at position 36
    hasrun = true;
    lcd.clear();
  }
  setbutton.update();
  upbutton.update();
  downbutton.update();

  lcd.createChar(1, up);    // Create custom character for up arrow
  lcd.createChar(2, down);  // Create custom character for down arrow
  
    if ((upbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    if (unitChoice == 1)
    {
      lowTempFahrenheit = (lowTempFahrenheit == 257) ? -67 : lowTempFahrenheit + 1;    // Increase the low temperature value by 1 based on unit choice when the down switch is released
      lastDebounceTime = millis();
      lcd.clear();
    }
    else if (unitChoice == 0)
    {
    lowTempCelcius = (lowTempCelcius == 125) ? -55 : lowTempCelcius + 1;    // Increase the low temperature value by 1 based on unit choice when the down switch is released
      lastDebounceTime = millis();;
      lcd.clear();
    }
  }

  if ((downbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    if (unitChoice == 1)
    {
      lowTempFahrenheit = (lowTempFahrenheit == -67) ? 257 : lowTempFahrenheit - 1;    // Decrease the low temperature value by 1 based on unit choice when the down switch is released
      lastDebounceTime = millis();
      lcd.clear();
    }
    else if (unitChoice == 0)
    {
      lowTempCelcius = (lowTempCelcius == -55) ? 125 : lowTempCelcius - 1;  // Decrease the low temperature value by 1 based on unit choice when the down switch is released
      lastDebounceTime = millis();
      lcd.clear();
    }
  }

  if (setbutton.fell())
  {
    ueeprom.eeprom_write(4, lowTempCelcius);     // Write the low temperature in Celsius to EEPROM
    ueeprom.eeprom_write(12, lowTempFahrenheit);     // Write the low temperature in Fahrenheit to EEPROM

    lcd.clear();
    hasrun = false;
    menu = 7;   // Switch to menu 7
  }


  if (lowTempCelcius < highTempCelcius && lowTempFahrenheit < highTempFahrenheit)
  {

    if (unitChoice == 1 && langChoice == 1)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[39])));   // To set temperature min:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[23])));    // Press"
      lcd.print(buffer);
      lcd.write(1);                                                   // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[21])));   // or"
      lcd.print(buffer);
      lcd.write(2);                                                   // Display down arrow
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[18])));    // Min Temp = "
      lcd.print(buffer);
      lcd.print(lowTempFahrenheit);
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[27])));   // Press Set to Save"
      lcd.print(buffer);
    }
    else if (unitChoice == 0 && langChoice == 1)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[39])));   // Celsius selected "
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[23])));    // Press"
      lcd.print(buffer);
      lcd.write(1);                                                    // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[21])));    // or"
      lcd.print(buffer);
      lcd.write(2);                                                    // Display down arrow
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[18])));    // Min Temp = "
      lcd.print(buffer);
      lcd.print(lowTempCelcius);
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[27])));   // Press Set to Save"
      lcd.print(buffer);
    }
    else if (unitChoice == 1 && langChoice == 0 )
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[40])));   // Pour regler temperature min:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[24])));   // Presser"
      lcd.print(buffer);
      lcd.write(1);                                                   // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[22])));   // ou"
      lcd.print(buffer);
      lcd.write(2);                                                   // Display down arrow
      lcd.setCursor(0, 2);  
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[19])));   // Temperature min =
      lcd.print(buffer);
      lcd.print(lowTempFahrenheit);
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[28])));   // Sauvegarde = Set"
      lcd.print(buffer);
    }
    else if (unitChoice == 0 && langChoice == 0)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[40])));   // Pour regler temperature min:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[24])));   // Presser"
      lcd.print(buffer);
      lcd.write(1);                                                   // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[22])));   // ou"
      lcd.print(buffer);
      lcd.write(2);                                                   // Display down arrow
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[19])));   // Temperature min =
      lcd.print(buffer);
      lcd.print(lowTempCelcius);
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[28])));   // Press Set to Save"
      lcd.print(buffer);
    }
  }
  else
  {
    if (langChoice == 0)
    {
      Serial.print("Crotte!");
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(" Min > Max = Erreur");
      delay(1500);
      lowTempCelcius = lowTempCelcius - 2;
      lowTempFahrenheit = lowTempFahrenheit - 2;

    }
    else if (langChoice == 1)
    {
      Serial.print("Wrong place");
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(" Min > Max = Error");
      delay(1500);
      lowTempCelcius = lowTempCelcius - 2;
      lowTempFahrenheit = lowTempFahrenheit - 2;

    }
  }
}

void SetMinutes()
{

  if (hasrun == false)
  {
    ueeprom.eeprom_read(0, &langChoice);   // Read the value of hourValue from EEPROM memory at position 0
    hasrun = true;
    lcd.clear();
  }
  setbutton.update();
  upbutton.update();
  downbutton.update();
  leftbutton.update();
  rightbutton.update();

  if ((upbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    minuteValue = (minuteValue == 59) ? 0 : minuteValue + 1;   // Increase the minute value by 1 when the up switch is pressed and set it to 0 if it is greater than 59
    lastDebounceTime = millis();
    lcd.clear();
  }

  if ((downbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    minuteValue = (minuteValue == 0) ? 59 : minuteValue - 1;    // Decrease the minute value by 1 when the down switch is pressed and set it to 59 if it is less than 0
    lastDebounceTime = millis();
    lcd.clear();
  }

  if (setbutton.fell())
  {
    lcd.clear();
    menu = 4;   // Switch to menu 4
  }

  if (langChoice == 0)
  {
    lcd.setCursor(0, 0);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[42])));   // Pour regler Minutes:
    lcd.print(buffer);
    lcd.setCursor(0, 1);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[24])));    // Presser
    lcd.print(buffer);
    lcd.write(1);                                                   // Display up arrow
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[22])));   // ou
    lcd.print(buffer);
    lcd.write(2);                                                   // Display down arrow
    lcd.setCursor(0, 2);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[20])));    // Minutes =
    lcd.print(buffer);
    lcd.print(max(min(minuteValue, 59), 0));                           // Display the current hour value
    lcd.setCursor(0, 3);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[28])));   // Press Set to Save"
    lcd.print(buffer);
  }
  else if (langChoice == 1)
  {
    lcd.setCursor(0, 0);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[41])));   // To Set Minutes:
    lcd.print(buffer);
    lcd.setCursor(0, 1);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[23])));    // Press"
    lcd.print(buffer);
    lcd.write(1);                                                    // Display up arrow
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[21])));    // or"
    lcd.print(buffer);
    lcd.write(2);                                                    // Display down arrow
    lcd.setCursor(0, 2);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[20])));    // Min Temp = "
    lcd.print(buffer);
    lcd.print(max(min(minuteValue,59),0));                        // Display the current hour value
    lcd.setCursor(0, 3);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[27])));   // Press Set to Save"
    lcd.print(buffer);
  }
}

void SetUnits()
{
  ueeprom.eeprom_read(0, &langChoice);   // Read the value of hourValue from EEPROM memory at position 0
  ueeprom.eeprom_read(36, &unitChoice);   // Read the value of unitChoice from EEPROM memory at position 36
  lcd.clear();
  unitChoice = !unitChoice;        // Invert the value of langChoice
  ueeprom.eeprom_write(36, unitChoice); // Writes the value of langChoice to EEPROM memory at position 0

  if (langChoice == 1 && unitChoice == 1) 
  {
    lcd.setCursor(0, 1);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[55])));   // Saving ..."
    lcd.print(buffer);
    lcd.setCursor(0, 2);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[29])));   // Saving ..."
    lcd.print(buffer);
  }
  else if (langChoice == 1 && unitChoice == 0)
  {
    lcd.setCursor(0, 1);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[52])));   // Saving ..."
    lcd.print(buffer);
    lcd.setCursor(0, 2);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[29])));   // Saving ..."
    lcd.print(buffer);
  }
  else if (langChoice == 0 && unitChoice == 1)
  {
    lcd.setCursor(0, 1);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[56])));   // Saving ..."
    lcd.print(buffer);
    lcd.setCursor(0, 2);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[30])));   // Saving ..."
    lcd.print(buffer);
  }
  else if (langChoice == 0 && unitChoice == 0)
  {
    lcd.setCursor(0, 1);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[53])));   // Saving ..."
    lcd.print(buffer);
    lcd.setCursor(0, 2);
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[30])));   // Saving ..."
    lcd.print(buffer);
  }

  delay(1000);
  lcd.clear();
  hasrun = false;
  menu = 0;   // Switch to menu 0
}

void SetStartHour()
{
  if (hasrun == false)
  {
    ueeprom.eeprom_read(0, &langChoice);   // Read the value of hourValue from EEPROM memory at position 100
    ueeprom.eeprom_read(28, &startHour);    // Read the value of startHour from EEPROM memory at position 28
    ueeprom.eeprom_read(32, &stopHour);     // Read the value of stopHour from EEPROM memory at position 32
    ueeprom.eeprom_read(40, &startMinute);  // Read the value of startMinute from EEPROM memory at position 40
    ueeprom.eeprom_read(44, &stopMinute);   // Read the value of stopMinute from EEPROM memory at position 44
    hasrun = true;
    lcd.clear();
  }

  setbutton.update();
  upbutton.update();
  downbutton.update();

  if ((upbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    startHour = (startHour == 23) ? 0 : startHour + 1; // Increase the start time value by 1 when the up switch is pressed and set it to 0 if it is greater than 23
    lastDebounceTime = millis();
    lcd.clear();
  }

  if (downbutton.fell())
  {
    startHour = (startHour == 0) ? 23 : startHour - 1;    // Decrease the start time value by 1 when the down switch is pressed and set it to 23 if it is less than 0
    lastDebounceTime = millis();
    lcd.clear();
  }

  if (setbutton.fell())
  {
    ueeprom.eeprom_write(28, startHour);     // Write the start time to EEPROM
    hasrun = false;
    menu = 11;   // Switch to menu 11
  }

  if (!(startHour == stopHour && startMinute == stopMinute))
  {
    if (langChoice == 0)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[48])));   // Pour regler Heures de Debut:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[24])));    // Presser
      lcd.print(buffer);
      lcd.write(1);                                                   // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[22])));   // ou
      lcd.print(buffer);
      lcd.write(2);                                                   // Display down arrow
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[9])));    // Heure =
      lcd.print(buffer);
      lcd.print(startHour);                                         // Display the current hour value
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[28])));   // Press Set to Save"
      lcd.print(buffer);
    }
    else if (langChoice == 1)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[47])));   // To Set Start Hours:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[23])));    // Press"
      lcd.print(buffer);
      lcd.write(1);                                                    // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[21])));    // or"
      lcd.print(buffer);
      lcd.write(2);                                                    // Display down arrow
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[8])));    // Hours =
      lcd.print(buffer);
      lcd.print(startHour);                                         // Display the current hour value
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[27])));   // Press Set to Save"
      lcd.print(buffer);
    }
  }
  else
  {
    if (langChoice == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Debut = Fin = Erreur");
      delay(1500);
      startHour = startHour + 2;
    }
    else if (langChoice == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Start = Stop = Error");
      delay(1500);
      startHour = startHour + 2;
    }
  }
}

void SetStartMinute()
{
  if (hasrun == false)
  {
    ueeprom.eeprom_read(0, &langChoice);   // Read the value of hourValue from EEPROM memory at position 0
    ueeprom.eeprom_read(28, &startHour);    // Read the value of startHour from EEPROM memory at position 28
    ueeprom.eeprom_read(32, &stopHour);     // Read the value of stopHour from EEPROM memory at position 32
    ueeprom.eeprom_read(40, &startMinute);  // Read the value of startMinute from EEPROM memory at position 40
    ueeprom.eeprom_read(44, &stopMinute);   // Read the value of stopMinute from EEPROM memory at position 44
    lcd.clear();
    hasrun = true;
  }

  setbutton.update();
  upbutton.update();
  downbutton.update();

  if ((upbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    startMinute = (startMinute == 59) ? 0 : startMinute + 1; // Increase the start time value by 1 when the up switch is pressed and set it to 0 if it is greater than 59.
    lastDebounceTime = millis();
    lcd.clear();
  }

  if (downbutton.fell())
  {
    startMinute = (startMinute == 0) ? 59 : startMinute - 1;    // Decrease the start time value by 1 when the down switch is pressed and set it to 59 if it is less than 0.
    lastDebounceTime = millis();
    lcd.clear();
  }

  if (setbutton.fell())
  {
    ueeprom.eeprom_write(44, startMinute);     // Write the start time to EEPROM.
    hasrun = false;
    menu = 12;   // Switch to menu 12
  }

  if (!(startHour == stopHour && startMinute == stopMinute))
  {

    if (langChoice == 0)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[5])));   // Pour regler Minutes de Debut.
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[24])));    // Presser.
      lcd.print(buffer);
      lcd.write(1);                                                   // Display up arrow.
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[22])));   // ou.
      lcd.print(buffer);
      lcd.write(2);                                                   // Display down arrow.
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[20])));    // Minutes =
      lcd.print(buffer);
      lcd.print(startMinute);                                         // Display the current hour value.
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[28])));   // Press Set to Save.
      lcd.print(buffer);
    }
    else if (langChoice == 1)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[0])));   // To Set Start Hours:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[23])));    // Press.
      lcd.print(buffer);
      lcd.write(1);                                                    // Display up arrow.
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[21])));    // or.
      lcd.print(buffer);
      lcd.write(2);                                                    // Display down arrow.
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[20])));    // Min Temp =
      lcd.print(buffer);
      lcd.print(startMinute);                                         // Display the current hour value.
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[27])));   // Press Set to Save.
      lcd.print(buffer);
    }
  }
  else
  {
    if (langChoice == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Debut = Fin = Erreur");
      delay(1500);
      startMinute = startMinute + 2;
    }
    else if (langChoice == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(" Start = Stop = Error");
      delay(1500);
      startMinute = startMinute + 2;
    }
  }
}

void SetStopHour()
{
  if (hasrun == false)
  {
    ueeprom.eeprom_read(0, &langChoice);   // Read the value of hourValue from EEPROM memory at position 0
    ueeprom.eeprom_read(32, &stopHour);     // Read the value of stopHour from EEPROM memory at position 32
    ueeprom.eeprom_read(28, &startHour);    // Read the value of startHour from EEPROM memory at position 28
    ueeprom.eeprom_read(40, &startMinute);  // Read the value of startMinute from EEPROM memory at position 44
    ueeprom.eeprom_read(44, &stopMinute);   // Read the value of stopMinute from EEPROM memory at position 40
    hasrun = true;
    lcd.clear();
  }

  setbutton.update();
  upbutton.update();
  downbutton.update();

  if ((upbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    stopHour = (stopHour == 23) ? 0 : stopHour + 1;
    lastDebounceTime = millis();
    lcd.clear();
  }

  if ((downbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    stopHour = (stopHour == 0) ? 23 : stopHour - 1;
    lastDebounceTime = millis();
    lcd.clear();
  }

  if (setbutton.fell())
  {
    ueeprom.eeprom_write(32, stopHour);      // Write the stop time to EEPROM
    hasrun = false;
    menu = 13;   // Switch to menu 13

  }
  if (!(startHour == stopHour && startMinute == stopMinute))
  {

    if (langChoice == 0)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[50])));   // Pour regler Heures de Fin:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[24])));   // Presser
      lcd.print(buffer);
      lcd.write(1);                                                   // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[22])));   // ou
      lcd.print(buffer);
      lcd.write(2);                                                   // Display down arrow
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[9])));    // Heure =
      lcd.print(buffer);
      lcd.print(stopHour);                                            // Display the current hour value
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[28])));   // Press Set to Save"
      lcd.print(buffer);
    }
    else if (langChoice == 1)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[47])));    // To Set Start Hours:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[23])));    // Press"
      lcd.print(buffer);
      lcd.write(1);                                                    // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[21])));    // or"
      lcd.print(buffer);
      lcd.write(2);                                                    // Display down arrow
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[8])));    // Hours =
      lcd.print(buffer);
      lcd.print(stopHour);                                            // Display the current start time value 
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[27])));   // Press Set to Save"
      lcd.print(buffer);
    }
  }
  else
  {
    if (langChoice == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Debut = Fin = Erreur");
      delay(1500);
      stopHour = stopHour + 2;
    }
    else if (langChoice == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(" Start = Stop = Error");
      delay(1500);
      stopHour = stopHour + 2;
    }
  }
}

void SetStopMinute()
{
  if (hasrun == false)
  {
    ueeprom.eeprom_read(0, &langChoice);   // Read the value of hourValue from EEPROM memory at position 0
    ueeprom.eeprom_read(28, &startHour);    // Read the value of startHour from EEPROM memory at position 28
    ueeprom.eeprom_read(32, &stopHour);     // Read the value of stopHour from EEPROM memory at position 32
    ueeprom.eeprom_read(40, &startMinute);  // Read the value of startMinute from EEPROM memory at position 44
    ueeprom.eeprom_read(44, &stopMinute);   // Read the value of stopMinute from EEPROM memory at position 40
    hasrun = true;
    lcd.clear();
  }

  setbutton.update();
  upbutton.update();
  downbutton.update();

  if ((upbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    stopMinute = (stopMinute == 59) ? 0 : stopMinute + 1; // Increase the stop time value by 1 when the up switch is pressed and set it to 0 if it is greater than 59
    lastDebounceTime = millis();
    lcd.clear();
  }

  if ((downbutton.read() == LOW && (millis() - lastDebounceTime > buttonInterval)))
  {
    stopMinute = (stopMinute == 0) ? 59 : stopMinute - 1;    // Decrease the stop time value by 1 when the down switch is pressed and set it to 59 if it is less than 0
    lastDebounceTime = millis();
    lcd.clear();
  }

  if (setbutton.fell())
  {
    if (langChoice == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[30])));   // Sauvegarde...
      lcd.print(buffer);
      delay(1500);
    }
    else if (langChoice == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[29])));   // Saving...
      lcd.print(buffer);
      delay(1500);
    }
    ueeprom.eeprom_write(44, stopMinute);     // Write the stop time to EEPROM
    hasrun = false;
    menu = 0;   // Switch to menu 0
  }

  if (!(startHour == stopHour && startMinute == stopMinute))
  {

    if (langChoice == 0)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[4])));   // Pour regler Minutes de Fin:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[24])));   // Presser
      lcd.print(buffer);
      lcd.write(1);                                                   // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[22])));   // ou
      lcd.print(buffer);
      lcd.write(2);                                                   // Display down arrow
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[20])));    // Minutes =
      lcd.print(buffer);
      lcd.print(stopMinute);                                          // Display the current hour value
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[28])));   // Press Set to Save
      lcd.print(buffer);
    }
    else if (langChoice == 1)
    {
      lcd.setCursor(0, 0);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[1])));    // To Set Stop Minutes:
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[23])));    // Press
      lcd.print(buffer);
      lcd.write(1);                                                    // Display up arrow
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[21])));    // or
      lcd.print(buffer);
      lcd.write(2);                                                    // Display down arrow
      lcd.setCursor(0, 2);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[20])));    // Minutes =
      lcd.print(buffer);
      lcd.print(stopMinute);                                          // Display the current hour value
      lcd.setCursor(0, 3);
      strcpy_P(buffer, (char *)pgm_read_word(&(string_table[27])));   // Press Set to Save
      lcd.print(buffer);
    }
  }
  else
  {
    if (langChoice == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Debut = Fin = Erreur");
      delay(1500);
      stopMinute = stopMinute + 2;
    }
    else if (langChoice == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(" Start = Stop = Error");
      delay(1500);
      stopMinute = stopMinute + 2;
    }
  }
}

void Saving() 
{
  rtc.adjust(DateTime(0000, 00, 00, hourValue, minuteValue, 0));    // Set the time on the RTC to the hour and minute values

  if (langChoice == 0)
  {
    lcd.clear();
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[30])));   // Sauvegarde...
    lcd.print(buffer);
  }
  else if (langChoice == 1)
  {
    lcd.clear();
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[29])));   // Saving...
    lcd.print(buffer);
  }

  delay(500);
  hasrun = false;
  menu = 0;   // Switch to menu 0
}

void setup()
{
  lcd.createChar(0, deg);     // Create custom character for degree symbol
  lcd.createChar(1, up);      // Create custom character for up arrow
  lcd.createChar(2, down);    // Create custom character for down arrow

  // Serial.begin(9600);         // Start serial communication at 9600 baud
  rtc.begin();                // Start the RTC
  dht.begin();
  lcd.init();                 // Initialize the LCD
  lcd.backlight();            // Turn on the LCD backlight
  lcd.clear();                // Clear the LCD

  pinMode(setswitch, INPUT_PULLUP);   // Set the set switch to input pullup
  pinMode(upswitch, INPUT_PULLUP);    // Set the up switch to input pullup
  pinMode(downswitch, INPUT_PULLUP);  // Set the down switch to input pullup
  pinMode(leftswitch, INPUT_PULLUP);  // Set the left switch to input pullup
  pinMode(rightswitch, INPUT_PULLUP); // Set the right switch to input pullup

  setbutton.attach(setswitch);        // Attach the set switch to the set button
  upbutton.attach(upswitch);          // Attach the up switch to the up button
  downbutton.attach(downswitch);      // Attach the down switch to the down button
  leftbutton.attach(leftswitch);      // Attach the left switch to the left button
  rightbutton.attach(rightswitch);    // Attach the right switch to the right button

  setbutton.interval(10);             // Set the debouncing of the set button to 50 milliseconds
  upbutton.interval(10);              // Set the debouncing of the up button to 50 milliseconds
  downbutton.interval(10);            // Set the debouncing of the down button to 50 milliseconds
  leftbutton.interval(10);            // Set the debouncing of the left button to 50 milliseconds
  rightbutton.interval(10);           // Set the debouncing of the right button to 50 milliseconds

  pinMode(heat, OUTPUT);              // Set the heat pin to output
  pinMode(cool, OUTPUT);              // Set the cool pin to output
  pinMode(humLow, OUTPUT);            // Set the low humidity pin to output
  pinMode(humHigh, OUTPUT);           // Set the high humidity pin to output
  pinMode(lights, OUTPUT);            // Set the lights pin to output

  digitalWrite(heat, LOW);            // Turn off the heat
  digitalWrite(cool, LOW);           // Turn off the cool
  digitalWrite(humLow, LOW);         // Turn off the low humidity
  digitalWrite(humHigh, LOW);        // Turn off the high humidity
  digitalWrite(lights, LOW);         // Turn off the lights

  ueeprom.eeprom_read(0, &langChoice);      // Read the language choice from EEPROM
  ueeprom.eeprom_read(4, &lowTempCelcius);       // Read the low temperature in Celsius from EEPROM
  ueeprom.eeprom_read(8, &highTempCelcius);      // Read the high temperature in Celsius from EEPROM
  ueeprom.eeprom_read(12, &lowTempFahrenheit);       // Read the low temperature in Fahrenheit from EEPROM
  ueeprom.eeprom_read(16, &highTempFahrenheit);      // Read the high temperature in Fahrenheit from EEPROM
  ueeprom.eeprom_read(20, &lowHumidity);     // Read the low humidity from EEPROM
  ueeprom.eeprom_read(24, &highHumidity);    // Read the high humidity from EEPROM
  ueeprom.eeprom_read(28, &startHour);       // Read the start time from EEPROM
  ueeprom.eeprom_read(32, &stopHour);        // Read the stop time from EEPROM
  ueeprom.eeprom_read(36, &unitChoice);     // Read the unit choice from EEPROM
  ueeprom.eeprom_read(40, &startMinute);       // Read the hour value from EEPROM
  ueeprom.eeprom_read(44, &stopMinute);        // Read the minute value from EEPROM

  delay(1000);

  if (langChoice == 0)                      // If the language choice is French
  {
    lcd.setCursor(0, 1);
    lcd.print(F("    Controleur      "));   // Print "Controleur" to the LCD
    lcd.setCursor(0, 2);
    lcd.print(F("    de Climat       "));   // Print "de Climat" to the LCD
  }
  else                                      // If the language choice is English  
  {
    lcd.setCursor(0, 1);
    lcd.print(F("    Climate         "));   // Print "Climate" to the LCD
    lcd.setCursor(0, 2);
    lcd.print(F("    Controller      "));   // Print "Controller" to the LCD
  }
  delay(1000);
}

void loop()
{
  switch (menu)         // Switch between the different menus
  {
  case 0:
    Relays();           // Run the relays function
    break;          
  case 1:
    Language();         // Run the language function
    break;
  case 2:
    SetHour();          // Run the set hour function
    break;
  case 3:
    SetMinutes();       // Run the set minutes function 
    break;
  case 4:
    Saving();           // Run the saving function
    break;
  case 5:
    SetUnits();         // Run the set units function
    break;
  case 6:
    SetLowTemp();       // Run the set low temperature function
    break;
  case 7:
    SetHighTemp();      // Run the set high temperature function
    break;
  case 8:
    SetLowHumidity();   // Run the set low humidity function
    break;
  case 9:
    SetHighHumidity();  // Run the set high humidity function
    break;
  case 10:
    SetStartHour();     // Run the start time function
    break;
  case 11:
    SetStartMinute();      // Run the stop time function
    break;
  case 12:
    SetStopHour();      // Run the stop time function
    break;
  case 13:
    SetStopMinute();      // Run the stop time function
    break;
  }
}
