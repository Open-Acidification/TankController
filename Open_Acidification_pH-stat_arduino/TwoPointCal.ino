// ************************************************
// Two Point Calibration
// ************************************************

void TwoPointCal() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Calibration mode"));
  lcd.setCursor(3, 1);
  lcd.print(F("Two-point"));
  delay(5000);

  // Lower Buffer ///
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Higher Buffer pH:"));
  lcd.setCursor(0, 1);
  lcd.print(F("  .   "));

  Key = customKeypad.waitForKey();
  midBuffer = (Key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(Key);
  Serial.print(F("Tens place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  midBuffer = (Key - '0') + midBuffer;
  lcd.setCursor(1, 1);
  lcd.print(Key);
  Serial.print(F("Ones place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  midBuffer = ((Key - '0') * 0.1) + midBuffer;
  lcd.setCursor(3, 1);
  lcd.print(Key);
  Serial.print(F("Tenths place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  midBuffer = ((Key - '0') * 0.01) + midBuffer;
  lcd.setCursor(4, 1);
  lcd.print(Key);
  Serial.print(F("Hundreths place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  midBuffer = ((Key - '0') * 0.001) + midBuffer;
  lcd.setCursor(5, 1);
  lcd.print(Key);
  Serial.print(F("Thousanths place: "));
  Serial.println(Key);
  midcalstring = PRE_MID_CAL_STRING + String(midBuffer);
  Serial.print(midcalstring);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Press '#' to cal"));
  while (Key != '#') {
    /*     Serial1.print("R");                              //Ask for pH reading
         Serial1.print('\r');                               //add a <CR> to the end of the string

         if (Serial1.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
           char inchar = (char)Serial1.read();              //get the char we just received
           sensorstring += inchar;                           //add the char to the var called sensorstring
           if (inchar == '\r') {                             //if the incoming character is a <CR>
             sensor_string_complete = true;                  //set the flag
           }
         }


         if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
           if (isdigit(sensorstring[0])) {                   //if the first character in the string is a digit
             pH = sensorstring.toFloat();                    //convert the string to a floating point number so it can be evaluated by the Arduino
           }
           sensorstring = "";                                //clear the string
           sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
         }
    */
    unsigned long sensor_currentMillis = millis();
    if (sensor_currentMillis - sensor_previousMillis >= sensor_interval) {
      sensor_previousMillis = sensor_currentMillis;

      Get_pH();
      Get_Temperature();
      Set_Temp_Comp();
    }

    lcd.setCursor(0, 1);
    lcd.print(F("pH="));
    lcd.print(pH, 3);

    Key = customKeypad.getKey();
  }
  Key = NO_KEY;

  Serial1.print(midcalstring);  // send that string to the Atlas Scientific product
  Serial1.print('\r');          // add a <CR> to the end of the string

  // High pH Buffer ///
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Lower Buffer pH:"));
  lcd.setCursor(0, 1);
  lcd.print(F("  .   "));

  Key = customKeypad.waitForKey();
  float lowBuffer = (Key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(Key);
  Serial.print(F("Tens place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  lowBuffer = (Key - '0') + lowBuffer;
  lcd.setCursor(1, 1);
  lcd.print(Key);
  Serial.print(F("Ones place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  lowBuffer = ((Key - '0') * 0.1) + lowBuffer;
  lcd.setCursor(3, 1);
  lcd.print(Key);
  Serial.print(F("Tenths place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  lowBuffer = ((Key - '0') * 0.01) + lowBuffer;
  lcd.setCursor(4, 1);
  lcd.print(Key);
  Serial.print(F("Hundreths place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  lowBuffer = ((Key - '0') * 0.001) + lowBuffer;
  lcd.setCursor(5, 1);
  lcd.print(Key);
  Serial.print(F("Thousanths place: "));
  Serial.println(Key);
  String lowcalstring = PRE_LOW_CAL_STRING + String(lowBuffer);
  Serial.print(lowcalstring);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Press '#' to cal"));
  while (Key != '#') {
    /*      Serial1.print("R");                              //Ask for pH reading
          Serial1.print('\r');                               //add a <CR> to the end of the string

          serialEvent();
          serialEvent3();
          if (Serial1.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
            char inchar = (char)Serial1.read();              //get the char we just received
            sensorstring += inchar;                           //add the char to the var called sensorstring
            if (inchar == '\r') {                             //if the incoming character is a <CR>
              sensor_string_complete = true;                  //set the flag
            }
          }


          if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
            if (isdigit(sensorstring[0])) {                   //if the first character in the string is a digit
              pH = sensorstring.toFloat();                    //convert the string to a floating point number so it can be evaluated by the Arduino
            }
            sensorstring = "";                                //clear the string
            sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
          }
    */
    unsigned long sensor_currentMillis = millis();
    if (sensor_currentMillis - sensor_previousMillis >= sensor_interval) {
      sensor_previousMillis = sensor_currentMillis;

      Get_pH();
      Get_Temperature();
      Set_Temp_Comp();
      lcd.setCursor(0, 1);
      lcd.print(F("pH="));
      lcd.print(pH, 3);
    }

    Key = customKeypad.getKey();
  }
  Key = NO_KEY;

  Serial1.print(lowcalstring);  // send that string to the Atlas Scientific product
  Serial1.print('\r');          // add a <CR> to the end of the string

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("###Calibration##"));
  lcd.setCursor(0, 1);
  lcd.print(F("####Complete####"));
  delay(3000);
}