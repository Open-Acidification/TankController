// ************************************************
// Change Ki value
// ************************************************

void Change_Ki()
{
  Serial.println(F("starting Ki adjustment routine"));
  double KiTemp;
  KiTemp = Ki;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Ki:"));
  lcd.setCursor(0, 1);
  lcd.print(KiTemp);
  Key = NO_KEY;
  Serial.print(F("Key: "));
  Serial.println(Key);
  while (Key != '#') {
    Serial.println(F("In the while loop to wait for key presses"));
    Key = customKeypad.waitForKey();
    Serial.print(F("Key: "));
    Serial.println(Key);
    if (Key == '1') {
      Serial.println(F("in the 1 if statement"));
      KiTemp = KiTemp + 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }
    if (Key == '4') {
      Serial.println(F("in the 4 if statement"));
      KiTemp = KiTemp - 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '2') {
      KiTemp = KiTemp + 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '5') {
      KiTemp = KiTemp - 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '3') {
      KiTemp = KiTemp + 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '6') {
      KiTemp = KiTemp - 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == 'A') {
      KiTemp = KiTemp + 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == 'B') {
      KiTemp = KiTemp - 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '7') {
      KiTemp = KiTemp + 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '*') {
      KiTemp = KiTemp - 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '8') {
      KiTemp = KiTemp + 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '0') {
      KiTemp = KiTemp - 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }
  }
  Serial.println(F("out of the Ki adjusting while loop"));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Ki:"));
  lcd.print(KiTemp);
  lcd.setCursor(0, 1);
  lcd.print(F("Keep:1 Discard:2"));

  while (Key != '2') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      Ki = KiTemp;
      EEPROM_writeDouble(KiAddress, Ki);
      myPID.SetTunings(Kp, Ki, Kd);
      Key = '2';
    }
  }
}