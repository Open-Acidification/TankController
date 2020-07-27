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

    if (sensor_string_complete == true) {  // if a string from the Atlas Scientific product has been received in its entirety
      Serial.println("Response complete");
      sloperaw = sensorstring;  // Store raw slope string into another
      slope = sloperaw;
      slope.remove(0, 7);  // removing the first 7 characters of the slope string
      slope[slope.length() - 1] = ' ';
      sensorstring = "";               // clear the string
      sensor_string_complete = false;  // reset the flag used to tell if we have received a completed string from the Atlas Scientific product
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