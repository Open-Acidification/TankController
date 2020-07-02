// Adding for Time//////////////////////////////////////////////////////////////////////////////////////////////////////
void digitalClockDisplay() {
  // digital clock display of the time
  DateTime now = rtc.now();
  Serial.print(now.hour());
  printDigits(now.minute());
  printDigits(now.second());
  Serial.print(F(" "));
  Serial.print(now.day());
  Serial.print(F("-"));
  Serial.print(now.month());
  Serial.print(F("-"));
  Serial.print(now.year());
  Serial.println();
}
void LCDupdate() {
  lcd.setCursor(3, 0);
  lcd.print(pHDisplay, 3);
  lcd.setCursor(11, 0);
  lcd.print(phset, 3);
  lcd.setCursor(0, 1);
  lcd.setCursor(2, 1);
  lcd.print(temp, 2);
  lcd.setCursor(11, 1);
  lcd.print(tempset, 2);
  lcd.setCursor(9, 1);
  if (heat == 0) {
    lcd.print("C");
  }
  if (heat == 1) {
    lcd.print("H");
  }
}
