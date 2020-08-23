///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BEGIN LOOP//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  wdt_reset();
  char to_start = customKeypad.getKey();
  if (to_start != NO_KEY) {
    Serial.print(F("To start key: "));
    Serial.println(to_start);
  }
  /// Change pH Setpoint ///////////////////////////////////////////////////////////////////////////////////

  if (to_start == 'A') {
    wdt_disable();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("New pH setpoint:"));
    lcd.setCursor(0, 1);
    lcd.print(F(" .   "));
    Serial.println(F("Step 1"));

    Key = customKeypad.waitForKey();
    double newph = 0;
    newph = Key - '0';
    lcd.setCursor(0, 1);
    lcd.print(Key);
    Serial.print(F("Ones place: "));
    Serial.println(Key);

    Key = customKeypad.waitForKey();
    newph = ((Key - '0') * 0.1) + newph;
    lcd.setCursor(2, 1);
    lcd.print(Key);
    Serial.print(F("Tenths place: "));
    Serial.println(Key);

    Key = customKeypad.waitForKey();
    newph = ((Key - '0') * 0.01) + newph;
    lcd.setCursor(3, 1);
    lcd.print(Key);
    Serial.print(F("Hundreths place: "));
    Serial.println(Key);

    Key = customKeypad.waitForKey();
    newph = ((Key - '0') * 0.001) + newph;
    lcd.setCursor(4, 1);
    lcd.print(Key);
    Serial.print(F("Thousanths place: "));
    Serial.println(Key);
    lcd.setCursor(10, 1);
    lcd.print(newph, 3);

    phset = newph;
    Setpoint = -1 * phset;
    SavepHSet();
    delay(1000);
    Serial.println(F("New pH Set End"));

    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// Change Temperature Setpoint /////////////////////////////////////////////////////////////////////////////

  if (to_start == 'B') {
    wdt_disable();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("New Temp set:"));
    lcd.setCursor(0, 1);
    lcd.print(F("  .  "));
    Serial.println(F("Step 1"));

    Key = customKeypad.waitForKey();
    double newtemp = 0;
    newtemp = (Key - '0') * 10;
    lcd.setCursor(0, 1);
    lcd.print(Key);
    Serial.print(F("Tens place: "));
    Serial.println(Key);

    Key = customKeypad.waitForKey();
    newtemp = (Key - '0') + newtemp;
    lcd.setCursor(1, 1);
    lcd.print(Key);
    Serial.print(F("Ones place: "));
    Serial.println(Key);

    Key = customKeypad.waitForKey();
    newtemp = ((Key - '0') * 0.1) + newtemp;
    lcd.setCursor(3, 1);
    lcd.print(Key);
    Serial.print(F("Tenths place: "));
    Serial.println(Key);

    Key = customKeypad.waitForKey();
    newtemp = ((Key - '0') * 0.01) + newtemp;
    lcd.setCursor(4, 1);
    lcd.print(Key);
    Serial.print(F("Hundreths place: "));
    Serial.println(Key);
    lcd.setCursor(10, 1);
    lcd.print(newtemp, 3);

    tempset = newtemp;
    SaveTempSet();
    delay(1000);
    Serial.println(F("New Temp Set End"));

    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  /// pH One-Point Calibration /////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  int answer = 0;
  int queststart = millis();
  int timdiff = 0;

  if (to_start == 'C') {
    wdt_disable();
    onTime = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Calibration"));
    lcd.setCursor(0, 1);
    lcd.print(F("1-pt:1 2-pt:2"));
    while (answer == 0 && timdiff <= 5000) {
      char answerkey = customKeypad.getKey();
      if (answerkey == '1') {
        OnePointCal();
        break;
      }
      if (answerkey == '2') {
        TwoPointCal();
        break;
      }
      timdiff = millis() - queststart;
    }
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Calibration Management //////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (to_start == 'D') {
    wdt_disable();
    onTime = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Cal Management"));
    lcd.setCursor(0, 1);
    lcd.print(F("Slope:1 Clear:2"));
    while (answer == 0 && timdiff <= 5000) {
      char answerkey = customKeypad.getKey();
      if (answerkey == '1') {
        if (Serial1.available() > 0) {  // if we see that the Atlas Scientific product has sent a character
          Serial.println(F("clearing buffer"));
          char inchar = (char)Serial1.read();  // get the char we just received
        }

        Serial.println(F("pressed 1"));
        GetCalSlope();
        lcd.clear();
        lcd.print("Cal Slope:");
        lcd.setCursor(0, 1);
        lcd.print(slope);
        delay(5000);
        answer = 1;
      }
      if (answerkey == '2') {
        Serial.println(F("pressed 2"));
        Serial1.print(F("Cal,clear"));  // send Calibration clear command to EZO pH stamp
        Serial1.print('\r');            // add a <CR> to the end of the string
        answer = 1;
      }
      timdiff = millis() - queststart;
    }

    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// Change Tank ID /////////////////////////////////////////////////////////////////////////////

  if (to_start == '#') {
    wdt_disable();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Set Tank ID#:"));
    Serial.println(F("Start Tank ID change"));

    Key = customKeypad.waitForKey();
    tankid = (Key - '0') * 10;
    lcd.setCursor(0, 1);
    lcd.print(Key);
    Serial.print(F("Tens place: "));
    Serial.println(Key);

    Key = customKeypad.waitForKey();
    tankid = (Key - '0') + tankid;
    lcd.setCursor(1, 1);
    lcd.print(Key);
    Serial.print(F("Ones place: "));
    Serial.println(Key);

    EEPROM_writeDouble(TANKID_ADDRESS, tankid);
    delay(1000);
    Serial.println(F("Tank ID change End"));

    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// Change Google Sheet Interval /////////////////////////////////////////////////////////////////////////////

  if (to_start == '*') {
    wdt_disable();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("G Sheet Interval"));
    lcd.setCursor(0, 1);
    lcd.print(F("in min: "));
    Key = customKeypad.waitForKey();
    float newinterval;
    newinterval = (Key - '0') * 10;
    lcd.setCursor(8, 1);
    lcd.print(Key);
    Serial.print(F("Tens place: "));
    Serial.println(Key);

    Key = customKeypad.waitForKey();
    newinterval = (Key - '0') + newinterval;
    lcd.setCursor(9, 1);
    lcd.print(Key);
    Serial.print(F("Ones place: "));
    Serial.println(Key);
    delay(1000);

    interval = newinterval * 60000;
    Serial.println(F("New Google Sheet Interval End"));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("in min: "));
    lcd.print(newinterval, 0);
    lcd.setCursor(0, 1);
    lcd.print(F("in ms: "));
    lcd.print(interval, 0);
    delay(3000);
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// See Device Uptime & Current Time/////////////////////////////////////////////////////////////////////////////

  if (to_start == '0') {
    wdt_disable();
    int starttime = millis();
    int nowtime = millis();
    int yearnow;
    DateTime now = rtc.now();
    if (now.year() >= 2000) {
      yearnow = now.year() - 2000;
    }
    if (now.year() < 2000) {
      yearnow = now.year() - 1900;
    }
    while (nowtime <= starttime + 5000) {
      DateTime now = rtc.now();
      nowtime = millis();
      lcd.clear();
      lcd.print(String(now.month()) + "/" + String(now.day()) + "/" + String(yearnow));
      lcd.print(F(" "));
      lcd.print(String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()));
      lcd.setCursor(0, 1);

      int days = floor(millis() / 86400000);
      int hours = floor((millis() - (days * 86400000)) / 3600000);
      int minutes = floor((millis() - (days * 86400000) - (hours * 3600000)) / 60000);
      int seconds = floor((millis() - (days * 86400000) - (hours * 3600000) - (minutes * 60000)) / 1000);
      lcd.print(F("Up d:"));
      lcd.print(days);
      lcd.print(F(" "));
      lcd.print(hours);
      lcd.print(F(":"));
      lcd.print(minutes);
      lcd.print(F(":"));
      lcd.print(seconds);

      delay(980);
    }
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// See Device addresses /////////////////////////////////////////////////////////////////////////////

  if (to_start == '1') {
    wdt_disable();
    lcd.clear();
    lcd.print(F("Unit Information"));
    lcd.setCursor(0, 1);
    lcd.print(F("IP:1 MAC:2 SV:3"));
    while (answer == 0 && timdiff <= 5000) {
      char answerkey = customKeypad.getKey();
      if (answerkey == '1') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Device IP Address:"));
        lcd.setCursor(0, 1);
        lcd.print(Ethernet.localIP());
        delay(7000);
        answer = 1;
      }
      if (answerkey == '2') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("MAC    "));
        lcd.print(macstr);
        lcd.setCursor(0, 1);
        lcd.print(F("ADDRESS "));
        lcd.print(&macstr[9]);
        Serial.print(F("MAC Address: "));
        Serial.println(macstr);
        delay(7000);
        answer = 1;
      }
      if (answerkey == '3') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Software Version"));
        lcd.setCursor(0, 1);
        lcd.print(SOFTVERS, 3);
        delay(7000);
        answer = 1;
      }
      timdiff = millis() - queststart;
    }
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// Reset LCD Screen /////////////////////////////////////////////////////////////////////////////

  if (to_start == '2') {
    Key = NO_KEY;
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
  }

  /// See Tank ID and Log File Name/////////////////////////////////////////////////////////////////////////////

  if (to_start == '3') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Tank ID: "));
    lcd.print(tankid);
    lcd.setCursor(0, 1);
    lcd.print(F("Log: "));
    lcd.print(filename);
    lcd.print(F(".txt"));
    delay(5000);
    Key = NO_KEY;
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
  }

  /// See PID Constants/////////////////////////////////////////////////////////////////////////////

  if (to_start == '4') {
    wdt_disable();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Kp:"));
    lcd.print(Kp);
    lcd.setCursor(0, 1);
    lcd.print(F("Ki:"));
    lcd.print(Ki);
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Kd:"));
    lcd.print(Kd);
    delay(5000);
    Key = NO_KEY;
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// PID Tuning Menu/////////////////////////////////////////////////////////////////////////////

  if (to_start == '5') {
    wdt_disable();
    answer = 0;
    queststart = millis();
    timdiff = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("   PID TUNING   "));
    lcd.setCursor(0, 1);
    lcd.print(F("      MENU      "));
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Start AutoTune:1"));
    lcd.setCursor(0, 1);
    lcd.print(F("Manual Tune:2"));

    while (answer == 0 && timdiff <= 10000) {
      char answerkey = customKeypad.getKey();
      if (answerkey == '1') {
        RunAutoTune();
        answer = 1;
      }
      if (answerkey == '2') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Change Kp:1 Ki:2"));
        lcd.setCursor(0, 1);
        lcd.print(F("Kd:3"));
        while (answerkey == '2') {
          char secondkey = customKeypad.getKey();
          if (secondkey == '1') {
            Change_Kp();
            answerkey = '0';
          }

          if (secondkey == '2') {
            Change_Ki();
            answerkey = '0';
          }

          if (secondkey == '3') {
            Change_Kd();
            answerkey = '0';
          }
        }
        answer = 1;
      }
      if (answerkey == '3') {
        SetSineWave();
        answer = 1;
      }
      timdiff = millis() - queststart;
    }
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }
  /// End PID Tuning Menu/////////////////////////////////////////////////////////////////////////////

  /// Temperature Calibration/////////////////////////////////////////////////////////////////////////////
  if (to_start == '6') {
    wdt_disable();
    Serial.println(F("Temperature Calibration"));
    lcd.clear();
    lcd.print(F("###Calibrate####"));
    lcd.setCursor(0, 1);
    lcd.print(F("##Temperature###"));
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Calibration Temp:"));
    lcd.setCursor(0, 1);
    lcd.print(F("  .  "));

    Key = customKeypad.waitForKey();
    double realtemp;
    realtemp = (Key - '0') * 10;
    lcd.setCursor(0, 1);
    lcd.print(Key);
    Serial.print(F("Tens place: "));
    Serial.println(Key);

    Key = customKeypad.waitForKey();
    realtemp = (Key - '0') + realtemp;
    lcd.setCursor(1, 1);
    lcd.print(Key);
    Serial.print(F("Ones place: "));
    Serial.println(Key);

    Key = customKeypad.waitForKey();
    realtemp = ((Key - '0') * 0.1) + realtemp;
    lcd.setCursor(3, 1);
    lcd.print(Key);
    Serial.print(F("Tenths place: "));
    Serial.println(Key);

    Key = customKeypad.waitForKey();
    realtemp = ((Key - '0') * 0.01) + realtemp;
    lcd.setCursor(4, 1);
    lcd.print(Key);
    Serial.print(F("Hundreths place: "));
    Serial.println(Key);
    lcd.setCursor(10, 1);
    lcd.print(realtemp, 3);
    Serial.print(F("realtemp: "));
    Serial.println(realtemp);
    delay(1000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Place temp probe"));
    lcd.setCursor(0, 1);
    lcd.print(F("in cal solution"));
    delay(5000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Press '#' to cal"));
    while (Key != '#') {
      uint16_t rtd = max.readRTD();
      float ratio = rtd;
      ratio /= 32768;
      tempnow = max.temperature(100, RREF);
      Serial.print(F("Temperature = "));
      Serial.println(temp);

      total = total - readings[readIndex];  // Delete oldest temperature reading
      readings[readIndex] = tempnow;        // Add new temperature reading to the array
      total = total + readings[readIndex];  // Add the temperature reading to the total
      readIndex = readIndex + 1;            // advance to the next position in the array

      if (readIndex >= NUM_READINGS) {  // if we're at the end of the array...
        readIndex = 0;                  // ...wrap around to the beginning
      }

      temp = total / NUM_READINGS;  // calculate the average

      lcd.setCursor(0, 1);
      lcd.print(F("Temp="));
      lcd.print(temp, 3);
      delay(1000);
      Key = customKeypad.getKey();
    }
    Key = NO_KEY;
    tempcorr = realtemp - temp;
    Serial.print(F("realtemp: "));
    Serial.println(realtemp);
    Serial.print(F("measured temp: "));
    Serial.println(temp);
    Serial.print(F("tempcorr: "));
    Serial.println(tempcorr);

    EEPROM_writeDouble(TEMP_CORR_ADDRESS, tempcorr);

    Serial.println(F("Temp Calibration End"));

    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// Manual Set Time/////////////////////////////////////////////////////////////////////////////
  if (to_start == '7') {
    wdt_disable();
    ManualTime();
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// Enable PID/////////////////////////////////////////////////////////////////////////////
  bool pidrun = true;
  if (to_start == '8') {
    wdt_disable();
    answer = 0;
    queststart = millis();
    timdiff = 0;
    lcd.clear();
    lcd.print(F("Enable PID?"));
    lcd.setCursor(0, 1);
    lcd.print(F("Yes:1       No:2"));
    while (answer == 0 && timdiff <= 5000) {
      char answerkey = customKeypad.getKey();
      if (answerkey == '1') {
        pidrun = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("PID Enabled"));
        delay(3000);
        answer = 1;
      }
      if (answerkey == '2') {
        pidrun = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("PID DISABLED"));
        delay(3000);
        answer = 1;
      }
      timdiff = millis() - queststart;
    }
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// Set Chill or Heat /////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (to_start == '9') {
    wdt_disable();
    lcd.clear();
    lcd.print(F("Chill or Heat?"));
    lcd.setCursor(0, 1);
    lcd.print(F("Chill:1   Heat:2"));
    while (answer == 0 && timdiff <= 5000) {
      char answerkey = customKeypad.getKey();
      if (answerkey == '1') {
        heat = 0;
        EEPROM_writeDouble(HEAT_ADDRESS, 0);
        answer = 1;
      }
      if (answerkey == '2') {
        heat = 1;
        EEPROM_writeDouble(HEAT_ADDRESS, 1);
        answer = 1;
      }
      timdiff = millis() - queststart;
    }
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Main Running Loop /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  unsigned long sensor_currentMillis = millis();
  if (sensor_currentMillis - sensor_previousMillis >= sensor_interval) {
    sensor_previousMillis = sensor_currentMillis;

    Get_pH();
    Get_Temperature();
    Set_Temp_Comp();
    Set_Chiller();
    updateGoals();
    if (!pidrun) {
      if (pH > phset) {
        onTime = 10000;
      }
      if (pH <= phset) {
        onTime = 0;
      }
    }
    if (pidrun) {
      myPID.Compute();
      onTime = Output;
    }
    LCDupdate();
    unsigned long second_currentMillis = millis();
    if (second_currentMillis - second_previousMillis >= second_interval) {
      second_previousMillis = second_currentMillis;
      LogToSD();
    }
    digitalClockDisplay();
    Serial.print(F("freeMemory()="));
    Serial.println(freeMemory());
    Serial.print(F("Kp:"));
    Serial.print(Kp);
    Serial.print(F(" Ki:"));
    Serial.print(Ki);
    Serial.print(F(" Kd:"));
    Serial.println(Kd);
    Serial.print(F("PID Output (s): "));
    Serial.println(Output / 1000, 1);
    DateTime now = rtc.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(F(" "));
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    if (now.minute() < 10) {
      Serial.print(F("0"));
    }
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    if (now.second() < 10) {
      Serial.print(F("0"));
    }
    Serial.print(now.second(), DEC);
    Serial.println();
    Serial.println(F("FREQUENCY: "));
    frequency = EEPROM_readDouble(FREQUENCY_ADDRESS);
    Serial.println(frequency);
    Serial.println(F("AMPLITUDE: "));
    amplitude = EEPROM_readDouble(AMPLITUDE_ADDRESS);
    Serial.println(amplitude);
    Serial.println(F("SETPOINT: "));
    Serial.println(Setpoint);
    Serial.println(F("SINE SETPOINT: "));
    double frequencyInMillis = frequency * 60 * 1000;
    double SineSetpoint = amplitude * sin(2 * M_PI * frequencyInMillis * millis());
    Serial.println(SineSetpoint);
    Serial.println(F("MILLIS: "));
    Serial.println(millis());
    Serial.println();
    Serial.println();
  }

  // Sending data to Google Sheets////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (EthConnect) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      Serial.println(F("Starting upload to Google Docs"));
      wdt_reset();

      lcd.setCursor(9, 0);
      lcd.write((uint8_t)0);
      pinMode(4, OUTPUT);
      digitalWrite(4, HIGH);

      previousMillis = currentMillis;  // save the last time you updated Google Sheets

      packData();  // packing GET query with data

      bool cxn = false;

      Serial.println(F("connecting..."));
      if (client.connect(API_SERVER, 80)) {
        sendData();
        cxn = true;  // connected = true
      } else {
        Serial.println(F("connection failed"));
      }
      // loop
      while (cxn) {
        if (client.available()) {
          char c = client.read();  // save http header to c
          Serial.print(c);         // print http header to serial monitor
        }
        if (!client.connected()) {
          Serial.println();
          Serial.println(F("disconnecting."));
          Serial.print(F("Temperature Sent :"));
          Serial.println(temp);  // print sent value to serial monitor

          client.stop();
          cxn = false;

          data = "";  // data reset
        }
      }
      Serial.println(F("exiting Google Docs loop"));
      lcd.setCursor(9, 0);
      lcd.write(" ");
    }

    // listen for incoming clients
    EthernetClient RPClient = ethernetServer.available();  // Raspberry Pi Client
    if (RPClient) {
      handleRequest(RPClient);

      // give the web browser time to receive the data
      delay(1000);
      // close the connection:
      RPClient.stop();
      Serial.println(F("RPClient disconnected"));
    }
  }

  // Renewing DHCP lease every so often///////////////////////////////////////////////////////////////////////////////////////
  unsigned long currentMillis = millis();

  if (currentMillis - previousLease >= LEASE_INTERVAL) {
    Ethernet.maintain();
  }

  // Serial.println("");
  pH = -99;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////END MAIN LOOP//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
