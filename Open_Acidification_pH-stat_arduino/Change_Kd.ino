
// ************************************************
// Change Kd value
// ************************************************

void ChangeKd() {
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

    switch (Key) {
      case '1':
        KdTemp = KdTemp + 10000;
        break;
      case '4':
        KdTemp = KdTemp - 10000;
        break;
      case '2':
        KdTemp = KdTemp + 1000;
        break;
      case '5':
        KdTemp = KdTemp - 1000;
        break;
      case '3':
        KdTemp = KdTemp + 100;
        break;
      case '6':
        KdTemp = KdTemp - 100;
        break;
      case 'A':
        KdTemp = KdTemp + 10;
        break;
      case 'B':
        KdTemp = KdTemp - 10;
        break;
      case '7':
        KdTemp = KdTemp + 1;
        break;
      case '*':
        KdTemp = KdTemp - 1;
        break;
      case '8':
        KdTemp = KdTemp + 0.1;
        break;
      case '0':
        KdTemp = KdTemp - 0.1;
        break;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Kd:"));
    lcd.setCursor(0, 1);
    lcd.print(KdTemp, 2);
    Serial.print(F("New Kd: "));
    Serial.println(KdTemp);
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
      EepromWriteDouble(KD_ADDRESS, Kd);
      myPID.SetTunings(Kp, Ki, Kd);
      Key = '2';
    }
  }
}
