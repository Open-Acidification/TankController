// ************************************************
// Get temperature reading from PT100
// ************************************************
void GetTemperature() {
  uint16_t rtd = max.readRTD();
  float ratio = rtd;
  ratio /= 32768;
  temp_now = max.temperature(100, RREF) + temp_corr;

  total = total - readings[read_index];  // Delete oldest temperature reading
  readings[read_index] = temp_now;        // Add new temperature reading to the array
  total = total + readings[read_index];  // Add the temperature reading to the total
  read_index = read_index + 1;            // advance to the next position in the array

  if (read_index >= NUM_READINGS) {  // if we're at the end of the array...
    read_index = 0;                  // ...wrap around to the beginning
  }

  temp = total / NUM_READINGS;  // calculate the average
  Serial.print(F("Temperature = "));
  Serial.println(temp);
  Serial.print(F("Resistance = "));
  Serial.println(RREF * ratio, 5);
}