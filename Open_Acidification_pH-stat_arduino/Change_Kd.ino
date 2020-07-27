
// ************************************************
// Change Kd value
// ************************************************

void Change_Kd() {
  double KdTemp;
  KdTemp = Kd;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Kd:"));
  lcd.setCursor(0, 1);
  lcd.print(KdTemp);
  Key = NO_KEY;
  while (Key != '#') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      KdTemp = KdTemp + 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }
    if (Key == '4') {
      KdTemp = KdTemp - 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '2') {
      KdTemp = KdTemp + 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '5') {
      KdTemp = KdTemp - 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '3') {
      KdTemp = KdTemp + 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '6') {
      KdTemp = KdTemp - 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == 'A') {
      KdTemp = KdTemp + 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == 'B') {
      KdTemp = KdTemp - 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '7') {
      KdTemp = KdTemp + 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '*') {
      KdTemp = KdTemp - 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '8') {
      KdTemp = KdTemp + 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '0') {
      KdTemp = KdTemp - 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Kd:"));
  lcd.print(KdTemp);
  lcd.setCursor(0, 1);
  lcd.print(F("Keep:1 Discard:2"));

  while (Key != '2') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      Kd = KdTemp;
      EEPROM_writeDouble(KdAddress, Kd);
      myPID.SetTunings(Kp, Ki, Kd);
      Key = '2';
    }
  }
}
