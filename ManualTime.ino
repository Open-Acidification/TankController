// ************************************************
// Manual Time Set
// ************************************************
void ManualTime()
{
  lcd.clear();
  lcd.print(F("What year is it?"));
  unsigned long Yearnow;

  Key = customKeypad.waitForKey();
  Yearnow = (Key - '0') * 1000;
  lcd.setCursor(0, 1);
  lcd.print(Key);
  Serial.print(F("Thousands place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  Yearnow = ((Key - '0') * 100) + Yearnow;
  lcd.setCursor(1, 1);
  lcd.print(Key);
  Serial.print(F("Hundreds place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  Yearnow = ((Key - '0') * 10) + Yearnow;
  lcd.setCursor(2, 1);
  lcd.print(Key);
  Serial.print(F("Tens place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  Yearnow = (Key - '0') + Yearnow;
  lcd.setCursor(3, 1);
  lcd.print(Key);
  Serial.print(F("Ones place: "));
  Serial.println(Key);
  delay(500);

  lcd.clear();
  lcd.print(F("Month? (01-12)"));
  unsigned long Monthnow;

  Key = customKeypad.waitForKey();
  Monthnow = (Key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(Key);
  Serial.print(F("Ten place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  Monthnow = (Key - '0') + Monthnow;
  lcd.setCursor(1, 1);
  lcd.print(Key);
  Serial.print(F("Ones place: "));
  Serial.println(Key);
  delay(500);

  lcd.clear();
  lcd.print(F("Day? (01-31)"));
  unsigned long Daynow;

  Key = customKeypad.waitForKey();
  Daynow = (Key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(Key);
  Serial.print(F("Ten place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  Daynow = (Key - '0') + Daynow;
  lcd.setCursor(1, 1);
  lcd.print(Key);
  Serial.print(F("Ones place: "));
  Serial.println(Key);
  delay(500);

  lcd.clear();
  lcd.print(F("Hour? (00-23)"));
  unsigned long Hournow;

  Key = customKeypad.waitForKey();
  Hournow = (Key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(Key);
  Serial.print(F("Ten place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  Hournow = (Key - '0') + Hournow;
  lcd.setCursor(1, 1);
  lcd.print(Key);
  Serial.print(F("Ones place: "));
  Serial.println(Key);
  delay(500);

  lcd.clear();
  lcd.print(F("Minute? (00-59)"));
  unsigned long Minnow;

  Key = customKeypad.waitForKey();
  Minnow = (Key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(Key);
  Serial.print(F("Ten place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  Minnow = (Key - '0') + Minnow;
  lcd.setCursor(1, 1);
  lcd.print(Key);
  Serial.print(F("Ones place: "));
  Serial.println(Key);
  delay(500);

  setTime(Hournow, Minnow, 30, Daynow, Monthnow, Yearnow);
  rtc.adjust(DateTime(Yearnow, Monthnow, Daynow, Hournow, Minnow, 30));


  int starttime = millis();
  int nowtime = millis();
  while (nowtime <= starttime + 5000)
  {
    nowtime = millis();
    lcd.clear();
    lcd.print(String(month()) + "/" + String(day()) + "/" + String(year()));
    lcd.setCursor(0, 1);
    lcd.print(String(hour()) + ":" + String(minute()) + ":" + String(second()));
    delay(950);
  }
  lcd.clear();
}