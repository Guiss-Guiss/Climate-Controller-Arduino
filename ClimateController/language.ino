void Language() {
  if ((langChoice == 0)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("  English Selected  "));
    lcd.setCursor(0, 1);
    lcd.print(F("     Press Set      "));
    lcd.setCursor(0, 2);
    lcd.print(F("      to Save       "));
    lcd.setCursor(0, 3);
    lcd.print(F(" Presser >> pour Fr "));
  } else {
    lcd.setCursor(0, 0);
    lcd.print(F("Francais Selectionne"));
    lcd.setCursor(0, 1);
    lcd.print(F("  Presser Set pour  "));
    lcd.setCursor(0, 2);
    lcd.print(F("     Sauvegarder    "));
    lcd.setCursor(0, 3);
    lcd.print(F("Press >> for English"));
  }
  if (RightSwitch.isPressed()) {
    if ((langChoice) == 0) {
      langChoice = 1;
    } else {
      langChoice = 0;
    }
  }
  if (SetSwitch.isPressed()) {
    lcd.clear();
    eeprom.eeprom_write(0, langChoice);
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