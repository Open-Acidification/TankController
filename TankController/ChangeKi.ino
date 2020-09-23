// ************************************************
// Change Ki value
// ************************************************

void ChangeKi() {
  Serial.println(F("starting Ki adjustment routine"));
  double ki_temp;
  ki_temp = Ki;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Ki:"));
  lcd.setCursor(0, 1);
  lcd.print(ki_temp);
  key = NO_KEY;
  Serial.print(F("key: "));
  Serial.println(key);
  while (key != '#') {
    Serial.println(F("In the while loop to wait for key presses"));
    key = custom_keypad.waitForKey();
    Serial.print(F("key: "));
    Serial.println(key);

    switch (key) {
      case '1':
        ki_temp = ki_temp + 10000;
        break;
      case '4':
        ki_temp = ki_temp - 10000;
        break;
      case '2':
        ki_temp = ki_temp + 1000;
        break;
      case '5':
        ki_temp = ki_temp - 1000;
        break;
      case '3':
        ki_temp = ki_temp + 100;
        break;
      case '6':
        ki_temp = ki_temp - 100;
        break;
      case 'A':
        ki_temp = ki_temp + 10;
        break;
      case 'B':
        ki_temp = ki_temp - 10;
        break;
      case '7':
        ki_temp = ki_temp + 1;
        break;
      case '*':
        ki_temp = ki_temp - 1;
        break;
      case '8':
        ki_temp = ki_temp + 0.1;
        break;
      case '0':
        ki_temp = ki_temp - 0.1;
        break;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Ki:"));
    lcd.setCursor(0, 1);
    lcd.print(ki_temp, 2);
    Serial.print(F("New Ki: "));
    Serial.println(ki_temp);
  }
  Serial.println(F("out of the Ki adjusting while loop"));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Ki:"));
  lcd.print(ki_temp);
  lcd.setCursor(0, 1);
  lcd.print(F("Keep:1 Discard:2"));

  while (key != '2') {
    key = custom_keypad.waitForKey();
    if (key == '1') {
      Ki = ki_temp;
      EepromWriteDouble(KI_ADDRESS, Ki);
      my_pid.SetTunings(Kp, Ki, Kd);
      key = '2';
    }
  }
}
