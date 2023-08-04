void SetHighHumidity() {
  lcd.createChar(1, haut);
  lcd.createChar(2, bas);
  lcd.createChar(3, droite);
  if (langChoice == 0) {
    lcd.setCursor(0, 0);
    lcd.print(F("To Set Max Humi:    "));
    lcd.setCursor(0, 1);
    lcd.print(F("Press "));
    lcd.write(1);
    lcd.print(F(" or "));
    lcd.write(2);
    lcd.print(F("        "));
    lcd.setCursor(0, 2);
    lcd.print(F("Max Humi =  "));
    if (highHumidity <= 9 && highHumidity >= 0
    ) {
      lcd.print(F("0"));
    }
    lcd.print(highHumidity);
    lcd.print(F("      "));
    lcd.setCursor(0, 3);
    lcd.print(F("Press "));
    lcd.write(3);
    lcd.write(3);
    lcd.print(F(" to Save    "));
  } else {
    lcd.setCursor(0, 0);
    lcd.print(F(" Ajuster Humi Max:  "));
    lcd.setCursor(0, 1);
    lcd.print(F("Presser "));
    lcd.write(1);
    lcd.print(F(" ou "));
    lcd.write(2);
    lcd.print(F("     "));
    lcd.setCursor(0, 2);
    lcd.print(F("Humi Max = "));
    if (highHumidity <= 9 && highHumidity >= 0) {
      lcd.print(F("0"));
    }
    lcd.print(highHumidity);
    lcd.print(F("       "));
    lcd.setCursor(0, 3);
    lcd.print(F("  Sauvegarde = "));
    lcd.write(3);
    lcd.write(3);
    lcd.print(F("   "));
  }
  if (UpSwitch.isPressed()) {
    if (highHumidity == 100) {
      highHumidity = 0;
    } else {
      highHumidity = highHumidity + 1;
    }
  }
  if (DownSwitch.isPressed()) {
    if (highHumidity == 0) {
      highHumidity = 100;
    } else {
      highHumidity = highHumidity - 1;
    }
  }
  if (RightSwitch.isReleased()) {
    menu = 10;
  }

}
