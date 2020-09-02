// ************************************************
// Get pH reading from Atlas pH EZO
// ************************************************
void GetPh() {
  Serial1.print(F("R"));  // Ask EZO pH stamp for pH reading
  Serial1.print('\r');    // add a <CR> to the end of the string
  while (pH == -99) {
    if (input_string_complete) {      // if a string from the PC has been received in its entirety
      Serial1.print(input_string);    // send that string to the Atlas Scientific product
      Serial1.print('\r');            // add a <CR> to the end of the string
      input_string = "";              // clear the string
      input_string_complete = false;  // reset the flag used to tell if we have received a completed string from the PC
    }

    if (Serial1.available() > 0) {         // if we see that the Atlas Scientific product has sent a character
      char inchar = (char)Serial1.read();  // get the char we just received
      sensor_string += inchar;             // add the char to the var called sensor_string
      if (inchar == '\r') {                // if the incoming character is a <CR>
        sensor_string_complete = true;     // set the flag
      }
    }

    if (sensor_string_complete == true) {  // if a string from the Atlas Scientific product has been received in its entirety
      if (isdigit(sensor_string[0])) {     // if the first character in the string is a digit
        pH = sensor_string.toFloat();      // convert the string to a floating point number so it can be evaluated by the Arduino
        Serial.print(F("pH = "));
        Serial.println(pH, 3);
      }
      sensor_string = "";              // clear the string
      sensor_string_complete = false;  // reset the flag used to tell if we have received a completed string from the Atlas Scientific product
    }
  }
  input = -1 * pH;
  ph_display = pH;
}
