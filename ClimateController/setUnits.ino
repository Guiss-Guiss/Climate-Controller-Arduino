void SetUnits() {
  lcd.createChar(0, deg);
  lcd.createChar(3, droite);
  if ((unitChoice == 0)) {
    if (langChoice == 0) {
      lcd.setCursor(0, 0);
      lcd.print(F("    "));
      lcd.write(0);
      lcd.print(F("F Selected     "));
      lcd.setCursor(0, 1);
      lcd.print(F(" Press Set to Save  "));
      lcd.setCursor(0, 2);
      lcd.print(F("                    "));
      lcd.setCursor(0, 3);
      lcd.print(F("   Press "));
      lcd.write(3);
      lcd.write(3);
      lcd.print(F(" For "));
      lcd.write(0);
      lcd.print(F("C  "));
    } else {
      lcd.setCursor(0, 0);
      lcd.print(F("   "));
      lcd.write(0);
      lcd.print(F("F Selectionne   "));
      lcd.setCursor(0, 1);
      lcd.print(F("  Sauvegarde = Set   "));
      lcd.setCursor(0, 2);
      lcd.print(F("                    "));
      lcd.setCursor(0, 3);
      lcd.print(F(" Presser "));
      lcd.write(3);
      lcd.write(3);
      lcd.print(F(" Pour "));
      lcd.write(0);
      lcd.print(F("C "));
    }
  } else {
    if (langChoice == 0) {
      lcd.setCursor(0, 0);
      lcd.print(F("    "));
      lcd.write(0);
      lcd.print(F("C Selected     "));
      lcd.setCursor(0, 1);
      lcd.print(F(" Press Set to Save  "));
      lcd.setCursor(0, 2);
      lcd.print(F("                    "));
      lcd.setCursor(0, 3);
      lcd.print(F("   Press "));
      lcd.write(3);
      lcd.write(3);
      lcd.print(F(" For "));
      lcd.write(0);
      lcd.print(F("F  "));
    } else {
      lcd.setCursor(0, 0);
      lcd.print(F("   "));
      lcd.write(0);
      lcd.print(F("C Selectionne   "));
      lcd.setCursor(0, 1);
      lcd.print(F("  Sauvegarde = Set   "));
      lcd.setCursor(0, 2);
      lcd.print(F("                    "));
      lcd.setCursor(0, 3);
      lcd.print(F(" Presser "));
      lcd.write(3);
      lcd.write(3);
      lcd.print(F(" Pour "));
      lcd.write(0);
      lcd.print(F("F "));
    }
  }
  if (RightSwitch.isPressed()) {
    if ((unitChoice) == 0) {
      unitChoice = 1;
    } else {
      unitChoice = 0;
    }
  }
  if (SetSwitch.isPressed()) {
    lcd.clear();
    eeprom.eeprom_write(90, &unitChoice);
    if ((langChoice == 0)) {
      lcd.setCursor(0, 2);
      lcd.print(F("       Saving...   "));
      delay(500);
      lcd.clear();
      menu = 0;
    } else {
      lcd.setCursor(0, 2);
      lcd.print(F("   Sauvegarde...   "));
      delay(500);
      lcd.clear();
      menu = 0;
    }
  }
}