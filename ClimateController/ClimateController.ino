//This sketch allows the user to control the climate in a room
//using a variety of sensing and controlling devices.
//It sets up the necessary pins, libraries, and variables to read the
//temperature, humidity, and control the outputs for the heater,
//exhaust fan, humidifier, dehumidifier, and lights.
//The user can access a menu to set the desired temperature, humidity,
//and lighting times.
//Written by Guillaume Ste-Marie <guillaume@guillaumeguiss.com>
//GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.

//Libraries
#include <OneWire.h>
#include <DFRobot_SHT20.h>      // By DfRobot.
#include <DallasTemperature.h>  // By Miles Burton.
#include <uEEPROMLib.h>         // By Naguissa.
#include <RTClib.h>             // By Adafruit.
#include <LiquidCrystal_I2C.h>  // By Marco Schwartz.
#include <Wire.h>               // Built-in.
#include <ezButton.h>           // By Arduinogetstarted.com
#define ONE_WIRE_BUS 7

//Addressing
uEEPROMLib eeprom(0x57);
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 20, 4);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DFRobot_SHT20 sht20(&Wire, SHT20_I2C_ADDR);

ezButton SetSwitch(8);     // "Set" momentary switch.
ezButton UpSwitch(9);      // "Up" momentary switch.
ezButton DownSwitch(10);   // "Down" momentary switch.
ezButton LeftSwitch(11);   // "Left" momentary switch.
ezButton RightSwitch(12);  // "Right" momentary switch.

const int heat = 2;        // Output for heater relay.
const int cool = 3;        // Output for Exhaust fan relay.
const int humLow = 4;      // Output for humidifier relay.
const int humHigh = 5;     // Output for dehumidifier relay.
const int lights = 6;      // Output for lighting relay.

//Variables
int menu = 0;
int HourValue;
int MinuteValue;
int lowTemp_C;
int highTemp_C;
int lowTemp_F;
int highTemp_F;
int lowHumidity;
int highHumidity;
int startTime;
int stopTime;
int langChoice;
int unitChoice;
float humidity;
float temperature_C;
float temperature_F;
const unsigned long eventInterval = 5000;
unsigned long previousTime = 0;

// LCD custom 5 by 8 dots characters
byte deg[8] = { B00110, B01001, B01001, B00110, B00000, B00000, B00000, B00000 };     // LCD design for degree symbol.
byte haut[8] = { B00100, B01110, B11111, B00000, B00000, B00100, B01110, B11111 };    // LCD design for up arrows symbol.
byte bas[8] = { B11111, B01110, B00100, B00000, B00000, B11111, B01110, B00100 };     // LCD design for down arrows symbol.
byte droite[8] = { B00000, B00000, B01000, B01100, B01110, B01100, B01000, B00000 };  // LCD design for right arrows symbol.
byte gauche[8] = { B00000, B00000, B00010, B00110, B01110, B00110, B00010, B00000 };  // LCD design for left arrows symbol.

void Language();
void Relays();
void Saving();
void SetHighHumidity();
void SetHighTemp();
void SetHour();
void SetLowHumidity();
void SetLowTemp();
void SetMinutes();
void SetUnits();
void StartTime();
void StopTime();

void setup() {
  Serial.begin(9600);
  rtc.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Wire.begin();
  sensors.begin();
  sht20.initSHT20();
  sht20.checkSHT20();
  
  pinMode(heat, OUTPUT);
  pinMode(cool, OUTPUT);
  pinMode(humLow, OUTPUT);
  pinMode(humHigh, OUTPUT);
  pinMode(lights, OUTPUT);
  
  digitalWrite(heat, LOW);
  digitalWrite(cool, HIGH);
  digitalWrite(humLow, HIGH);
  digitalWrite(humHigh, HIGH);
  digitalWrite(lights, HIGH);
  
  SetSwitch.setDebounceTime(10);
  UpSwitch.setDebounceTime(10);
  DownSwitch.setDebounceTime(10);
  LeftSwitch.setDebounceTime(10);
  RightSwitch.setDebounceTime(10);

  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("     Controleur     "));
  lcd.setCursor(0, 1);
  lcd.print(F("     de Climat      "));
  lcd.setCursor(0, 2);
  lcd.print(F("Guillaume Ste-Marie "));
  lcd.setCursor(0, 3);
  lcd.print(F("          version 37"));
  
  eeprom.eeprom_read(0, &langChoice);
  eeprom.eeprom_read(10, &lowTemp_C);
  eeprom.eeprom_read(20, &highTemp_C);
  eeprom.eeprom_read(30, &lowTemp_F);
  eeprom.eeprom_read(40, &highTemp_F);
  eeprom.eeprom_read(50, &lowHumidity);
  eeprom.eeprom_read(60, &highHumidity);
  eeprom.eeprom_read(70, &startTime);
  eeprom.eeprom_read(80, &stopTime);
  eeprom.eeprom_read(90, &unitChoice);
}
void loop() {
  SetSwitch.loop();
  UpSwitch.loop();
  DownSwitch.loop();
  LeftSwitch.loop();
  RightSwitch.loop();
  
  if (menu == 0) {
    Relays();
  }
  if (menu == 1) {
    Language();
  }
  if (menu == 2) {
    SetHour();
  }
  if (menu == 3) {
    SetMinutes();
  }
  if (menu == 4) {
    Saving();
  }
  if (menu == 5) {
    SetUnits();
  }
  if (menu == 6) {
    SetLowTemp();
  }
  if (menu == 7) {
    SetHighTemp();
  }
  if (menu == 8) {
    SetLowHumidity();
  }
  if (menu == 9) {
    SetHighHumidity();
  }
  if (menu == 10) {
    StartTime();
  }
  if (menu == 11) {
    StopTime();
  }
}
