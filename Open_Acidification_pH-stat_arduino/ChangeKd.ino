
// ************************************************
// Change Kd value
// ************************************************

void ChangeKd() {
  double kd_temp;
  kd_temp = Kd;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Kd:"));
  lcd.setCursor(0, 1);
  lcd.print(kd_temp);
  key = NO_KEY;
  while (key != '#') {
    key = custom_keypad.waitForKey();

    switch (key) {
      case '1':
        kd_temp = kd_temp + 10000;
        break;
      case '4':
        kd_temp = kd_temp - 10000;
        break;
      case '2':
        kd_temp = kd_temp + 1000;
        break;
      case '5':
        kd_temp = kd_temp - 1000;
        break;
      case '3':
        kd_temp = kd_temp + 100;
        break;
      case '6':
        kd_temp = kd_temp - 100;
        break;
      case 'A':
        kd_temp = kd_temp + 10;
        break;
      case 'B':
        kd_temp = kd_temp - 10;
        break;
      case '7':
        kd_temp = kd_temp + 1;
        break;
      case '*':
        kd_temp = kd_temp - 1;
        break;
      case '8':
        kd_temp = kd_temp + 0.1;
        break;
      case '0':
        kd_temp = kd_temp - 0.1;
        break;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Kd:"));
    lcd.setCursor(0, 1);
    lcd.print(kd_temp, 2);
    Serial.print(F("New Kd: "));
    Serial.println(kd_temp);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Kd:"));
  lcd.print(kd_temp);
  lcd.setCursor(0, 1);
  lcd.print(F("Keep:1 Discard:2"));

  while (key != '2') {
    key = custom_keypad.waitForKey();
    if (key == '1') {
      Kd = kd_temp;
      EepromWriteDouble(KD_ADDRESS, Kd);
      my_pid.SetTunings(Kp, Ki, Kd);
      key = '2';
    }
  }
}
