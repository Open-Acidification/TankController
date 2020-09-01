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

  key = custom_keypad.waitForKey();
  double new_amplitude = key - '0';
  lcd.setCursor(0, 1);
  lcd.print(key);

  key = custom_keypad.waitForKey();
  new_amplitude = ((key - '0') * 0.1) + new_amplitude;
  lcd.setCursor(2, 1);
  lcd.print(key);
  Serial.print(F("Tenths place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  new_amplitude = ((key - '0') * 0.01) + new_amplitude;
  lcd.setCursor(3, 1);
  lcd.print(key);
  Serial.print(F("Hundreths place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  new_amplitude = ((key - '0') * 0.001) + new_amplitude;
  lcd.setCursor(4, 1);
  lcd.print(key);
  Serial.print(F("Thousanths place: "));
  Serial.println(key);
  lcd.setCursor(10, 1);
  lcd.print(new_amplitude, 3);

  amplitude_set = new_amplitude;

  SaveAmplitudeSet();

  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Frequency:"));
  lcd.setCursor(0, 1);
  lcd.print(F(" .   "));

  key = custom_keypad.waitForKey();
  double new_frequency = key - '0';
  lcd.setCursor(0, 1);
  lcd.print(key);

  key = custom_keypad.waitForKey();
  new_frequency = ((key - '0') * 0.1) + new_frequency;
  lcd.setCursor(2, 1);
  lcd.print(key);
  Serial.print(F("Tenths place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  new_frequency = ((key - '0') * 0.01) + new_frequency;
  lcd.setCursor(3, 1);
  lcd.print(key);
  Serial.print(F("Hundreths place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  new_frequency = ((key - '0') * 0.001) + new_frequency;
  lcd.setCursor(4, 1);
  lcd.print(key);
  Serial.print(F("Thousandths place: "));
  Serial.println(key);
  lcd.setCursor(10, 1);
  lcd.print(new_frequency, 3);

  frequency_set = new_frequency;

  SaveFrequencySet();

  delay(1000);

  lcd.clear();
  lcd.print(F("pH="));
  lcd.setCursor(0, 1);  // Display position
  lcd.print(F("T="));   // display"Temp="
  wdt_enable(WDTO_8S);
}