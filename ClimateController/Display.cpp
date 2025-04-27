/*
 * Display.cpp
 * 
 * Implementation of display functions and text string management.
 * 
 * Written by Guillaume Ste-Marie
 * GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
 */

#include "Display.h"

// Custom character arrays are defined in Config.cpp and declared as extern in Config.h

// Declaration of text strings in PROGMEM
const char a00[] PROGMEM = "English";              // English string for "English".
const char a01[] PROGMEM = "Francais";             // French string for "Francais".
const char a02[] PROGMEM = "To Choose Language:";  // English string for "To Choose Language:".
const char a03[] PROGMEM = "Choisir la Langue:";   // French string for "Choisir la Langue:".
const char a04[] PROGMEM = "Press";                // English string for "Press".
const char a05[] PROGMEM = "Presser";              // French string for "Presser".
const char a06[] PROGMEM = "       Time:";         // English string for "Time: ".
const char a07[] PROGMEM = "      Heure:";         // French string for "Time: ".
const char a08[] PROGMEM = "Hour = ";               // English string for "Hour = ".
const char a09[] PROGMEM = "Heure = ";              // French string for "Hour = ".
const char a10[] PROGMEM = "   Humidity: ";         // English string for "Humidity: ".
const char a11[] PROGMEM = "   Humidite: ";         // French string for "Humidity: ".
const char a12[] PROGMEM = "Max Humi = ";           // English string for "Max Humi = ".
const char a13[] PROGMEM = "Humidite max = ";       // French string for "Max Humi = ".
const char a14[] PROGMEM = "Min Humi = ";           // English string for "Min Humi = ".
const char a15[] PROGMEM = "Humidite min = ";       // French string for "Min Humi = ".
const char a16[] PROGMEM = "Max Temp = ";           // English string for "Max Temp = ".
const char a17[] PROGMEM = "Temp max = ";           // French string for "Max Temp = ".
const char a18[] PROGMEM = "Min Temp = ";           // English string for "Min Temp = ".
const char a19[] PROGMEM = "Temp min = ";           // French string for "Min Temp = ".
const char a20[] PROGMEM = "Temperature: ";         // String for "Temperature: ".
const char a21[] PROGMEM = " or ";                  // English string for " or ".
const char a22[] PROGMEM = " ou ";                  // French string for " or ".
const char a23[] PROGMEM = "Press ";                // English string for "Press ".
const char a24[] PROGMEM = "Presser ";              // French string for "Press ".
const char a25[] PROGMEM = "! Sonde temperature ";  // English string for "  Press >> to Save  ".
const char a26[] PROGMEM = "  Press SET to Save  ";  // English string for "  Press SET to Save  ".
const char a27[] PROGMEM = "Sauvegarde = SET  ";     // French string for "  Press SET to Save  ".
const char a28[] PROGMEM = "Saving ...";            // English string for "Saving ...".
const char a29[] PROGMEM = "Sauvegarde ...";        // French string for "Saving ...".
const char a30[] PROGMEM = "Minute = ";             // English string for "Minute = ".
const char a31[] PROGMEM = "Minute = ";             // French string for "Minute = ".
const char a32[] PROGMEM = "To Select Unit:    ";   // English string for "To Select Unit:    ".
const char a33[] PROGMEM = "Choisir Unite:     ";   // French string for "To Select Unit:    ".
const char a34[] PROGMEM = "To Set Max Humi:    ";  // English string for "To Set Max Humi:    ".
const char a35[] PROGMEM = "Ajuster Humi Max:   ";  // French string for "To Set Max Humi:    ".
const char a36[] PROGMEM = "To Set Max Temp:    ";  // English string for "To Set Max Temp:    ".
const char a37[] PROGMEM = "Ajuster Temp Max:   ";  // French string for "To Set Max Temp:    ".
const char a38[] PROGMEM = "To Set Min Humi:    ";  // English string for "To Set Min Humi:    ".
const char a39[] PROGMEM = "Regler Humi Min:    ";  // French string for "To Set Min Humi:    ".
const char a40[] PROGMEM = "To Set Min Temp:    ";  // English string for "To Set Min Temp:    ".
const char a41[] PROGMEM = "Regler Temp Min:    ";  // French string for "To Set Min Temp:    ".
const char a42[] PROGMEM = "Start Hour = ";         // English string for "Start Time = ".
const char a43[] PROGMEM = "Heure Debut = ";        // French string for "Start Time = ".
const char a44[] PROGMEM = "Stop Hour = ";          // English string for "Stop Time = ".
const char a45[] PROGMEM = "Heure Fin = ";          // French string for "Stop Time = ".
const char a46[] PROGMEM = "To Set Start Hour:  ";  // English string for "To Set Start Time:  ".
const char a47[] PROGMEM = "Ajuster Heure Debut:";  // French string for "To Set Start Time:  ".
const char a48[] PROGMEM = "To Set Stop Hour:   ";  // English string for "To Set Stop Time:   ".
const char a49[] PROGMEM = "Ajuster Heure Fin:  ";  // French string for "To Set Stop Time:   ".
const char a50[] PROGMEM = "Start Minute = ";       // English string for "Start Time = ".
const char a51[] PROGMEM = "Minute Debut = ";       // French string for "Start Time = ".
const char a52[] PROGMEM = "Stop Minute = ";        // English string for "Stop Time = ".
const char a53[] PROGMEM = "Minute Fin = ";         // French string for "Stop Time = ".
const char a54[] PROGMEM = "  Celcius Selected  ";  // English string for "  Celcius Selected  ".
const char a55[] PROGMEM = "   Celcius Choisi   ";  // French string for "  Celcius Selected  ".
const char a56[] PROGMEM = "Fahrenheit Selected ";  // English string for "Fahrenheit Selected ".
const char a57[] PROGMEM = " Fahrenheit Choisi  ";  // French string for "Fahrenheit Selected ".

// Array of pointers to strings
const char* const string_table[] PROGMEM = {
  a00, a01, a02, a03, a04, a05, a06, a07, a08, a09,
  a10, a11, a12, a13, a14, a15, a16, a17, a18, a19,
  a20, a21, a22, a23, a24, a25, a26, a27, a28, a29,
  a30, a31, a32, a33, a34, a35, a36, a37, a38, a39,
  a40, a41, a42, a43, a44, a45, a46, a47, a48, a49,
  a50, a51, a52, a53, a54, a55, a56, a57
};

// Initialize LCD screen
void initializeDisplay() {
  Wire.begin();                   // Initialize I2C communication
  lcd.init();                    // Initialize the LCD
  lcd.backlight();               // Turn on backlight
  lcd.clear();                   // Clear display
  lcd.home();                    // Move cursor to home position
  
  // Add a small delay after initialization for stability
  delay(100);
  
  createCustomCharacters();      // Create custom characters
  
  // Test display by printing directly
  lcd.print(F("LCD Test"));
  delay(1000);
  lcd.clear();
}

// Create custom characters
void createCustomCharacters() {
  lcd.createChar(0, deg);         // Create degree symbol
  lcd.createChar(1, up);          // Create up arrow character
  lcd.createChar(2, down);        // Create down arrow character
}

// Display localized welcome message at startup
void displayWelcomeScreen() {
  // Clear display for fresh start
  lcd.clear();
  
  // Display appropriate title based on language (centered on line 1)
  lcd.setCursor(0, 1);
  // Use F() macro to store strings in flash memory instead of RAM
  lcd.print(langChoice ? F("    Climate         ") : F("    Controleur      "));
  
  // Display appropriate subtitle based on language (centered on line 2)
  lcd.setCursor(0, 2);
  lcd.print(langChoice ? F("    Controller      ") : F("    de Climat       "));
  
  // Show welcome screen for 1 second
  delay(1000);
}

// Display current temperature and humidity values
void displayCurrentValues(float temperature, float humidity, DateTime now) {
  // Display time on first row
  lcd.setCursor(0, 0);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 6 : 7]))); // Time label
  lcd.print(buffer);
  
  // Format time with leading zeros (HH:MM:SS)
  char timeStr[9]; // Buffer for time string
  sprintf(timeStr, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  lcd.print(timeStr);
  
  // Display temperature on second row
  lcd.setCursor(0, 1);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[20]))); // "Temperature: "
  lcd.print(buffer);
  
  // Display temperature value with unit
  lcd.print(unitChoice ? temperatureFahrenheit : temperatureCelcius);
  lcd.write(0); // Display degree symbol
  lcd.print(unitChoice ? "F" : "C");
  
  // Display humidity on third row
  lcd.setCursor(0, 2);
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 10 : 11]))); // Humidity label
  lcd.print(buffer);
  lcd.print(humidity);
  lcd.print(" %");
}

// Display time elapsed since last reset in days, hours, minutes, seconds
void displayUptime(unsigned long startTime) {
  // Calculate elapsed time components from milliseconds
  unsigned long currentTime = millis();
  unsigned long allSeconds = currentTime / 1000;
  
  // Extract days, hours, minutes, seconds
  int runDays = allSeconds / 86400;
  allSeconds %= 86400; // Seconds remaining after days
  
  int runHours = allSeconds / 3600;
  int secsRemaining = allSeconds % 3600;
  
  int runMinutes = secsRemaining / 60;
  int runSeconds = secsRemaining % 60;
  
  // Format string with proper label based on language ("Since" or "Depuis")
  // j=jour (French for day)
  lcd.setCursor(0, 3);
  char buf[21]; // Buffer for formatted uptime string
  sprintf(buf, langChoice ? "Since: %02dd%02dh%02dm%02ds" : 
                         "Depuis: %02dj%02dh%02dm%02ds", 
          runDays, runHours, runMinutes, runSeconds);
  lcd.print(buf);
}

// Display error message when sensors fail to provide valid readings
void displaySensorError() {
  // Clear display and position cursor for the message
  lcd.clear();
  lcd.setCursor(0, 1);
  
  // Display appropriate error message based on current language
  // We use indexes 26 (English) and 25 (French) for sensor error messages
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 26 : 25])));
  lcd.print(buffer);
  
  // Show message for 2 seconds then clear screen
  delay(2000);
  lcd.clear();
}

// Display a confirmation message when settings are saved
void displaySavedMessage() {
  // Clear display and position cursor for centered message
  lcd.clear();
  lcd.setCursor(0, 1);
  
  // Select appropriate message based on language setting
  // We use indexes 28 (English) and 29 (French) for save confirmation
  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[langChoice ? 28 : 29])));
  lcd.print(buffer);
  
  // Show message briefly (1 second) then clear screen
  delay(1000);
  lcd.clear();
}

// Display a message in the currently selected language
// This is a helper function to simplify localization throughout the code
void displayLocalizedMessage(const char* englishMessage, const char* frenchMessage) {
  // Use ternary operator for concise language selection
  // langChoice: 1 = English, 0 = French
  lcd.print(langChoice ? englishMessage : frenchMessage);
}
