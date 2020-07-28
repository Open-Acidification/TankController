// Adding for Time//////////////////////////////////////////////////////////////////////////////////////////////////////////////
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