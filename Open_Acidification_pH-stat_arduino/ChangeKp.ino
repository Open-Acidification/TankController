// ************************************************
// Change Kp value
// ************************************************

void ChangeKp() {
  double kp_temp;
  kp_temp = Kp;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Kp:"));
  lcd.setCursor(0, 1);
  lcd.print(kp_temp);
  key = NO_KEY;
  while (key != '#') {
    key = custom_keypad.waitForKey();

    switch (key) {
      case '1':
        kp_temp = kp_temp + 10000;
        break;
      case '4':
        kp_temp = kp_temp - 10000;
        break;
      case '2':
        kp_temp = kp_temp + 1000;
        break;
      case '5':
        kp_temp = kp_temp - 1000;
        break;
      case '3':
        kp_temp = kp_temp + 100;
        break;
      case '6':
        kp_temp = kp_temp - 100;
        break;
      case 'A':
        kp_temp = kp_temp + 10;
        break;
      case 'B':
        kp_temp = kp_temp - 10;
        break;
      case '7':
        kp_temp = kp_temp + 1;
        break;
      case '*':
        kp_temp = kp_temp - 1;
        break;
      case '8':
        kp_temp = kp_temp + 0.1;
        break;
      case '0':
        kp_temp = kp_temp - 0.1;
        break;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Kp:"));
    lcd.setCursor(0, 1);
    lcd.print(kp_temp, 2);
    Serial.print(F("New Kp: "));
    Serial.println(kp_temp);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Kp:"));
  lcd.print(kp_temp);
  lcd.setCursor(0, 1);
  lcd.print(F("Keep:1 Discard:2"));

  while (key != '2') {
    key = custom_keypad.waitForKey();
    if (key == '1') {
      Kp = kp_temp;
      EepromWriteDouble(KP_ADDRESS, Kp);
      my_pid.SetTunings(Kp, Ki, Kd);
      key = '2';
    }
  }
}
