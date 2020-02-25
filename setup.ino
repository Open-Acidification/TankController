
void setup()
{
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  Serial1.begin(9600);
  wdt_disable();

  // Starting Real Time CLock and Setting time
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));   // set the RTC to the date & time this sketch was compiled
  }

  // Store MAC address in EEPROM
  if (EEPROM.read(44) == '#') {
    for (int i = 3; i < 6; i++) {
      mac[i] = EEPROM.read(i + 44);
    }
    Serial.println(F("MAC Address found in EEPROM and read"));
  }
  else {
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
  lcd.createChar(0, ol1);
  lcd.createChar(1, ol2);
  lcd.createChar(2, ol3);
  lcd.createChar(3, ol4);
  lcd.createChar(4, ol5);
  lcd.createChar(5, ol6);
  lcd.createChar(6, ol7);
  lcd.createChar(7, ol8);
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
  delay(5000);
  lcd.clear();

  max.begin(MAX31865_3WIRE);                          //start pt100 temperature probe
  Serial1.begin(9600);                               //set baud rate for the software serial port to 9600
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific pH EZO
  Serial1.print("*OK,0");                              //Turn off the returning of OK after command to EZO pH
  Serial1.print('\r');                               //add a <CR> to the end of the string
  Serial1.print("C,0");                              //Reset pH stamp to continuous measurement: once per second
  Serial1.print('\r');                               //add a <CR> to the end of the string
  pinMode(chiller, OUTPUT);
  pinMode(co2reg, OUTPUT);
  digitalWrite(chiller, HIGH);
  digitalWrite(co2reg, HIGH);

  ///Check if ethernet is connected/////////////////////////////////////////////////////////////////////
  int ethanswer = 0;
  int ethstart = millis();
  int timdiff = 0;

  lcd.clear();
  lcd.print(F("Ethernet cable?"));
  lcd.setCursor(0, 1);
  lcd.print(F("Yes:1       No:2"));
  Serial.print("Ethernet cable?  Yes:1  No: 2");
  while (ethanswer == 0 && timdiff <= 10000) {
    char ether = customKeypad.getKey();
    if (ether == '1') {
      ethanswer = 1;
    }
    if (ether == '2') {
      EthConnect = false;
      ethanswer = 1;
    }
    timdiff = millis() - ethstart;
  }
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


  }


  ///Starting the SD Card//////////////////////////////////////////////////////////////////////////////////
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  SD.begin(4);

  DateTime now = rtc.now();
  filename = String(now.year() - 2000) + "-" + now.month() + "-" + now.day();
  file_full = filename + ".txt";

  myFile = SD.open(file_full, FILE_WRITE);
  myFile.println("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd");
  myFile.close();
  SD_previousMillis = millis();
  ///Starting the SD Card//////////////////////////////////////////////////////////////////////////////////


  //loading Tank ID///////////////////////////////////////////////////////////////////////////////////////
  tankid = EEPROM_readDouble(tankidAddress);


  if (isnan(tankid))
  {
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

    EEPROM_writeDouble(tankidAddress, tankid);
    delay(1000);
    Serial.println(F("Tank ID change End"));
  }

  // load granularity
  sensor_interval = EEPROM_readDouble(granularityAddress);
  if (isnan(sensor_interval)) {
    sensor_interval = 800; // default granularity for interval between logging
    EEPROM_writeDouble(granularityAddress, sensor_interval);
  }

  // load maxDataAge
  maxDataAge = EEPROM_readDouble(granularityAddress);
  if (isnan(maxDataAge)) {
    maxDataAge = 800; // default max data age
    EEPROM_writeDouble(maxDataAgeAddress, maxDataAge);
  }

  //Setting PID parameters////////////////////////////////////////////////////////////////////////////////////////
  myPID.SetTunings(Kp, Ki, Kd);
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(1000);
  myPID.SetOutputLimits(0, WindowSize);

  // Run timer2 interrupt every 15 ms
  TCCR2A = 0;
  TCCR2B = 1 << CS22 | 1 << CS21 | 1 << CS20;

  //Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1 << TOIE2;



  //loading Temp Correction////////////////////////////////////////////////////////////////////////////////////////
  tempcorr = EEPROM_readDouble(tempcorrAddress);

  if (isnan(tempcorr))
  {
    tempcorr = 0;
  }


  //Filling array for temp smoothing///////////////////////////////////////////////////////////////////////
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////


  byte upArrow[8] = {
    0b00100,
    0b01110,
    0b10101,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100
  };
  lcd.createChar(0, upArrow);

  lcd.clear();
  lcd.print(F("pH="));
  lcd.setCursor(0, 1) ;          //Display position
  lcd.print(F("T="));            //display"Temp="
  wdt_enable(WDTO_8S);
}