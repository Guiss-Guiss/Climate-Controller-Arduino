void Saving() {
  if (RightSwitch.isReleased()) {
    lcd.clear();
     lcd.setCursor(0, 0);
    if ((langChoice) == 0) {
    lcd.print(F("SAVING ..."));
    } else {
     lcd.print(F("Sauvegarde ..."));
    }
  rtc.adjust(DateTime(1971, 06, 25, HourValue, MinuteValue, 0));
  delay(500);
  menu = 0;
  }
}