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

    switch (Key) {
      case '1':
        KpTemp = KpTemp + 10000;
        break;
      case '4':
        KpTemp = KpTemp - 10000;
        break;
      case '2':
        KpTemp = KpTemp + 1000;
        break;
      case '5':
        KpTemp = KpTemp - 1000;
        break;
      case '3':
        KpTemp = KpTemp + 100;
        break;
      case '6':
        KpTemp = KpTemp - 100;
        break;
      case 'A':
        KpTemp = KpTemp + 10;
        break;
      case 'B':
        KpTemp = KpTemp - 10;
        break;
      case '7':
        KpTemp = KpTemp + 1;
        break;
      case '*':
        KpTemp = KpTemp - 1;
        break;
      case '8':
        KpTemp = KpTemp + 0.1;
        break;
      case '0':
        KpTemp = KpTemp - 0.1;
        break;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Kp:"));
    lcd.setCursor(0, 1);
    lcd.print(KpTemp, 2);
    Serial.print(F("New Kp: "));
    Serial.println(KpTemp);
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
      EEPROM_writeDouble(KP_ADDRESS, Kp);
      myPID.SetTunings(Kp, Ki, Kd);
      Key = '2';
    }
  }
}
