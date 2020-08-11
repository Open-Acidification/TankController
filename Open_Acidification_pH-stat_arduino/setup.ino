
void setup() {
  Serial.begin(2000000);  // set baud rate for the hardware serial port_0 to 9600
  Serial1.begin(9600);
  wdt_disable();

  // Starting Real Time CLock and Setting time
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  if (!rtc.initialized()) {
    Serial.println("RTC is NOT running!");
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
  snprintf(macstr, 18, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  LoadParameters();
  Setpoint = -1 * phset;
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
  delay(1000);
  lcd.clear();

  max.begin(MAX31865_3WIRE);  // start pt100 temperature probe
  Serial1.begin(9600);        // set baud rate for the software serial port to 9600
  inputstring.reserve(10);    // set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);   // set aside some bytes for receiving data from Atlas Scientific pH EZO
  Serial1.print("*OK,0");     // Turn off the returning of OK after command to EZO pH
  Serial1.print('\r');        // add a <CR> to the end of the string
  Serial1.print("C,0");       // Reset pH stamp to continuous measurement: once per second
  Serial1.print('\r');        // add a <CR> to the end of the string
  pinMode(CHILLER, OUTPUT);
  pinMode(CO2_REG, OUTPUT);
  digitalWrite(CHILLER, HIGH);
  digitalWrite(CO2_REG, HIGH);

  /// Check if ethernet is connected/////////////////////////////////////////////////////////////////////
  int ethanswer = 0;
  int ethstart = millis();
  int timdiff = 0;

  lcd.clear();
  // lcd.print(F("Ethernet cable?"));
  // lcd.setCursor(0, 1);
  // lcd.print(F("Yes:1       No:2"));
  // Serial.print("Ethernet cable?  Yes:1  No: 2");
  // while (ethanswer == 0 && timdiff <= 10000) {
  //   char ether = customKeypad.getKey();
  //   if (ether == '1') {
  //     ethanswer = 1;
  //   }
  //   if (ether == '2') {
  //     EthConnect = false;
  //     ethanswer = 1;
  //   }
  //   timdiff = millis() - ethstart;
  // }
  ethanswer = 1;  // temporarily assume Ethernet is always connected
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  if (EthConnect) {
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

    // start the ethernetServer
    ethernetServer.begin();
    Serial.print("ethernetServer is at ");
    Serial.println(Ethernet.localIP());
  }

  // loading Tank ID///////////////////////////////////////////////////////////////////////////////////////
  tankid = EEPROM_readDouble(TANKID_ADDRESS);
  if (isnan(tankid)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Tank ID# is not"));
    lcd.setCursor(0, 1);
    lcd.print(F("set. Set ID now"));
    Serial.println(F("Start Tank ID change"));
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("New Tank ID:"));

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
  }

  // load granularity
  sensor_interval = EEPROM_readDouble(GRANULARITY_ADDRESS);
  if (isnan(sensor_interval)) {
    sensor_interval = 800;  // default granularity for interval between logging
    EEPROM_writeDouble(GRANULARITY_ADDRESS, sensor_interval);
  }

  // load maxDataAge
  maxDataAge = EEPROM_readDouble(MAX_DATA_AGE_ADDRESS);
  if (isnan(maxDataAge)) {
    maxDataAge = 800;  // default max data age
    EEPROM_writeDouble(MAX_DATA_AGE_ADDRESS, maxDataAge);
  }

  // load PH_SERIES_SIZE_ADDRESS;
  phSeriesSize = EEPROM_readDouble(PH_SERIES_SIZE_ADDRESS);
  if (isnan(phSeriesSize)) {
    phSeriesSize = 0;
    EEPROM_writeDouble(PH_SERIES_SIZE_ADDRESS, phSeriesSize);
  }

  // load PH_SERIES_POINTER_ADDRESS;
  phSeriesPointer = EEPROM_readDouble(PH_SERIES_POINTER_ADDRESS);
  if (isnan(phSeriesPointer)) {
    phSeriesPointer = 0;
    EEPROM_writeDouble(PH_SERIES_POINTER_ADDRESS, phSeriesPointer);
  }

  // load TEMP_SERIES_SIZE_ADDRESS;
  tempSeriesSize = EEPROM_readDouble(TEMP_SERIES_SIZE_ADDRESS);
  if (isnan(tempSeriesSize)) {
    tempSeriesSize = 0;
    EEPROM_writeDouble(TEMP_SERIES_SIZE_ADDRESS, tempSeriesSize);
  }

  // load TEMP_SERIES_POINTER_ADDRESS;
  tempSeriesPointer = EEPROM_readDouble(TEMP_SERIES_POINTER_ADDRESS);
  if (isnan(tempSeriesPointer)) {
    tempSeriesPointer = 0;
    EEPROM_writeDouble(TEMP_SERIES_POINTER_ADDRESS, tempSeriesPointer);
  }

  // load phInterval;
  phInterval = EEPROM_readDouble(PH_INTERVAL_ADDRESS);
  if (isnan(PH_INTERVAL_ADDRESS)) {
    phInterval = 0;
    EEPROM_writeDouble(PH_INTERVAL_ADDRESS, phInterval);
  }

  // load phDelay;
  phDelay = EEPROM_readDouble(PH_DELAY_ADDRESS);
  if (isnan(PH_DELAY_ADDRESS)) {
    phDelay = 0;
    EEPROM_writeDouble(PH_DELAY_ADDRESS, phDelay);
  }

  // load tempInterval;
  tempInterval = EEPROM_readDouble(TEMP_INTERVAL_ADDRESS);
  if (isnan(TEMP_INTERVAL_ADDRESS)) {
    tempInterval = 0;
    EEPROM_writeDouble(TEMP_INTERVAL_ADDRESS, tempInterval);
  }

  // load tempDelay;
  tempDelay = EEPROM_readDouble(TEMP_DELAY_ADDRESS);
  if (isnan(TEMP_DELAY_ADDRESS)) {
    tempDelay = 0;
    EEPROM_writeDouble(TEMP_DELAY_ADDRESS, tempDelay);
  }

  /// Starting the SD Card//////////////////////////////////////////////////////////////////////////////////
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  SD.begin(4);

  File root = SD.open("/");
  printDirectory(root, 0);
  // doDirectoryMaintenance();
  /// Starting the SD Card//////////////////////////////////////////////////////////////////////////////////

  // Setting PID parameters////////////////////////////////////////////////////////////////////////////////////////
  myPID.SetTunings(Kp, Ki, Kd);
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(1000);
  myPID.SetOutputLimits(0, WINDOW_SIZE);

  noInterrupts();  // disable all interrupts
  // Run timer2 interrupt every 15 ms
  TCCR2A = 0;
  TCCR2B = 1 << CS22 | 1 << CS21 | 1 << CS20;

  // Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1 << TOIE2;
  interrupts();

  // loading Temp Correction////////////////////////////////////////////////////////////////////////////////////////
  tempcorr = EEPROM_readDouble(TEMP_CORR_ADDRESS);

  if (isnan(tempcorr)) {
    tempcorr = 0;
  }

  // Filling array for temp smoothing///////////////////////////////////////////////////////////////////////
  for (int thisReading = 0; thisReading < NUM_READINGS; thisReading++) {
    readings[thisReading] = 0;
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////

  byte upArrow[8] = {0b00100, 0b01110, 0b10101, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100};
  lcd.createChar(0, upArrow);

  lcd.clear();
  lcd.print(F("pH="));
  lcd.setCursor(0, 1);  // Display position
  lcd.print(F("T="));   // display"Temp="
  wdt_enable(WDTO_8S);
}
