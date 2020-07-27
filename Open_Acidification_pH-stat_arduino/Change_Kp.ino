// ************************************************
// Change Kp value
// ************************************************

void Change_Kp() {
  double KpTemp;
  KpTemp = Kp;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Kp:"));
  lcd.setCursor(0, 1);
  lcd.print(KpTemp);
  Key = NO_KEY;
  while (Key != '#') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      KpTemp = KpTemp + 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }
    if (Key == '4') {
      KpTemp = KpTemp - 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '2') {
      KpTemp = KpTemp + 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '5') {
      KpTemp = KpTemp - 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '3') {
      KpTemp = KpTemp + 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '6') {
      KpTemp = KpTemp - 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == 'A') {
      KpTemp = KpTemp + 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == 'B') {
      KpTemp = KpTemp - 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '7') {
      KpTemp = KpTemp + 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '*') {
      KpTemp = KpTemp - 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '8') {
      KpTemp = KpTemp + 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '0') {
      KpTemp = KpTemp - 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Kp:"));
  lcd.print(KpTemp);
  lcd.setCursor(0, 1);
  lcd.print(F("Keep:1 Discard:2"));

  while (Key != '2') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      Kp = KpTemp;
      EEPROM_writeDouble(KpAddress, Kp);
      myPID.SetTunings(Kp, Ki, Kd);
      Key = '2';
    }
  }
}