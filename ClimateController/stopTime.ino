void StopTime() {
  lcd.createChar(1, haut);
  lcd.createChar(2, bas);
  if (langChoice == 0) {
    lcd.setCursor(0, 0);
    lcd.print(F("To Set  Stop Time:  "));
    lcd.setCursor(0, 1);
    lcd.print(F("Press "));
    lcd.write(1);
    lcd.print(F(" or "));
    lcd.write(2);
    lcd.print(F("        "));
    lcd.setCursor(0, 2);
    lcd.print(F("Stop Time = "));
    if (stopTime <= 9 && stopTime >= 0) {
      lcd.print(F("0"));
    }
    lcd.print(stopTime);
    lcd.print(F("      "));
    lcd.setCursor(0, 3);
    lcd.print(F("Press "));
    lcd.write(3);
    lcd.write(3);
    lcd.print(F(" to Save    "));
  } else {
    lcd.setCursor(0, 0);
    lcd.print(F(" Ajuster Heure Fin: "));
    lcd.setCursor(0, 1);
    lcd.print(F("Presser "));
    lcd.write(1);
    lcd.print(F(" ou "));
    lcd.write(2);
    lcd.print(F("      "));
    lcd.setCursor(0, 2);
    lcd.print(F("Heure Fin = "));
    if (stopTime <= 9 && stopTime >=0) {
      lcd.print(F("0"));
    }
    lcd.print(stopTime);
    lcd.print(F("      "));
    lcd.setCursor(0, 3);
    lcd.print(F("  Sauvegarde = "));
    lcd.write(3);
    lcd.write(3);
    lcd.print(F("   "));
  }
  if (UpSwitch.isPressed()) {
    if (stopTime == 23) {
      stopTime = 0;
    } else {
      stopTime = stopTime + 1;
    }
  }
  if (DownSwitch.isPressed()) {
    if (stopTime == 0) {
      stopTime = 23;
    } else {
      stopTime = stopTime - 1;
    }
  }
  if (RightSwitch.isReleased()) {
    lcd.clear();
    eeprom.eeprom_write(10, lowTemp_C);
    eeprom.eeprom_write(20, highTemp_C);
    eeprom.eeprom_write(30, lowTemp_F);
    eeprom.eeprom_write(40, highTemp_F);
    eeprom.eeprom_write(50, lowHumidity);
    eeprom.eeprom_write(60, highHumidity);
    eeprom.eeprom_write(70, startTime);
    eeprom.eeprom_write(80, stopTime);
    lcd.setCursor(0, 0);
    if (langChoice == 0) {
      lcd.print(F("Saving..."));
    } else{
      lcd.print(F("Sauvegarde..."));
    }
    
    delay(500);
    menu = 0;
  }
}