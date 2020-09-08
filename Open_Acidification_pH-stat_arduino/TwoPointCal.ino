// ************************************************
// Two Point Calibration
// ************************************************

void TwoPointCal() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Calibration mode"));
  lcd.setCursor(3, 1);
  lcd.print(F("Two-point"));
  delay(FIVE_SECOND_DELAY_IN_MILLIS);

  // Lower Buffer ///
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Higher Buffer pH:"));
  lcd.setCursor(0, 1);
  lcd.print(F("  .   "));

  key = custom_keypad.waitForKey();
  mid_buffer = (key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(key);
  Serial.print(F("Tens place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  mid_buffer = (key - '0') + mid_buffer;
  lcd.setCursor(1, 1);
  lcd.print(key);
  Serial.print(F("Ones place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  mid_buffer = ((key - '0') * 0.1) + mid_buffer;
  lcd.setCursor(3, 1);
  lcd.print(key);
  Serial.print(F("Tenths place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  mid_buffer = ((key - '0') * 0.01) + mid_buffer;
  lcd.setCursor(4, 1);
  lcd.print(key);
  Serial.print(F("Hundreths place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  mid_buffer = ((key - '0') * 0.001) + mid_buffer;
  lcd.setCursor(5, 1);
  lcd.print(key);
  Serial.print(F("Thousanths place: "));
  Serial.println(key);
  mid_cal_string = PRE_MID_CAL_STRING + String(mid_buffer);
  Serial.print(mid_cal_string);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Press '#' to cal"));
  while (key != '#') {
    /*     Serial1.print("R");                              //Ask for pH reading
         Serial1.print('\r');                               //add a <CR> to the end of the string

         if (Serial1.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
           char inchar = (char)Serial1.read();              //get the char we just received
           sensor_string += inchar;                           //add the char to the var called sensor_string
           if (inchar == '\r') {                             //if the incoming character is a <CR>
             sensor_string_complete = true;                  //set the flag
           }
         }


         if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
           if (isdigit(sensor_string[0])) {                   //if the first character in the string is a digit
             pH = sensor_string.toFloat();                    //convert the string to a floating point number so it can be evaluated by the Arduino
           }
           sensor_string = "";                                //clear the string
           sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
         }
    */
    unsigned long sensor_current_millis = millis();
    if (sensor_current_millis - sensor_previous_millis >= sensor_interval) {
      sensor_previous_millis = sensor_current_millis;

      GetPh();
      GetTemperature();
      SetTempComp();
    }

    lcd.setCursor(0, 1);
    lcd.print(F("pH="));
    lcd.print(pH, 3);

    key = custom_keypad.getKey();
  }
  key = NO_KEY;

  Serial1.print(mid_cal_string);  // send that string to the Atlas Scientific product
  Serial1.print('\r');            // add a <CR> to the end of the string

  // High pH Buffer ///
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Lower Buffer pH:"));
  lcd.setCursor(0, 1);
  lcd.print(F("  .   "));

  key = custom_keypad.waitForKey();
  float low_buffer = (key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(key);
  Serial.print(F("Tens place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  low_buffer = (key - '0') + low_buffer;
  lcd.setCursor(1, 1);
  lcd.print(key);
  Serial.print(F("Ones place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  low_buffer = ((key - '0') * 0.1) + low_buffer;
  lcd.setCursor(3, 1);
  lcd.print(key);
  Serial.print(F("Tenths place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  low_buffer = ((key - '0') * 0.01) + low_buffer;
  lcd.setCursor(4, 1);
  lcd.print(key);
  Serial.print(F("Hundreths place: "));
  Serial.println(key);

  key = custom_keypad.waitForKey();
  low_buffer = ((key - '0') * 0.001) + low_buffer;
  lcd.setCursor(5, 1);
  lcd.print(key);
  Serial.print(F("Thousanths place: "));
  Serial.println(key);
  String low_cal_string = PRE_LOW_CAL_STRING + String(low_buffer);
  Serial.print(low_cal_string);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Press '#' to cal"));
  while (key != '#') {
    /*      Serial1.print("R");                              //Ask for pH reading
          Serial1.print('\r');                               //add a <CR> to the end of the string

          serialEvent();
          serialEvent3();
          if (Serial1.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
            char inchar = (char)Serial1.read();              //get the char we just received
            sensor_string += inchar;                           //add the char to the var called sensor_string
            if (inchar == '\r') {                             //if the incoming character is a <CR>
              sensor_string_complete = true;                  //set the flag
            }
          }


          if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
            if (isdigit(sensor_string[0])) {                   //if the first character in the string is a digit
              pH = sensor_string.toFloat();                    //convert the string to a floating point number so it can be evaluated by the Arduino
            }
            sensor_string = "";                                //clear the string
            sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
          }
    */
    unsigned long sensor_current_millis = millis();
    if (sensor_current_millis - sensor_previous_millis >= sensor_interval) {
      sensor_previous_millis = sensor_current_millis;

      GetPh();
      GetTemperature();
      SetTempComp();
      lcd.setCursor(0, 1);
      lcd.print(F("pH="));
      lcd.print(pH, 3);
    }

    key = custom_keypad.getKey();
  }
  key = NO_KEY;

  Serial1.print(low_cal_string);  // send that string to the Atlas Scientific product
  Serial1.print('\r');            // add a <CR> to the end of the string

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("###Calibration##"));
  lcd.setCursor(0, 1);
  lcd.print(F("####Complete####"));
  delay(THREE_SECOND_DELAY_IN_MILLIS);
}
