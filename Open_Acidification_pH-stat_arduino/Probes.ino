// ************************************************
// Called by ISR every 15ms to drive the output
// ************************************************
void DriveOutput() {
  long now = millis();
  // Set the output
  // "on time" is proportional to the PID output
  if (now - windowStartTime > WindowSize) {  // time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  if ((onTime > 100) && (onTime > (now - windowStartTime))) {
    digitalWrite(co2reg, LOW);  // OPEN CO2 solenoid
  } else {
    digitalWrite(co2reg, HIGH);  // CLOSE CO2 solenoid
  }
}
// ************************************************
// Get pH reading from Atlas pH EZO
// ************************************************
void Get_pH() {
  Serial1.print("R");   // Ask EZO pH stamp for pH reading
  Serial1.print('\r');  // add a <CR> to the end of the string
  while (pH == -99) {
    if (input_string_complete) {      // if a string from the PC has been received in its entirety
      Serial1.print(inputstring);     // send that string to the Atlas Scientific product
      Serial1.print('\r');            // add a <CR> to the end of the string
      inputstring = "";               // clear the string
      input_string_complete = false;  // reset the flag used to tell if we have received a completed string from the PC
    }

    if (Serial1.available() > 0) {         // if we see that the Atlas Scientific product has sent a character
      char inchar = (char)Serial1.read();  // get the char we just received
      sensorstring += inchar;              // add the char to the var called sensorstring
      if (inchar == '\r') {                // if the incoming character is a <CR>
        sensor_string_complete = true;     // set the flag
      }
    }

    if (sensor_string_complete == true) {  // if a string from the Atlas Scientific product has been received in its entirety
      if (isdigit(sensorstring[0])) {      // if the first character in the string is a digit
        pH = sensorstring.toFloat();       // convert the string to a floating point number so it can be evaluated by the Arduino
        Serial.print(F("pH = "));
        Serial.println(pH, 3);
      }
      sensorstring = "";               // clear the string
      sensor_string_complete = false;  // reset the flag used to tell if we have received a completed string from the Atlas Scientific product
    }
  }
  Input = -1 * pH;
  pHDisplay = pH;
}
// ************************************************
// Send temperature compensation to Atlas pH EZO
// ************************************************
void Set_Temp_Comp() {
  if (temp > 0 && temp < 100) {
    tempcomp = pretempcomp + String(temp, 2);
  } else {
    tempcomp = pretempcomp + 20;
  }
  Serial.println(tempcomp);
  Serial1.print(tempcomp);  // send that string to the Atlas Scientific product
  Serial1.print('\r');      // add a <CR> to the end of the string
}
// Get Calibration Slope
void GetCalSlope() {
  Serial.println("Shut Down pH Reading");
  delay(1000);
  Serial1.print("Slope,?");  // Sending request for Calibration Slope
  Serial1.print('\r');       // add a <CR> to the end of the string
  Serial.println("Asked for slope");
  WaitForString = true;
  slope = "";
  while (WaitForString == true) {  // Into a loop that will wait for the response
    Serial.println("Waiting for response");
    if (Serial1.available() > 0) {  // if we see that the Atlas Scientific product has sent a character
      Serial.println("Receiving response");
      char inchar = (char)Serial1.read();  // get the char we just received
      sensorstring += inchar;              // add the char to the var called sensorstring
      if (inchar == '\r') {                // if the incoming character is a <CR>
        sensor_string_complete = true;     // set the flag
      }
    }

    // if a string from the Atlas Scientific product has been received in its entirety
    if (sensor_string_complete == true) {
      Serial.println("Response complete");
      sloperaw = sensorstring;  // Store raw slope string into another
      slope = sloperaw;
      slope.remove(0, 7);  // removing the first 7 characters of the slope string
      slope[slope.length() - 1] = ' ';
      sensorstring = "";  // clear the string
      // reset the flag used to tell if we have received a completed string from the Atlas Scientific product
      sensor_string_complete = false;
      Serial.print("Raw String: ");
      Serial.println(sloperaw);
      Serial.print("Calibration Slope: ");
      Serial.println(slope);
    }

    if (slope.length() > 3) {
      WaitForString = false;
    }
    Serial.println(WaitForString);
  }
  Serial.print("finshing GetCalSlope");
  WaitForString = true;  // Resetting waiting flag
}
// ************************************************
// Set chiller state
// ************************************************

void Set_Chiller() {
  if (heat == 0) {
    unsigned long chiller_currentMillis = millis();
    // pause 30 seconds between swtiching chiller on and off to prevent damage to chiller
    if (chiller_currentMillis - chiller_previousMillis >= chiller_interval) {
      chiller_previousMillis = chiller_currentMillis;
      // if the observed temperature is greater than or equal the temperature setpoint plus .05 degree
      if (temp >= tempset + 0.05) {
        Serial.println(F("chiller on"));  // print chiller state to serial
        digitalWrite(chiller, LOW);
      }
      if (temp <= tempset - 0.05) {        // see if temperature is lower than .05 below setpoint
        Serial.println(F("chiller off"));  // print chiller state to serial
        digitalWrite(chiller, HIGH);
      }
    }
  }
  if (heat == 1) {
    // if the observed temperature is less than or equal the temperature setpoint plus .05 degree
    if (temp <= tempset + 0.05) {
      Serial.println(F("chiller on"));  // print chiller state to serial
      digitalWrite(chiller, LOW);
    }
    if (temp >= tempset - 0.05) {        // see if temperature is greater than or equal to .05 below setpoint
      Serial.println(F("chiller off"));  // print chiller state to serial
      digitalWrite(chiller, HIGH);
    }
  }
}
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

  if (readIndex >= numReadings) {  // if we're at the end of the array...
    readIndex = 0;                 // ...wrap around to the beginning
  }

  temp = total / numReadings;  // calculate the average
  Serial.print(F("Temperature = "));
  Serial.println(temp);
  Serial.print(F("Resistance = "));
  Serial.println(RREF * ratio, 5);
}
