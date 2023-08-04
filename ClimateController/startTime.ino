void StartTime() {
  lcd.createChar(1, haut);
  lcd.createChar(2, bas);
  if (langChoice == 0) {
    lcd.setCursor(0, 0);
    lcd.print(F("To Set Start Time:  "));
    lcd.setCursor(0, 1);
    lcd.print(F("Press "));
    lcd.write(1);
    lcd.print(F(" or "));
    lcd.write(2);
    lcd.print(F("        "));
    lcd.setCursor(0, 2);
    lcd.print(F("Start Time = "));
    if (startTime <= 9 && startTime >=0) {
      lcd.print(F("0"));
    }
    lcd.print(startTime);
    lcd.print(F("     "));
    lcd.setCursor(0, 3);
    lcd.print(F("Press Set to Save   "));
  } else {
    lcd.setCursor(0, 0);
    lcd.print(F("Ajuster Heure Debut:"));
    lcd.setCursor(0, 1);
    lcd.print(F("Presser "));
    lcd.write(1);
    lcd.print(F(" ou "));
    lcd.write(2);
    lcd.print(F("      "));
    lcd.setCursor(0, 2);
    lcd.print(F("Heure Debut = "));
    if (startTime <= 9 && startTime >=0) {
      lcd.print(F("0"));
    }
    lcd.print(startTime);
    lcd.print(F("    "));
    lcd.setCursor(0, 3);
    lcd.print(F("  Sauvegarde = Set  "));
  }
  if (UpSwitch.isPressed()) {
    if (startTime == 23) {
      startTime = 0;
    } else {
      startTime = startTime + 1;
    }
  }
  if (DownSwitch.isPressed()) {
    if (startTime == 0) {
      startTime = 23;
    } else {
      startTime = startTime - 1;
    }
  }
  if (SetSwitch.isPressed()) {
    menu = 11;
  }
}
