void SetLowHumidity() {
  lcd.createChar(1, haut);
  lcd.createChar(2, bas);
  if (langChoice == 0) {
    lcd.setCursor(0, 0);
    lcd.print("To Set Min Humi:    ");
    lcd.setCursor(0, 1);
    lcd.print("Press ");
    lcd.write(1);
    lcd.print(" or ");
    lcd.write(2);
    lcd.print("        ");
    lcd.setCursor(0, 2);
    lcd.print("Min Humi = ");
    if (lowHumidity <= 9 && lowHumidity >=0){
    lcd.print("0");
    }
    lcd.print(lowHumidity);
    lcd.print("       ");
    lcd.setCursor(0, 3);
    lcd.print("Press Set to Save   ");
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print(" Ajuster Humi Min:  ");
    lcd.setCursor(0, 1);
    lcd.print("Presser ");
    lcd.write(1);
    lcd.print(" ou ");
    lcd.write(2);
    lcd.print(" ");
    lcd.setCursor(0, 2);
    lcd.print("Humi Min = ");
    if (lowHumidity <= 9 && lowHumidity >=0) {
      lcd.print("0");
    }
    lcd.print(lowHumidity);
    lcd.print("       ");
    lcd.setCursor(0, 3);
    lcd.print("  Sauvegarde = Set  ");
  }
  if (UpSwitch.isPressed()) {
    if (lowHumidity == 100) {
      lowHumidity = 0;
    } else {
      lowHumidity = lowHumidity + 1;
    }
  }
  if (DownSwitch.isPressed()) {
    if (lowHumidity == 0) {
      lowHumidity = 100;
    } else {
      lowHumidity = lowHumidity - 1;
    }
  }
  if (SetSwitch.isPressed()) {
    menu = 9;
  }
}