void SetHighTemp() {
  lcd.createChar(1, haut);
  lcd.createChar(2, bas);
  lcd.createChar(3, droite);

  if ((unitChoice) == 0) {
    if (langChoice == 0) {
      lcd.setCursor(0, 0);
      lcd.print(F("To Set Max Temp:    "));
      lcd.setCursor(0, 1);
      lcd.print(F("Press "));
      lcd.write(1);
      lcd.print(F(" or "));
      lcd.write(2);
      lcd.print(F("        "));
      lcd.setCursor(0, 2);
      lcd.print(F("Max Temp = "));
      if (highTemp_F <= 9 && highTemp_F > 0) {
        lcd.print(F("0"));
      }
      lcd.print(highTemp_F);
      lcd.print(F("       "));
      lcd.setCursor(0, 3);
      lcd.print(F("Press "));
      lcd.write(3);
      lcd.write(3);
      lcd.print(F(" to Save    "));
    } else {
      lcd.setCursor(0, 0);
      lcd.print(F(" Ajuster Temp Max:  "));
      lcd.setCursor(0, 1);
      lcd.print(F("Presser "));
      lcd.write(1);
      lcd.print(F(" ou "));
      lcd.write(2);
      lcd.print(F("     "));
      lcd.setCursor(0, 2);
      lcd.print(F("Temp Max = "));
      if ((highTemp_F) <= 9 && highTemp_F >0) {
        lcd.print(F("0"));
      }
      lcd.print(highTemp_F);
      lcd.print(F("          "));
      lcd.setCursor(0, 3);
      lcd.print(F("  Sauvegarde = "));
      lcd.write(3);
      lcd.write(3);
      lcd.print(F("   "));
    }
  } else {
      if (langChoice == 0) {
        lcd.setCursor(0, 0);
        lcd.print(F("To Set Max Temp:    "));
        lcd.setCursor(0, 1);
        lcd.print(F("Press "));
        lcd.write(1);
        lcd.print(F(" or "));
        lcd.write(2);
        lcd.print(F("        "));
        lcd.setCursor(0, 2);
        lcd.print(F("Max Temp =  "));
        if (highTemp_C <= 9 && highTemp_C >= 0) {
          lcd.print(F("0"));
        }
        lcd.print(highTemp_C);
        lcd.print(F("      "));
        lcd.setCursor(0, 3);
        lcd.print(F("Press "));
        lcd.write(3);
        lcd.write(3);
        lcd.print(F(" to Save    "));
      } else {
        lcd.setCursor(0, 0);
        lcd.print(F(" Ajuster Temp Max:  "));
        lcd.setCursor(0, 1);
        lcd.print(F("Presser "));
        lcd.write(1);
        lcd.print(F(" ou "));
        lcd.write(2);
        lcd.print(F("      "));
        lcd.setCursor(0, 2);
        lcd.print(F("Temp Max = "));
        if (highTemp_C <= 9 && highTemp_C >= 0) {
          lcd.print(F("0"));
        }
        lcd.print(highTemp_C);
        lcd.print(F("       "));
        lcd.setCursor(0, 3);
        lcd.print(F("  Sauvegarde = "));
        lcd.write(3);
        lcd.write(3);
        lcd.print(F("   "));
      }
    }
  if (UpSwitch.isPressed()) {
    if ((unitChoice) == 0) {
      if (highTemp_F == 257) {
        highTemp_F = -67;
      } else {
        highTemp_F = highTemp_F + 1;
      }
    } else {
      if (highTemp_C == 125) {
        highTemp_C = -55;
      } else {
        highTemp_C = highTemp_C + 1;
      }
    }
  }
  if (DownSwitch.isPressed()) {
    if ((unitChoice) == 0) {
      if (highTemp_F == -67) {
        highTemp_F = 257;
      } else {
        highTemp_F = highTemp_F - 1;
      }
    } 
    else {
    if (highTemp_C == -55) {
      highTemp_C = 125;
    } else {
      highTemp_C = highTemp_C - 1;
    }
  }
  }
  if (RightSwitch.isReleased()) {
    menu = 8;
  }
}
