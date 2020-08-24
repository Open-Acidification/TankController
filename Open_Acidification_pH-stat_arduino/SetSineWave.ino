// ************************************************
// Set Amplitude and Frequency of Sine Wave
// ************************************************

void SetSineWave() {
  wdt_disable();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Amplitude:"));
  lcd.setCursor(0, 1);
  lcd.print(F(" .   "));

  Key = customKeypad.waitForKey();
  double newAmplitude = Key - '0';
  lcd.setCursor(0, 1);
  lcd.print(Key);

  Key = customKeypad.waitForKey();
  newAmplitude = ((Key - '0') * 0.1) + newAmplitude;
  lcd.setCursor(2, 1);
  lcd.print(Key);
  Serial.print(F("Tenths place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  newAmplitude = ((Key - '0') * 0.01) + newAmplitude;
  lcd.setCursor(3, 1);
  lcd.print(Key);
  Serial.print(F("Hundreths place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  newAmplitude = ((Key - '0') * 0.001) + newAmplitude;
  lcd.setCursor(4, 1);
  lcd.print(Key);
  Serial.print(F("Thousanths place: "));
  Serial.println(Key);
  lcd.setCursor(10, 1);
  lcd.print(newAmplitude, 3);

  amplitudeSet = newAmplitude;

  SaveAmplitudeSet();

  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Frequency:"));
  lcd.setCursor(0, 1);
  lcd.print(F(" .   "));

  Key = customKeypad.waitForKey();
  double newFrequency = Key - '0';
  lcd.setCursor(0, 1);
  lcd.print(Key);

  Key = customKeypad.waitForKey();
  newFrequency = ((Key - '0') * 0.1) + newFrequency;
  lcd.setCursor(2, 1);
  lcd.print(Key);
  Serial.print(F("Tenths place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  newFrequency = ((Key - '0') * 0.01) + newFrequency;
  lcd.setCursor(3, 1);
  lcd.print(Key);
  Serial.print(F("Hundreths place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  newFrequency = ((Key - '0') * 0.001) + newFrequency;
  lcd.setCursor(4, 1);
  lcd.print(Key);
  Serial.print(F("Thousandths place: "));
  Serial.println(Key);
  lcd.setCursor(10, 1);
  lcd.print(newFrequency, 3);

  frequencySet = newFrequency;

  SaveFrequencySet();

  delay(1000);

  lcd.clear();
  lcd.print(F("pH="));
  lcd.setCursor(0, 1);  // Display position
  lcd.print(F("T="));   // display"Temp="
  wdt_enable(WDTO_8S);
}