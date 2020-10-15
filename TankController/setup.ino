
void setup() {
  tank->setup();
  Serial.begin(2000000);  // set baud rate for the hardware serial port_0 to 9600
  Serial1.begin(9600);
  wdt_disable();

  // Starting Real Time CLock and Setting time
  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    while (1)
      ;
  }

  if (!rtc.initialized()) {
    Serial.println(F("RTC is NOT running!"));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // set the RTC to the date & time this sketch was compiled
  }

  // Store MAC address in EEPROM
  if (EEPROM.read(44) == '#') {
    for (int i = 3; i < 6; i++) {
      mac[i] = EEPROM.read(i + 44);
    }
    Serial.println(F("MAC Address found in EEPROM and read"));
  } else {
    Serial.println(F("No MAC Address Found in EEPROM. Generating New MAC."));
    for (int i = 3; i < 6; i++) {
      mac[i] = TrueRandom.randomByte();
      EEPROM.write(i + 44, mac[i]);
    }
    EEPROM.write(44, '#');
  }
  snprintf(mac_str, 18, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  LoadParameters();
  set_point = -1 * ph_set;
  lcd.createChar(0, OL1);
  lcd.createChar(1, OL2);
  lcd.createChar(2, OL3);
  lcd.createChar(3, OL4);
  lcd.createChar(4, OL5);
  lcd.createChar(5, OL6);
  lcd.createChar(6, OL7);
  lcd.createChar(7, OL8);
  lcd.begin(16, 2);
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.write(byte(2));
  lcd.write(byte(3));
  lcd.print(F(" ONTHANK LAB"));
  lcd.setCursor(0, 1);
  lcd.write(byte(4));
  lcd.write(byte(5));
  lcd.write(byte(6));
  lcd.write(byte(7));
  lcd.print(F(" OA CONTROL"));
  delay(ONE_SECOND_DELAY_IN_MILLIS);
  lcd.clear();

  max.begin(MAX31865_3WIRE);  // start pt100 temperature probe
  Serial1.begin(9600);        // set baud rate for the software serial port to 9600
  input_string.reserve(10);   // set aside some bytes for receiving data from the PC
  sensor_string.reserve(30);  // set aside some bytes for receiving data from Atlas Scientific pH EZO
  Serial1.print(F("*OK,0"));  // Turn off the returning of OK after command to EZO pH
  Serial1.print('\r');        // add a <CR> to the end of the string
  Serial1.print(F("C,0"));    // Reset pH stamp to continuous measurement: once per second
  Serial1.print('\r');        // add a <CR> to the end of the string
  pinMode(CHILLER, OUTPUT);
  pinMode(CO2_REG, OUTPUT);
  digitalWrite(CHILLER, HIGH);
  digitalWrite(CO2_REG, HIGH);

  /// Check if ethernet is connected/////////////////////////////////////////////////////////////////////
  int eth_answer = 0;
  int eth_start = millis();
  int time_diff = 0;

  lcd.clear();
  // lcd.print(F("Ethernet cable?"));
  // lcd.setCursor(0, 1);
  // lcd.print(F("Yes:1       No:2"));
  // Serial.print("Ethernet cable?  Yes:1  No: 2");
  // while (eth_answer == 0 && time_diff <= 10000) {
  //   char ether = custom_keypad.getKey();
  //   if (ether == '1') {
  //     eth_answer = 1;
  //   }
  //   if (ether == '2') {
  //     eth_connect = false;
  //     eth_answer = 1;
  //   }
  //   time_diff = millis() - eth_start;
  // }
  eth_answer = 1;  // temporarily assume Ethernet is always connected
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  if (eth_connect) {
    lcd.clear();
    lcd.write(byte(0));
    lcd.write(byte(1));
    lcd.write(byte(2));
    lcd.write(byte(3));
    lcd.print(F("Ethernet is"));
    lcd.setCursor(0, 1);
    lcd.write(byte(4));
    lcd.write(byte(5));
    lcd.write(byte(6));
    lcd.write(byte(7));
    lcd.print(F("connecting.."));
    Serial.print(F("MAC Address: "));
    Serial.print(mac[1]);
    Serial.print(F(":"));
    Serial.print(mac[2]);
    Serial.print(F(":"));
    Serial.print(mac[3]);
    Serial.print(F(":"));
    Serial.print(mac[4]);
    Serial.print(F(":"));
    Serial.print(mac[5]);
    Serial.print(F(":"));
    Serial.println(mac[6]);
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
    if (Ethernet.begin(mac) == 0) {
      Serial.println(F("Failed to configure Ethernet using DHCP"));
      Ethernet.begin(mac, ip);
    }

    // start the ethernet_server
    ethernet_server.begin();
    Serial.print(F("ethernet_server is at "));
    Serial.println(Ethernet.localIP());
  }

  // loading Tank ID///////////////////////////////////////////////////////////////////////////////////////
  tank_id = EepromReadDouble(TANKID_ADDRESS);
  if (isnan(tank_id)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Tank ID# is not"));
    lcd.setCursor(0, 1);
    lcd.print(F("set. Set ID now"));
    Serial.println(F("Start Tank ID change"));
    delay(THREE_SECOND_DELAY_IN_MILLIS);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("New Tank ID:"));

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
  }

  // load granularity
  sensor_interval = EepromReadDouble(GRANULARITY_ADDRESS);
  if (isnan(sensor_interval)) {
    sensor_interval = 800;  // default granularity for interval between logging
    EepromWriteDouble(GRANULARITY_ADDRESS, sensor_interval);
  }

  // load max_data_age
  max_data_age = EepromReadDouble(MAX_DATA_AGE_ADDRESS);
  if (isnan(max_data_age)) {
    max_data_age = 800;  // default max data age
    EepromWriteDouble(MAX_DATA_AGE_ADDRESS, max_data_age);
  }

  // load PH_SERIES_SIZE_ADDRESS;
  ph_series_size = EepromReadDouble(PH_SERIES_SIZE_ADDRESS);
  if (isnan(ph_series_size)) {
    ph_series_size = 0;
    EepromWriteDouble(PH_SERIES_SIZE_ADDRESS, ph_series_size);
  }

  // load PH_SERIES_POINTER_ADDRESS;
  ph_series_pointer = EepromReadDouble(PH_SERIES_POINTER_ADDRESS);
  if (isnan(ph_series_pointer)) {
    ph_series_pointer = 0;
    EepromWriteDouble(PH_SERIES_POINTER_ADDRESS, ph_series_pointer);
  }

  // load TEMP_SERIES_SIZE_ADDRESS;
  temp_series_size = EepromReadDouble(TEMP_SERIES_SIZE_ADDRESS);
  if (isnan(temp_series_size)) {
    temp_series_size = 0;
    EepromWriteDouble(TEMP_SERIES_SIZE_ADDRESS, temp_series_size);
  }

  // load TEMP_SERIES_POINTER_ADDRESS;
  temp_series_pointer = EepromReadDouble(TEMP_SERIES_POINTER_ADDRESS);
  if (isnan(temp_series_pointer)) {
    temp_series_pointer = 0;
    EepromWriteDouble(TEMP_SERIES_POINTER_ADDRESS, temp_series_pointer);
  }

  // load ph_interval;
  ph_interval = EepromReadDouble(PH_INTERVAL_ADDRESS);
  if (isnan(PH_INTERVAL_ADDRESS)) {
    ph_interval = 0;
    EepromWriteDouble(PH_INTERVAL_ADDRESS, ph_interval);
  }

  // load ph_delay;
  ph_delay = EepromReadDouble(PH_DELAY_ADDRESS);
  if (isnan(PH_DELAY_ADDRESS)) {
    ph_delay = 0;
    EepromWriteDouble(PH_DELAY_ADDRESS, ph_delay);
  }

  // load temp_interval;
  temp_interval = EepromReadDouble(TEMP_INTERVAL_ADDRESS);
  if (isnan(TEMP_INTERVAL_ADDRESS)) {
    temp_interval = 0;
    EepromWriteDouble(TEMP_INTERVAL_ADDRESS, temp_interval);
  }

  // load temp_delay;
  temp_delay = EepromReadDouble(TEMP_DELAY_ADDRESS);
  if (isnan(TEMP_DELAY_ADDRESS)) {
    temp_delay = 0;
    EepromWriteDouble(TEMP_DELAY_ADDRESS, temp_delay);
  }

  /// Starting the SD Card//////////////////////////////////////////////////////////////////////////////////
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  SD.begin(4);

  File root = SD.open("/");
  PrintDirectory(root, 0);
  // DoDirectoryMaintenance();
  /// Starting the SD Card//////////////////////////////////////////////////////////////////////////////////

  // Setting PID parameters////////////////////////////////////////////////////////////////////////////////////////
  my_pid.SetTunings(Kp, Ki, Kd);
  my_pid.SetMode(AUTOMATIC);
  my_pid.SetSampleTime(1000);
  my_pid.SetOutputLimits(0, WINDOW_SIZE);

  noInterrupts();  // disable all interrupts
  // Run timer2 interrupt every 15 ms
  TCCR2A = 0;
  TCCR2B = 1 << CS22 | 1 << CS21 | 1 << CS20;

  // Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1 << TOIE2;
  interrupts();

  // loading Temp Correction////////////////////////////////////////////////////////////////////////////////////////
  temp_corr = EepromReadDouble(TEMP_CORR_ADDRESS);

  if (isnan(temp_corr)) {
    temp_corr = 0;
  }

  // Filling array for temp smoothing///////////////////////////////////////////////////////////////////////
  for (int this_reading = 0; this_reading < NUM_READINGS; this_reading++) {
    readings[this_reading] = 0;
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////

  byte up_arrow[8] = {0b00100, 0b01110, 0b10101, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100};
  lcd.createChar(0, up_arrow);

  lcd.clear();
  lcd.print(F("pH="));
  lcd.setCursor(0, 1);  // Display position
  lcd.print(F("T="));   // display"Temp="
  wdt_enable(WDTO_8S);
}
