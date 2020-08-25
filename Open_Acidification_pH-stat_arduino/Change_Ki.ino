// ************************************************
// Change Ki value
// ************************************************

void ChangeKi() {
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

    switch (Key) {
      case '1':
        KiTemp = KiTemp + 10000;
        break;
      case '4':
        KiTemp = KiTemp - 10000;
        break;
      case '2':
        KiTemp = KiTemp + 1000;
        break;
      case '5':
        KiTemp = KiTemp - 1000;
        break;
      case '3':
        KiTemp = KiTemp + 100;
        break;
      case '6':
        KiTemp = KiTemp - 100;
        break;
      case 'A':
        KiTemp = KiTemp + 10;
        break;
      case 'B':
        KiTemp = KiTemp - 10;
        break;
      case '7':
        KiTemp = KiTemp + 1;
        break;
      case '*':
        KiTemp = KiTemp - 1;
        break;
      case '8':
        KiTemp = KiTemp + 0.1;
        break;
      case '0':
        KiTemp = KiTemp - 0.1;
        break;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Ki:"));
    lcd.setCursor(0, 1);
    lcd.print(KiTemp, 2);
    Serial.print(F("New Ki: "));
    Serial.println(KiTemp);
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
      EEPROM_WriteDouble(KI_ADDRESS, Ki);
      myPID.SetTunings(Kp, Ki, Kd);
      Key = '2';
    }
  }
}
