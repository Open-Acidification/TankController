///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BEGIN LOOP//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  tank->loop();
  wdt_reset();
  char to_start = custom_keypad.getKey();
  if (to_start != NO_KEY) {
    Serial.print(F("To start key: "));
    Serial.println(to_start);
  }
  /// Change pH set_point ///////////////////////////////////////////////////////////////////////////////////

  if (to_start == 'A') {
    wdt_disable();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("New pH setpoint:"));
    lcd.setCursor(0, 1);
    lcd.print(F(" .   "));
    Serial.println(F("Step 1"));

    key = custom_keypad.waitForKey();
    double newph = key - '0';
    lcd.setCursor(0, 1);
    lcd.print(key);
    Serial.print(F("Ones place: "));
    Serial.println(key);

    key = custom_keypad.waitForKey();
    newph = ((key - '0') * 0.1) + newph;
    lcd.setCursor(2, 1);
    lcd.print(key);
    Serial.print(F("Tenths place: "));
    Serial.println(key);

    key = custom_keypad.waitForKey();
    newph = ((key - '0') * 0.01) + newph;
    lcd.setCursor(3, 1);
    lcd.print(key);
    Serial.print(F("Hundreths place: "));
    Serial.println(key);

    key = custom_keypad.waitForKey();
    newph = ((key - '0') * 0.001) + newph;
    lcd.setCursor(4, 1);
    lcd.print(key);
    Serial.print(F("Thousanths place: "));
    Serial.println(key);
    lcd.setCursor(10, 1);
    lcd.print(newph, 3);

    ph_set = newph;
    set_point = -1 * ph_set;
    SavePhSet();
    delay(ONE_SECOND_DELAY_IN_MILLIS);
    Serial.println(F("New pH Set End"));

    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// Change Temperature set_point /////////////////////////////////////////////////////////////////////////////

  if (to_start == 'B') {
    wdt_disable();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("New Temp set:"));
    lcd.setCursor(0, 1);
    lcd.print(F("  .  "));
    Serial.println(F("Step 1"));

    key = custom_keypad.waitForKey();
    double newtemp = (key - '0') * 10;
    lcd.setCursor(0, 1);
    lcd.print(key);
    Serial.print(F("Tens place: "));
    Serial.println(key);

    key = custom_keypad.waitForKey();
    newtemp = (key - '0') + newtemp;
    lcd.setCursor(1, 1);
    lcd.print(key);
    Serial.print(F("Ones place: "));
    Serial.println(key);

    key = custom_keypad.waitForKey();
    newtemp = ((key - '0') * 0.1) + newtemp;
    lcd.setCursor(3, 1);
    lcd.print(key);
    Serial.print(F("Tenths place: "));
    Serial.println(key);

    key = custom_keypad.waitForKey();
    newtemp = ((key - '0') * 0.01) + newtemp;
    lcd.setCursor(4, 1);
    lcd.print(key);
    Serial.print(F("Hundreths place: "));
    Serial.println(key);
    lcd.setCursor(10, 1);
    lcd.print(newtemp, 3);

    temp_set = newtemp;
    SaveTempSet();
    delay(ONE_SECOND_DELAY_IN_MILLIS);
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
  int quest_start = millis();
  int time_diff = 0;

  if (to_start == 'C') {
    wdt_disable();
    on_time = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Calibration"));
    lcd.setCursor(0, 1);
    lcd.print(F("1-pt:1 2-pt:2"));
    while (answer == 0 && time_diff <= 5 * SECOND_IN_MILLIS) {
      char answer_key = custom_keypad.getKey();
      if (answer_key == '1') {
        OnePointCal();
        break;
      }
      if (answer_key == '2') {
        TwoPointCal();
        break;
      }
      time_diff = millis() - quest_start;
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
    on_time = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Cal Management"));
    lcd.setCursor(0, 1);
    lcd.print(F("Slope:1 Clear:2"));
    while (answer == 0 && time_diff <= 5 * SECOND_IN_MILLIS) {
      char answer_key = custom_keypad.getKey();
      if (answer_key == '1') {
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
        delay(FIVE_SECOND_DELAY_IN_MILLIS);
        answer = 1;
      }
      if (answer_key == '2') {
        Serial.println(F("pressed 2"));
        Serial1.print(F("Cal,clear"));  // send Calibration clear command to EZO pH stamp
        Serial1.print('\r');            // add a <CR> to the end of the string
        answer = 1;
      }
      time_diff = millis() - quest_start;
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

    key = custom_keypad.waitForKey();
    tank_id = (key - '0') * 10;
    lcd.setCursor(0, 1);
    lcd.print(key);
    Serial.print(F("Tens place: "));
    Serial.println(key);

    key = custom_keypad.waitForKey();
    tank_id = (key - '0') + tank_id;
    lcd.setCursor(1, 1);
    lcd.print(key);
    Serial.print(F("Ones place: "));
    Serial.println(key);

    EepromWriteDouble(TANKID_ADDRESS, tank_id);
    delay(ONE_SECOND_DELAY_IN_MILLIS);
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
    key = custom_keypad.waitForKey();
    float new_interval = (key - '0') * 10;
    lcd.setCursor(8, 1);
    lcd.print(key);
    Serial.print(F("Tens place: "));
    Serial.println(key);

    key = custom_keypad.waitForKey();
    new_interval = (key - '0') + new_interval;
    lcd.setCursor(9, 1);
    lcd.print(key);
    Serial.print(F("Ones place: "));
    Serial.println(key);
    delay(ONE_SECOND_DELAY_IN_MILLIS);

    interval = new_interval * MINUTE_IN_MILLIS;
    Serial.println(F("New Google Sheet Interval End"));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("in min: "));
    lcd.print(new_interval, 0);
    lcd.setCursor(0, 1);
    lcd.print(F("in ms: "));
    lcd.print(interval, 0);
    delay(THREE_SECOND_DELAY_IN_MILLIS);
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// See Device Uptime & Current Time/////////////////////////////////////////////////////////////////////////////

  if (to_start == '0') {
    wdt_disable();
    int start_time = millis();
    int now_time = millis();
    int year_now;
    DateTime now = rtc.now();
    if (now.year() >= 2000) {
      year_now = now.year() - 2000;
    }
    if (now.year() < 2000) {
      year_now = now.year() - 1900;
    }
    while (now_time <= start_time + 5 * SECOND_IN_MILLIS) {
      DateTime now = rtc.now();
      now_time = millis();
      lcd.clear();
      lcd.print(String(now.month()) + "/" + String(now.day()) + "/" + String(year_now));
      lcd.print(F(" "));
      lcd.print(String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()));
      lcd.setCursor(0, 1);

      unsigned long days = floor(millis() / DAY_IN_MILLIS);
      unsigned long hours = floor((millis() - (days * DAY_IN_MILLIS)) / HOUR_IN_MILLIS);
      unsigned long minutes = floor((millis() - (days * DAY_IN_MILLIS) - (hours * HOUR_IN_MILLIS)) / MINUTE_IN_MILLIS);
      unsigned long seconds = floor((millis() - (days * DAY_IN_MILLIS) - (hours * HOUR_IN_MILLIS) - (minutes * MINUTE_IN_MILLIS)) / SECOND_IN_MILLIS);
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
    while (answer == 0 && time_diff <= 5 * SECOND_IN_MILLIS) {
      char answer_key = custom_keypad.getKey();
      if (answer_key == '1') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Device IP Address:"));
        lcd.setCursor(0, 1);
        lcd.print(Ethernet.localIP());
        delay(SEVEN_SECOND_DELAY_IN_MILLIS);
        answer = 1;
      }
      if (answer_key == '2') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("MAC    "));
        lcd.print(mac_str);
        lcd.setCursor(0, 1);
        lcd.print(F("ADDRESS "));
        lcd.print(&mac_str[9]);
        Serial.print(F("MAC Address: "));
        Serial.println(mac_str);
        delay(SEVEN_SECOND_DELAY_IN_MILLIS);
        answer = 1;
      }
      if (answer_key == '3') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Software Version"));
        lcd.setCursor(0, 1);
        lcd.print(SOFTWARE_VERSION);
        delay(SEVEN_SECOND_DELAY_IN_MILLIS);
        answer = 1;
      }
      time_diff = millis() - quest_start;
    }
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1);  // Display position
    lcd.print(F("T="));   // display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// Reset LCD Screen /////////////////////////////////////////////////////////////////////////////

  if (to_start == '2') {
    key = NO_KEY;
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
    lcd.print(tank_id);
    lcd.setCursor(0, 1);
    lcd.print(F("Log: "));
    lcd.print(file_name);
    lcd.print(F(".txt"));
    delay(FIVE_SECOND_DELAY_IN_MILLIS);
    key = NO_KEY;
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
    delay(FIVE_SECOND_DELAY_IN_MILLIS);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Kd:"));
    lcd.print(Kd);
    delay(FIVE_SECOND_DELAY_IN_MILLIS);
    key = NO_KEY;
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
    quest_start = millis();
    time_diff = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("   PID TUNING   "));
    lcd.setCursor(0, 1);
    lcd.print(F("      MENU      "));
    delay(ONE_SECOND_DELAY_IN_MILLIS);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Start AutoTune:1"));
    lcd.setCursor(0, 1);
    lcd.print(F("Manual Tune:2"));

    while (answer == 0 && time_diff <= 10 * SECOND_IN_MILLIS) {
      char answer_key = custom_keypad.getKey();
      if (answer_key == '1') {
        RunAutoTune();
        answer = 1;
      }
      if (answer_key == '2') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Change Kp:1 Ki:2"));
        lcd.setCursor(0, 1);
        lcd.print(F("Kd:3"));
        while (answer_key == '2') {
          char secondkey = custom_keypad.getKey();
          if (secondkey == '1') {
            ChangeKp();
            answer_key = '0';
          }

          if (secondkey == '2') {
            ChangeKi();
            answer_key = '0';
          }

          if (secondkey == '3') {
            ChangeKd();
            answer_key = '0';
          }
        }
        answer = 1;
      }
      if (answer_key == '3') {
        SetSineWave();
        answer = 1;
      }
      time_diff = millis() - quest_start;
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
    delay(THREE_SECOND_DELAY_IN_MILLIS);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Calibration Temp:"));
    lcd.setCursor(0, 1);
    lcd.print(F("  .  "));

    key = custom_keypad.waitForKey();
    double real_temp = (key - '0') * 10;
    lcd.setCursor(0, 1);
    lcd.print(key);
    Serial.print(F("Tens place: "));
    Serial.println(key);

    key = custom_keypad.waitForKey();
    real_temp = (key - '0') + real_temp;
    lcd.setCursor(1, 1);
    lcd.print(key);
    Serial.print(F("Ones place: "));
    Serial.println(key);

    key = custom_keypad.waitForKey();
    real_temp = ((key - '0') * 0.1) + real_temp;
    lcd.setCursor(3, 1);
    lcd.print(key);
    Serial.print(F("Tenths place: "));
    Serial.println(key);

    key = custom_keypad.waitForKey();
    real_temp = ((key - '0') * 0.01) + real_temp;
    lcd.setCursor(4, 1);
    lcd.print(key);
    Serial.print(F("Hundreths place: "));
    Serial.println(key);
    lcd.setCursor(10, 1);
    lcd.print(real_temp, 3);
    Serial.print(F("real_temp: "));
    Serial.println(real_temp);
    delay(ONE_SECOND_DELAY_IN_MILLIS);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Place temp probe"));
    lcd.setCursor(0, 1);
    lcd.print(F("in cal solution"));
    delay(FIVE_SECOND_DELAY_IN_MILLIS);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Press '#' to cal"));
    while (key != '#') {
      uint16_t rtd = max.readRTD();
      float ratio = rtd;
      ratio /= 32768;
      temp_now = max.temperature(100, RREF);
      Serial.print(F("Temperature = "));
      Serial.println(temp);

      total = total - readings[read_index];  // Delete oldest temperature reading
      readings[read_index] = temp_now;       // Add new temperature reading to the array
      total = total + readings[read_index];  // Add the temperature reading to the total
      read_index = read_index + 1;           // advance to the next position in the array

      if (read_index >= NUM_READINGS) {  // if we're at the end of the array...
        read_index = 0;                  // ...wrap around to the beginning
      }

      temp = total / NUM_READINGS;  // calculate the average

      lcd.setCursor(0, 1);
      lcd.print(F("Temp="));
      lcd.print(temp, 3);
      delay(ONE_SECOND_DELAY_IN_MILLIS);
      key = custom_keypad.getKey();
    }
    key = NO_KEY;
    temp_corr = real_temp - temp;
    Serial.print(F("real_temp: "));
    Serial.println(real_temp);
    Serial.print(F("measured temp: "));
    Serial.println(temp);
    Serial.print(F("temp_corr: "));
    Serial.println(temp_corr);

    EepromWriteDouble(TEMP_CORR_ADDRESS, temp_corr);

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
  if (to_start == '8') {
    wdt_disable();
    answer = 0;
    quest_start = millis();
    time_diff = 0;
    lcd.clear();
    lcd.print(F("Enable PID?"));
    lcd.setCursor(0, 1);
    lcd.print(F("Yes:1       No:2"));
    while (answer == 0 && time_diff <= 5 * SECOND_IN_MILLIS) {
      char answer_key = custom_keypad.getKey();
      if (answer_key == '1') {
        pid_run = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("PID Enabled"));
        delay(THREE_SECOND_DELAY_IN_MILLIS);
        answer = 1;
      }
      if (answer_key == '2') {
        pid_run = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("PID DISABLED"));
        delay(THREE_SECOND_DELAY_IN_MILLIS);
        answer = 1;
      }
      time_diff = millis() - quest_start;
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
    while (answer == 0 && time_diff <= 5 * SECOND_IN_MILLIS) {
      char answer_key = custom_keypad.getKey();
      if (answer_key == '1') {
        heat = 0;
        EepromWriteDouble(HEAT_ADDRESS, 0);
        answer = 1;
      }
      if (answer_key == '2') {
        heat = 1;
        EepromWriteDouble(HEAT_ADDRESS, 1);
        answer = 1;
      }
      time_diff = millis() - quest_start;
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

  unsigned long sensor_current_millis = millis();
  if (sensor_current_millis - sensor_previous_millis >= sensor_interval) {
    sensor_previous_millis = sensor_current_millis;

    GetPh();
    GetTemperature();
    SetTempComp();
    SetChiller();
    UpdateGoals();
    if (!pid_run) {
      if (pH > ph_set) {
        on_time = 10 * SECOND_IN_MILLIS;
      }
      if (pH <= ph_set) {
        on_time = 0;
      }
    }
    if (pid_run) {
      my_pid.Compute();
      on_time = output;
    }
    LcdUpdate();
    unsigned long second_current_millis = millis();
    if (second_current_millis - second_previous_millis >= second_interval) {
      second_previous_millis = second_current_millis;
      LogToSD();
    }
    DigitalClockDisplay();
    Serial.print(F("freeMemory()="));
    Serial.println(freeMemory());
    Serial.print(F("Kp:"));
    Serial.print(Kp);
    Serial.print(F(" Ki:"));
    Serial.print(Ki);
    Serial.print(F(" Kd:"));
    Serial.println(Kd);
    Serial.print(F("PID output (s): "));
    Serial.println(output / 1000, 1);
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
    frequency = EepromReadDouble(FREQUENCY_ADDRESS);
    Serial.println(frequency);
    Serial.println(F("AMPLITUDE: "));
    amplitude = EepromReadDouble(AMPLITUDE_ADDRESS);
    Serial.println(amplitude);
    Serial.println(F("SETPOINT: "));
    Serial.println(set_point);
    Serial.println(F("SINE SETPOINT: "));
    double frequency_in_millis = frequency * MINUTE_IN_MILLIS;
    double sine_set_point = amplitude * sin(2 * M_PI * frequency_in_millis * millis());
    Serial.println(sine_set_point);
    Serial.println(F("MILLIS: "));
    Serial.println(millis());
    Serial.println();
    Serial.println();
  }

  // Sending data to Google Sheets////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (eth_connect) {
    unsigned long current_millis = millis();

    if (current_millis - previous_millis >= interval) {
      Serial.println(F("Starting upload to Google Docs"));
      wdt_reset();

      lcd.setCursor(9, 0);
      lcd.write((uint8_t)0);
      pinMode(4, OUTPUT);
      digitalWrite(4, HIGH);

      previous_millis = current_millis;  // save the last time you updated Google Sheets

      PackData();  // packing GET query with data

      bool cxn = false;

      Serial.println(F("connecting..."));
      if (client.connect(API_SERVER, 80)) {
        SendData();
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
    EthernetClient rpc_client = ethernet_server.available();  // Raspberry Pi Client
    if (rpc_client) {
      HandleRequest(rpc_client);

      // give the web browser time to receive the data
      delay(ONE_SECOND_DELAY_IN_MILLIS);
      // close the connection:
      rpc_client.stop();
      Serial.println(F("rpc_client disconnected"));
    }
  }

  // Renewing DHCP lease every so often///////////////////////////////////////////////////////////////////////////////////////
  unsigned long current_millis = millis();

  if (current_millis - previous_lease >= LEASE_INTERVAL) {
    Ethernet.maintain();
  }

  // Serial.println("");
  pH = -99;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////END MAIN LOOP//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
