// ************************************************
// Manual Time Set
// ************************************************
void ManualTime() {
  lcd.clear();
  lcd.print(F("What year is it?"));
  unsigned long year_now;

  key = custom_keypad.waitForKey();
  year_now = (key - '0') * 1000;
  lcd.setCursor(0, 1);
  lcd.print(key);
  Serial.print(F("Thousands place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  year_now = ((key - '0') * 100) + year_now;
  lcd.setCursor(1, 1);
  lcd.print(key);
  Serial.print(F("Hundreds place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  year_now = ((key - '0') * 10) + year_now;
  lcd.setCursor(2, 1);
  lcd.print(key);
  Serial.print(F("Tens place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  year_now = (key - '0') + year_now;
  lcd.setCursor(3, 1);
  lcd.print(key);
  Serial.print(F("Ones place: "));
  Serial.println(key);
  delay(500);

  lcd.clear();
  lcd.print(F("Month? (01-12)"));
  unsigned long Monthnow;

  key = custom_keypad.waitForKey();
  Monthnow = (key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(key);
  Serial.print(F("Ten place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  Monthnow = (key - '0') + Monthnow;
  lcd.setCursor(1, 1);
  lcd.print(key);
  Serial.print(F("Ones place: "));
  Serial.println(key);
  delay(500);

  lcd.clear();
  lcd.print(F("Day? (01-31)"));
  unsigned long day_now;

  key = custom_keypad.waitForKey();
  day_now = (key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(key);
  Serial.print(F("Ten place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  day_now = (key - '0') + day_now;
  lcd.setCursor(1, 1);
  lcd.print(key);
  Serial.print(F("Ones place: "));
  Serial.println(key);
  delay(500);

  lcd.clear();
  lcd.print(F("Hour? (00-23)"));
  unsigned long hour_now;

  key = custom_keypad.waitForKey();
  hour_now = (key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(key);
  Serial.print(F("Ten place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  hour_now = (key - '0') + hour_now;
  lcd.setCursor(1, 1);
  lcd.print(key);
  Serial.print(F("Ones place: "));
  Serial.println(key);
  delay(500);

  lcd.clear();
  lcd.print(F("Minute? (00-59)"));
  unsigned long min_now;

  key = custom_keypad.waitForKey();
  min_now = (key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(key);
  Serial.print(F("Ten place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  min_now = (key - '0') + min_now;
  lcd.setCursor(1, 1);
  lcd.print(key);
  Serial.print(F("Ones place: "));
  Serial.println(key);
  delay(500);

  setTime(hour_now, min_now, 30, day_now, Monthnow, year_now);
  rtc.adjust(DateTime(year_now, Monthnow, day_now, hour_now, min_now, 30));

  int start_time = millis();
  int now_time = millis();
  while (now_time <= start_time + 5 * SECOND_IN_MILLIS) {
    now_time = millis();
    lcd.clear();
    lcd.print(String(month()) + "/" + String(day()) + "/" + String(year()));
    lcd.setCursor(0, 1);
    lcd.print(String(hour()) + ":" + String(minute()) + ":" + String(second()));
    delay(950);
  }
  lcd.clear();
}