void SetHour() {
  lcd.createChar(1, haut);
  lcd.createChar(2, bas);
  if (langChoice == 0) {
    lcd.setCursor(0, 0);
    lcd.print(F("To Set Hours:       "));
    lcd.setCursor(0, 1);
    lcd.print(F("Press "));
    lcd.write(1);
    lcd.print(F(" or "));
    lcd.write(2);
    lcd.print(F("        "));
    lcd.setCursor(0, 2);
    lcd.print(F("Hour = "));
    if (HourValue <= 9 && HourValue >= 0) {
      lcd.print(F("0"));
    }
    lcd.print(HourValue);
    lcd.print(F("           "));
    lcd.setCursor(0, 3);
    lcd.print(F("Press Set to Save   "));
  } else {
    lcd.setCursor(0, 0);
    lcd.print(F("Pour Ajuster Heures:"));
    lcd.setCursor(0, 1);
    lcd.print(F("Presser  "));
    lcd.write(1);
    lcd.print(F(" ou "));
    lcd.write(2);
    lcd.print(F("     "));
    lcd.setCursor(0, 2);
    lcd.print(F("Heure = "));
    if ((HourValue) <= 9) {
      lcd.print(F("0"));
    }
    lcd.print(HourValue);
    lcd.print(F("          "));
    lcd.setCursor(0, 3);
    lcd.print(F("  Sauvegarde = Set  "));
  }
  if (UpSwitch.isPressed()) {
    if (HourValue == 23) {
      HourValue = 0;
    } else {
      HourValue = HourValue + 1;
    }
  }
  if (DownSwitch.isPressed()) {
    if (HourValue == 0) {
      HourValue = 23;
    } else {
      HourValue = HourValue - 1;
    }
  }
  if (SetSwitch.isPressed()) {
    menu = 3;
  }
}