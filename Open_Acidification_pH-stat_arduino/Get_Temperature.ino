// ************************************************
// Get temperature reading from PT100
// ************************************************
void Get_Temperature() {
  uint16_t rtd = max.readRTD();
  float ratio = rtd;
  ratio /= 32768;
  tempnow = max.temperature(100, RREF) + tempcorr;

  total = total - readings[readIndex];  // Delete oldest temperature reading
  readings[readIndex] = tempnow;        // Add new temperature reading to the array
  total = total + readings[readIndex];  // Add the temperature reading to the total
  readIndex = readIndex + 1;            // advance to the next position in the array

  if (readIndex >= NUM_READINGS) {  // if we're at the end of the array...
    readIndex = 0;                  // ...wrap around to the beginning
  }

  temp = total / NUM_READINGS;  // calculate the average
  Serial.print(F("Temperature = "));
  Serial.println(temp);
  Serial.print(F("Resistance = "));
  Serial.println(RREF * ratio, 5);
}