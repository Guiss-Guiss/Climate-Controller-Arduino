void SetLowTemp() {
  lcd.createChar(1, haut);
  lcd.createChar(2, bas);
  if ((unitChoice) == 0) {
    if (langChoice == 0) {
      lcd.setCursor(0, 0);
      lcd.print(F("To Set Min Temp:    "));
      lcd.setCursor(0, 1);
      lcd.print(F("Press "));
      lcd.write(1);
      lcd.print(F(" or "));
      lcd.write(2);
      lcd.print(F("        "));
      lcd.setCursor(0, 2);
      lcd.print(F("Min Temp = "));
      if (lowTemp_F <= 9 && lowTemp_F >= 0) {
        lcd.print(F("0"));
      }
      lcd.print(lowTemp_F);
      lcd.print(F("       "));
      lcd.setCursor(0, 3);
      lcd.print(F("Press Set to Save   "));
    } else {
      lcd.setCursor(0, 0);
      lcd.print(F(" Ajuster Temp Min:  "));
      lcd.setCursor(0, 1);
      lcd.print(F("Presser "));
      lcd.write(1);
      lcd.print(F(" ou "));
      lcd.write(2);
      lcd.print(F("      "));
      lcd.setCursor(0, 2);
      lcd.print(F("Temp Min = "));
      if (lowTemp_F <= 9 && lowTemp_F >=0) {
        lcd.print(F("0"));
      }
      lcd.print(lowTemp_F);
      lcd.print(F("       "));
      lcd.setCursor(0, 3);
      lcd.print(F("  Sauvegarde = Set  "));
    }
  } else {
    if (langChoice == 0) {
      lcd.setCursor(0, 0);
      lcd.print(F("To Set Min Temp:    "));
      lcd.setCursor(0, 1);
      lcd.print(F("Press "));
      lcd.write(1);
      lcd.print(F(" or "));
      lcd.write(2);
      lcd.print(F("        "));
      lcd.setCursor(0, 2);
      lcd.print(F("Min Temp = "));
      if ((lowTemp_C) <= 9) {
        lcd.print(F("0"));
      }
      lcd.print(lowTemp_C);
      lcd.print(F("       "));
      lcd.setCursor(0, 3);
      lcd.print(F("Press Set to Save   "));
    } else {
      lcd.setCursor(0, 0);
      lcd.print(F(" Ajuster Temp Min:  "));
      lcd.setCursor(0, 1);
      lcd.print(F("Presser "));
      lcd.write(1);
      lcd.print(F(" ou "));
      lcd.write(2);
      lcd.print(F("      "));
      lcd.setCursor(0, 2);
      lcd.print(F("Temp Min = "));
      if ((lowTemp_C) <= 9) {
        lcd.print(F("0"));
      }
      lcd.print(lowTemp_C);
      lcd.print(F(" "));
      lcd.setCursor(0, 3);
      lcd.print(F("  Sauvegarde = Set  "));
    }
  }
  if (UpSwitch.isPressed()) {
    if ((unitChoice) == 0) {
      if (lowTemp_F == 257) {
        lowTemp_F = -67;
      } else {
        lowTemp_F = lowTemp_F + 1;
      }
    } else {
      if (lowTemp_C == 125) {
        lowTemp_C = -55;
      } else {
        lowTemp_C = lowTemp_C + 1;
      }
    }
  }
  if (DownSwitch.isPressed()) {
    if ((unitChoice) == 0) {
      if (lowTemp_F == -67) {
        lowTemp_F = 257;
      } else {
        lowTemp_F = lowTemp_F - 1;
      }
    } else {
      if (lowTemp_C == -55) {
        lowTemp_C = 125;
      } else {
        lowTemp_C = lowTemp_C - 1;
      }
    }
  }
  if (SetSwitch.isPressed()) {
    menu = 7;
  }
}