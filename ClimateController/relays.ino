void Relays() {
  unsigned long currentTime = millis();
  sensors.setResolution(12);
  DateTime now = rtc.now();
  humidity = sht20.readHumidity();
  sensors.requestTemperatures();
  temperature_C = sensors.getTempCByIndex(0);
  temperature_F = (DallasTemperature::toFahrenheit(temperature_C));
  void(* resetFunc) (void) = 0;

  if (isnan(humidity) || isnan(temperature_C) || isnan(temperature_F)) {
    delay(2000);
    return;
  }

  if (currentTime > 4000000000) {
    resetFunc(); //Resets the controller after 46 days so millis() never get to 4,294,967,295. 
  }
  
  if (currentTime - previousTime >= eventInterval) {
    lcd.clear();
    if (langChoice == 0) {
      DateTime now = rtc.now();
      lcd.setCursor(0, 0);
      lcd.print(F("       Time: "));
      if (now.hour() <= 9) {
        lcd.print(F("0"));
      }
      lcd.print(now.hour());
      HourValue = now.hour();
      lcd.print(F(":"));
      if (now.minute() <= 9) {
        lcd.print(F("0"));
      }
      lcd.print(now.minute());
      MinuteValue = now.minute();
      lcd.createChar(0, deg);
      lcd.setCursor(0, 1);
      if ((unitChoice) == 0) {
        lcd.print(F("Temperature: "));
        lcd.print(temperature_F);
        lcd.write(0);
        lcd.print(F("F"));
      } else {
        lcd.print(F("Temperature: "));
        lcd.print(temperature_C);
        lcd.write(0);
        lcd.print(F("C"));
      }
      lcd.setCursor(0, 2);
      lcd.print(F("   Humidity: "));
      lcd.print(humidity);
      lcd.print(F("%"));
      lcd.setCursor(0, 3);
      lcd.print(F("             "));
    } else {
      lcd.setCursor(0, 0);
      lcd.print(F("      Heure: "));
      if (now.hour() <= 9) {
        lcd.print(F("0"));
      }
      lcd.print(now.hour());
      HourValue = now.hour();
      lcd.print(F(":"));
      if (now.minute() <= 9) {
        lcd.print(F("0"));
      }
      lcd.print(now.minute());
      MinuteValue = now.minute();
      lcd.createChar(0, deg);
      lcd.setCursor(0, 1);
      if ((unitChoice) == 0) {
        lcd.print(F("Temperature: "));
        lcd.print(temperature_F);
        lcd.write(0);
        lcd.print(F("F"));
      } else {
        lcd.print(F("Temperature: "));
        lcd.print(temperature_C);
        lcd.write(0);
        lcd.print(F("C"));
      }
      lcd.setCursor(0, 2);
      lcd.print(F("   Humidite: "));
      lcd.print(humidity);
      lcd.print(F("%"));
      lcd.setCursor(0, 3);
      lcd.print(millis());
    }
    if ((unitChoice) == 0) {
      if (temperature_F < lowTemp_F) {
        digitalWrite(heat, HIGH);
      } else if (temperature_F > (lowTemp_F + 1)) {
        digitalWrite(heat, LOW);
      }
      if (temperature_F > highTemp_F) {
        digitalWrite(cool, LOW);
      } else if (temperature_F < (highTemp_F - 1)) {
        digitalWrite(cool, HIGH);
      }
      if (humidity < lowHumidity && temperature_F < highTemp_F) {
        digitalWrite(humLow, LOW);
      } else if (humidity > (lowHumidity + 1)) {
        digitalWrite(humLow, HIGH);
      }
      if (humidity > highHumidity && temperature_F < highTemp_F && temperature_F > lowTemp_F) {
        digitalWrite(humHigh, LOW);
      } else if (humidity < (highHumidity - 1)) {
        digitalWrite(humHigh, HIGH);
      }
    } else {
      if (temperature_C < lowTemp_C) {
        digitalWrite(heat, HIGH);
      } else if (temperature_C > (lowTemp_C + 1)) {
        digitalWrite(heat, LOW);
      }
      if (temperature_C > highTemp_C) {
        digitalWrite(cool, LOW);
      } else if (temperature_C < (highTemp_C - 1)) {
        digitalWrite(cool, HIGH);
      }
      if (humidity < lowHumidity && temperature_C < highTemp_C) {
        digitalWrite(humLow, LOW);
      } else if (humidity > (humLow + 1)) {
        digitalWrite(humLow, HIGH);
      }
      if (humidity > highHumidity && temperature_C < highTemp_C && temperature_C > lowTemp_C) {
        digitalWrite(humHigh, LOW);
      } else if (humidity < (humHigh - 1)) {
        digitalWrite(humHigh, HIGH);
      }
    }
    if (now.hour() >= startTime && now.hour() < stopTime) {
      digitalWrite(lights, LOW);
    } else {
      digitalWrite(lights, HIGH);
    }
    previousTime = currentTime;
  }
  if (UpSwitch.isPressed()) {
    menu = 1;
  }
  if (RightSwitch.isPressed()) {
    menu = 2;
  }
  if (DownSwitch.isPressed()) {
    menu = 5;
  }
  if (LeftSwitch.isPressed()) {
    menu = 6;
  }
}