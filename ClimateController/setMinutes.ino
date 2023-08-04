void SetMinutes() {
  lcd.createChar(1, haut);
  lcd.createChar(2, bas);
  lcd.createChar(3, droite);
  if (langChoice == 0) {
    lcd.setCursor(0, 0);
    lcd.print(F("To Set Minutes:     "));
    lcd.setCursor(0, 1);
    lcd.print(F("Press "));
    lcd.write(1);
    lcd.print(F(" or "));
    lcd.write(2);
    lcd.print(F("        "));
    lcd.setCursor(0, 2);
    lcd.print(F("Minute = "));
    if (MinuteValue <= 9 && MinuteValue >= 0) {
      lcd.print(F("0"));
    }
    lcd.print(MinuteValue);
    lcd.print(F("         "));
    lcd.setCursor(0, 3);
    lcd.print(F("Press "));
    lcd.write(3);
    lcd.write(3);
    lcd.print((" to Save    "));
  } else {
    lcd.setCursor(0, 0);
    lcd.print(F("Ajuster Minutes:    "));
    lcd.setCursor(0, 1);
    lcd.print(F("Presser  "));
    lcd.write(1);
    lcd.print(F(" ou "));
    lcd.write(2);
    lcd.print(F(" "));
    lcd.setCursor(0, 2);
    lcd.print(F("Minute = "));
    if (MinuteValue <= 9 && MinuteValue >= 0) {
      lcd.print(F("0"));
    }
    lcd.print(MinuteValue);
    lcd.print(F("         "));
    lcd.setCursor(0, 3);
    lcd.print(F("  Sauvegarde = "));
    lcd.write(3);
    lcd.write(3);
    lcd.print(F("   "));

  }
  if (UpSwitch.isPressed()) {
    if (MinuteValue == 59) {
      MinuteValue = 0;
    } else {
      MinuteValue = MinuteValue + 1;
    }
  }
  if (DownSwitch.isPressed()) {
    if (MinuteValue == 0) {
      MinuteValue = 59;
    } else {
      MinuteValue = MinuteValue - 1;
    }
  }
  if (RightSwitch.isReleased()) {
    menu = 4;
  }
}