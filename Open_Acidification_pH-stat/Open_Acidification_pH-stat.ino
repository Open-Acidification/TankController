
/*
   Version #: 0.196 
   (0.190: Adding Real Time Clock, 
    0.191: Temperature compensation defeat & PT100 resistance to serial monitor, 
    0.192: added fields to SD card output
    0.193: Fixed current time display output to take time from RTC
    0.194: Device ID to the beginning, software version sig figs)
    0.195: Fixing SD card writing stuff (was taking too long and overrunning pH reading time)
    0.196: Adding ability to switch between chilling and heating
   Author: Kirt L Onthank
   Date:2019/7/3
   IDE V1.8.4
   Email:kirt.onthank@wallawalla.edu
*/

#include <Adafruit_MAX31865.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>
#include <Keypad.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#include <TrueRandom.h>
#include <MemoryFree.h>
#include <avr/wdt.h>
#include <Wire.h>
#include "RTClib.h"

String DevID = "v172D35C152EDA6C"; //DeviceID from Pushingbox

#define RREF 430.0
Adafruit_MAX31865 max = Adafruit_MAX31865(45, 43, 41, 39);
RTC_PCF8523 rtc;

double softvers = 0.196;                                        //Software Version

//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Setting MAC Address
char server[] = "api.pushingbox.com"; //pushingbox API server
IPAddress ip(192, 168, 1, 2); //Arduino IP address. Only used when DHCP is turned off.
EthernetClient client; //define 'client' as object
String data; //GET query with data
String SDstring; //what to write to SD card
boolean cxn = false;

float interval = 1200000;                         // interval at which to update Google Sheets (milliseconds)
unsigned long previousMillis = 0 - interval + 30000; // will store last time Google Sheets was updated (-interval+30000 sets first upload for 30 seconds after startup. This eases troubleshooting)
unsigned long chiller_previousMillis = 0;         // will store last time chiller state was checked
float chiller_interval = 30000;                   // interval at which to change chiller state (milliseconds)
unsigned long SD_previousMillis = 0;              // will store last time A new log file was started on the SD card
float SD_interval = 86400000;                     // interval at which to start a new log file (milliseconds)
unsigned long sensor_previousMillis = 0;          // will store last time sensor readings were taken
float sensor_interval = 800;                      // interval at which to start a new log file (milliseconds)
float LeaseInterval = 4 * 86400000;               //Interval at which to renew DHCP lease (First number is days)
unsigned long previousLease = 0;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
String inputstring = "";                          //a string to hold incoming data from the PC
String sensorstring = "";                         //a string to hold the data from the EZO pH circuit
String premidcalstring = "Cal,mid,";
String prelowcalstring = "Cal,low,";
String midcalstring = "";
String lowcalstring = "";
String rightnow;
String pretempcomp = "T,";
String tempcomp;
boolean input_string_complete = false;             //have we received all the data from the PC
boolean sensor_string_complete = false;            //have we received all the data from the Atlas Scientific product
double pH;                                         //used to hold a floating point number that is the pH
double pHDisplay;                                  //used to hold a floating point number that is the pH
double temp;
double tempset;
double phset;
double newph = 0;
double newtemp = 0;
double realtemp;
double tempcorr = 0;
double heat;
double Input, Output, Setpoint;
double Kp;
double Ki;
double Kd;
long onTime = 0;
int WindowSize = 10000;                         // 10 second Time Proportional Output window
unsigned long windowStartTime;
float midBuffer = 0;
float lowBuffer = 0;
float tim = 0.00;
char Key = NO_KEY;
char exitph = NO_KEY;
float newinterval;
int tankid;
boolean EthConnect = true;
byte mac[6] = { 0x90, 0xA2, 0xDA, 0x00, 0x00, 0x00 };
char macstr[18];
boolean pidrun = true;
boolean sensed = false;
#define chiller 47
#define co2reg 49

//Temperature Smoothing/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int numReadings = 10;

double readings[numReadings];      // the readings from the analog input
volatile int readIndex = 0;              // the index of the current reading
volatile double total = 0;                  // the running total
volatile double tempnow = 0;                // the average
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// EEPROM addresses for persisted data///////////////////////////////////////////////////////////////////////////////////////////////////////////
const int pHAddress = 0;
const int tempAddress = 4;
const int tankidAddress = 8;
const int tempcorrAddress = 12;
const int KpAddress = 20;
const int KiAddress = 28;
const int KdAddress = 36;
const int MacAddress = 44;
const int heatAddress = 52;

// End EEPROM addresses for persisted data///////////////////////////////////////////////////////////////////////////////////////////////////////

String filename;
String file_full;

char stdKeys[ROWS][COLS] = {
  { '1' , '2' , '3' , 'A' },
  { '4' , '5' , '6' , 'B' },
  { '7' , '8' , '9' , 'C' },
  { '*' , '0' , '#' , 'D' }
};
byte rowPins[ROWS] = {34, 36, 38, 40};
byte colPins[COLS] = {42, 44, 46, 48};
Keypad customKeypad = Keypad(makeKeymap(stdKeys), rowPins, colPins, ROWS, COLS);
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT); //Starting the PID, Specify the links and initial tuning parameters
File myFile;
const int rs = 24, en = 22, d4 = 26, d5 = 28, d6 = 30, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// ************************************************
// Auto Tune Variables and constants
// ************************************************
byte ATuneModeRemember = 2;

double aTuneStep = 500;
double aTuneNoise = 1;
unsigned int aTuneLookBack = 20;

boolean tuning = false;

PID_ATune aTune(&Input, &Output);


//Stuff for retreiving time/////////////////////////////////////////////////////////////////////////////////////////////////////////
IPAddress timeServer(132, 163, 97, 1); // utcnist.colorado.edu
const int timeZone = -7;  // Pacific Daylight Time (USA)
EthernetUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets
//End Time////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Custom Characters for splash////////////////////////////////
byte ol1[8] = {
  0b00000,
  0b00000,
  0b00001,
  0b00011,
  0b00011,
  0b00111,
  0b00111,
  0b00110
};

byte ol2[8] = {
  0b00111,
  0b11111,
  0b11111,
  0b10011,
  0b11101,
  0b11110,
  0b00101,
  0b11011
};

byte ol3[8] = {
  0b11100,
  0b11111,
  0b01111,
  0b10111,
  0b10110,
  0b01110,
  0b10101,
  0b11011
};

byte ol4[8] = {
  0b00000,
  0b00000,
  0b10000,
  0b11000,
  0b11000,
  0b11100,
  0b11100,
  0b11100
};

byte ol5[8] = {
  0b00111,
  0b00111,
  0b00111,
  0b00011,
  0b00011,
  0b00001,
  0b00000,
  0b00000
};

byte ol6[8] = {
  0b11011,
  0b10101,
  0b01110,
  0b01101,
  0b11101,
  0b11110,
  0b11111,
  0b00111
};

byte ol7[8] = {
  0b11011,
  0b10100,
  0b01111,
  0b10111,
  0b11001,
  0b11111,
  0b11111,
  0b11100
};

byte ol8[8] = {
  0b01100,
  0b11100,
  0b11100,
  0b11000,
  0b11000,
  0b10000,
  0b00000,
  0b00000
};

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
  Serial1.print("C,1");                              //Reset pH stamp to continuous measurement: once per second
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

void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}

void serialEvent3() {                                 //if the hardware serial port_3 receives a char
  sensorstring = Serial1.readStringUntil(13);         //read the string until we see a <CR>
  sensor_string_complete = true;                      //set the flag used to tell if we have received a completed string from the PC
}

// ************************************************
// Timer Interrupt Handlers
// ************************************************
SIGNAL(TIMER2_OVF_vect)
{
  DriveOutput();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///BEGIN LOOP//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
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
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
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
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// pH One-Point Calibration /////////////////////////////////////////////////////////////////////////

  if (to_start == 'C') {
    wdt_disable();
    onTime=0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Calibration mode"));
    lcd.setCursor(3, 1);
    lcd.print(F("One-point"));
    delay(5000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Buffer pH:"));
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
    midcalstring = premidcalstring + String(midBuffer);
    Serial.print(midcalstring);


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Press '#' to cal"));
    while (Key != '#') {

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
      lcd.setCursor(0, 1);
      lcd.print(F("pH="));
      lcd.print(pH, 3);

      Key = customKeypad.getKey();
    }
    Key = NO_KEY;

    Serial1.print(midcalstring);                      //send that string to the Atlas Scientific product
    Serial1.print('\r');                             //add a <CR> to the end of the string

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("###Calibration##"));
    lcd.setCursor(0, 1);
    lcd.print(F("####Complete####"));
    delay(3000);
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
    wdt_enable(WDTO_8S);
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// pH Two-Point Calibration ////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (to_start == 'D') {
    wdt_disable();
    onTime=0;
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
    midcalstring = premidcalstring + String(midBuffer);
    Serial.print(midcalstring);


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Press '#' to cal"));
    while (Key != '#') {

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
      lcd.setCursor(0, 1);
      lcd.print(F("pH="));
      lcd.print(pH, 3);

      Key = customKeypad.getKey();
    }
    Key = NO_KEY;

    Serial1.print(midcalstring);                      //send that string to the Atlas Scientific product
    Serial1.print('\r');                             //add a <CR> to the end of the string

    // High pH Buffer ///
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Lower Buffer pH:"));
    lcd.setCursor(0, 1);
    lcd.print(F("  .   "));

    Key = customKeypad.waitForKey();
    lowBuffer = (Key - '0') * 10;
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
    lowcalstring = prelowcalstring + String(lowBuffer);
    Serial.print(lowcalstring);


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Press '#' to cal"));
    while (Key != '#') {

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
      lcd.setCursor(0, 1);
      lcd.print(F("pH="));
      lcd.print(pH, 3);

      Key = customKeypad.getKey();
    }
    Key = NO_KEY;

    Serial1.print(lowcalstring);                      //send that string to the Atlas Scientific product
    Serial1.print('\r');                             //add a <CR> to the end of the string



    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("###Calibration##"));
    lcd.setCursor(0, 1);
    lcd.print(F("####Complete####"));
    delay(3000);
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
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

    EEPROM_writeDouble(tankidAddress, tankid);
    delay(1000);
    Serial.println(F("Tank ID change End"));

    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
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
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
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
    while (nowtime <= starttime + 5000)
    {
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
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// See Device addresses /////////////////////////////////////////////////////////////////////////////

  int answer = 0;
  int queststart = millis();
  int timdiff = 0;

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
      if (answerkey == '3'){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Software Version"));
        lcd.setCursor(0, 1);
        lcd.print(softvers,3);
        delay(7000);
        answer = 1;
      }
      timdiff = millis() - queststart;
    }
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
    wdt_enable(WDTO_8S);
  }

  /// Reset LCD Screen /////////////////////////////////////////////////////////////////////////////

  if (to_start == '2') {
    Key = NO_KEY;
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
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
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
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
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
    wdt_enable(WDTO_8S);
  }

  ///PID Tuning Menu/////////////////////////////////////////////////////////////////////////////

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
    delay(3000);
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
      timdiff = millis() - queststart;
    }
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
    wdt_enable(WDTO_8S);
  }
  ///End PID Tuning Menu/////////////////////////////////////////////////////////////////////////////



  ///Temperature Calibration/////////////////////////////////////////////////////////////////////////////
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

      total = total - readings[readIndex];                      // Delete oldest temperature reading
      readings[readIndex] = tempnow;                            // Add new temperature reading to the array
      total = total + readings[readIndex];                      // Add the temperature reading to the total
      readIndex = readIndex + 1;                                // advance to the next position in the array

      if (readIndex >= numReadings) {                           // if we're at the end of the array...
        readIndex = 0;                                          // ...wrap around to the beginning
      }

      temp = total / numReadings;                               // calculate the average


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

    EEPROM_writeDouble(tempcorrAddress, tempcorr);


    Serial.println(F("Temp Calibration End"));

    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
    wdt_enable(WDTO_8S);
  }


  ///Manual Set Time/////////////////////////////////////////////////////////////////////////////
  if (to_start == '7') {
    wdt_disable();
    ManualTime();
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
    wdt_enable(WDTO_8S);
  }


  ///Enable PID/////////////////////////////////////////////////////////////////////////////
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
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
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
        EEPROM_writeDouble(heatAddress, 0);
        answer = 1;
      }
      if (answerkey == '2') {
        heat = 1;
        EEPROM_writeDouble(heatAddress, 1);
        answer = 1;
      }
      timdiff = millis() - queststart;
    }
    lcd.clear();
    lcd.print(F("pH="));
    lcd.setCursor(0, 1) ;          //Display position
    lcd.print(F("T="));            //display"Temp="
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
    LogToSD();
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
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    if (now.minute() < 10) {
     Serial.print("0");
    }
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    if (now.second() < 10) {
      Serial.print("0");
    }
    Serial.print(now.second(), DEC);
    Serial.println();
    Serial.println();
  }

  //Sending data to Google Sheets////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (EthConnect) {

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      Serial.println(F("Starting upload to Google Docs"));
      wdt_reset();
      
      lcd.setCursor(9, 0);
      lcd.write((uint8_t)0);
      pinMode(4, OUTPUT);
      digitalWrite(4, HIGH);

      previousMillis = currentMillis;                     // save the last time you updated Google Sheets

      packData();                                          //packing GET query with data

      Serial.println(F("connecting..."));
      if (client.connect(server, 80)) {
        sendData();
        cxn = true;                                        //connected = true
      }
      else {
        Serial.println(F("connection failed"));
      }
      // loop
      while (cxn) {
        if (client.available()) {
          char c = client.read(); //save http header to c
          Serial.print(c); //print http header to serial monitor
        }
        if (!client.connected()) {
          Serial.println();
          Serial.println(F("disconnecting."));
          Serial.print(F("Temperature Sent :"));
          Serial.println(temp); //print sent value to serial monitor

          client.stop();
          cxn = false;

          data = ""; //data reset

        }
      }
      Serial.println(F("exiting Google Docs loop"));
      lcd.setCursor(9, 0);
      lcd.write(" ");


    }
  }


  //Renewing DHCP lease every so often///////////////////////////////////////////////////////////////////////////////////////
  unsigned long currentMillis = millis();

  if (currentMillis - previousLease >= LeaseInterval) {
    Ethernet.maintain();
  }

  //Serial.println("");
  pH = -99;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////END MAIN LOOP//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ************************************************
// Packing data into Pushingbox request
// ************************************************
void packData() {
  data += "";
  data += "GET /pushingbox?devid="; //GET request query to pushingbox API
  data += DevID; //GET request query to pushingbox API
  data += "&tankid="; //GET request query to pushingbox API
  data += tankid;
  data += "&tempData="; //GET request query to pushingbox API
  data += temp;
  data += "&pHdata="; //GET request query to pushingbox API
  data += String(pHDisplay, 3);
  data += " HTTP/1.1";
}


// ************************************************
// Sending Pushingbox request
// ************************************************
void sendData() {
  Serial.println(F("connected"));
  Serial.println(data);
  client.println(data);
  client.println("Host: api.pushingbox.com");
  client.println("Connection: close");
  client.println();
}

//Adding for Time//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void digitalClockDisplay() {
  // digital clock display of the time
  DateTime now = rtc.now();
  Serial.print(now.hour());
  printDigits(now.minute());
  printDigits(now.second());
  Serial.print(F(" "));
  Serial.print(now.day());
  Serial.print(F("-"));
  Serial.print(now.month());
  Serial.print(F("-"));
  Serial.print(now.year());
  Serial.println();
}

void printDigits(int digits) {
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(F(":"));
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


// ************************************************
// Save any parameter changes to EEPROM
// ************************************************
void SavepHSet()
{
  if (phset != EEPROM_readDouble(pHAddress))
  {
    EEPROM_writeDouble(pHAddress, phset);
  }
}

void SaveTempSet()
{
  if (tempset != EEPROM_readDouble(tempAddress))
  {
    EEPROM_writeDouble(tempAddress, tempset);
  }
}

// ************************************************
// Load parameters from EEPROM
// ************************************************
void LoadParameters()
{
  // Load from EEPROM
  phset = EEPROM_readDouble(pHAddress);
  tempset = EEPROM_readDouble(tempAddress);
  Kp = EEPROM_readDouble(KpAddress);
  Ki = EEPROM_readDouble(KiAddress);
  Kd = EEPROM_readDouble(KdAddress);
  heat = EEPROM_readDouble(heatAddress);
  // Use defaults if EEPROM values are invalid
  if (isnan(phset))
  {
    phset = 8.1;
  }
  if (isnan(tempset))
  {
    tempset = 20;
  }
  if (isnan(Kp))
  {
    Kp = 100000;
  }
  if (isnan(Ki))
  {
    Ki = 0;
  }
  if (isnan(Kd))
  {
    Kd = 0;
  }
  if (isnan(heat))
  {
    heat = 0;
  }
}


// ************************************************
// Write floating point values to EEPROM
// ************************************************
void EEPROM_writeDouble(int address, double value)
{
  byte* p = (byte*)(void*)&value;
  for (int i = 0; i < sizeof(value); i++)
  {
    EEPROM.write(address++, *p++);
  }
}

// ************************************************
// Read floating point values from EEPROM
// ************************************************
double EEPROM_readDouble(int address)
{
  double value = 0.0;
  byte* p = (byte*)(void*)&value;
  for (int i = 0; i < sizeof(value); i++)
  {
    *p++ = EEPROM.read(address++);
  }
  return value;
}

// ************************************************
// Manual Time Set
// ************************************************
void ManualTime()
{
  lcd.clear();
  lcd.print(F("What year is it?"));
  unsigned long Yearnow;

  Key = customKeypad.waitForKey();
  Yearnow = (Key - '0') * 1000;
  lcd.setCursor(0, 1);
  lcd.print(Key);
  Serial.print(F("Thousands place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  Yearnow = ((Key - '0') * 100) + Yearnow;
  lcd.setCursor(1, 1);
  lcd.print(Key);
  Serial.print(F("Hundreds place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  Yearnow = ((Key - '0') * 10) + Yearnow;
  lcd.setCursor(2, 1);
  lcd.print(Key);
  Serial.print(F("Tens place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  Yearnow = (Key - '0') + Yearnow;
  lcd.setCursor(3, 1);
  lcd.print(Key);
  Serial.print(F("Ones place: "));
  Serial.println(Key);
  delay(500);

  lcd.clear();
  lcd.print(F("Month? (01-12)"));
  unsigned long Monthnow;

  Key = customKeypad.waitForKey();
  Monthnow = (Key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(Key);
  Serial.print(F("Ten place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  Monthnow = (Key - '0') + Monthnow;
  lcd.setCursor(1, 1);
  lcd.print(Key);
  Serial.print(F("Ones place: "));
  Serial.println(Key);
  delay(500);

  lcd.clear();
  lcd.print(F("Day? (01-31)"));
  unsigned long Daynow;

  Key = customKeypad.waitForKey();
  Daynow = (Key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(Key);
  Serial.print(F("Ten place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  Daynow = (Key - '0') + Daynow;
  lcd.setCursor(1, 1);
  lcd.print(Key);
  Serial.print(F("Ones place: "));
  Serial.println(Key);
  delay(500);

  lcd.clear();
  lcd.print(F("Hour? (00-23)"));
  unsigned long Hournow;

  Key = customKeypad.waitForKey();
  Hournow = (Key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(Key);
  Serial.print(F("Ten place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  Hournow = (Key - '0') + Hournow;
  lcd.setCursor(1, 1);
  lcd.print(Key);
  Serial.print(F("Ones place: "));
  Serial.println(Key);
  delay(500);

  lcd.clear();
  lcd.print(F("Minute? (00-59)"));
  unsigned long Minnow;

  Key = customKeypad.waitForKey();
  Minnow = (Key - '0') * 10;
  lcd.setCursor(0, 1);
  lcd.print(Key);
  Serial.print(F("Ten place: "));
  Serial.println(Key);

  Key = customKeypad.waitForKey();
  Minnow = (Key - '0') + Minnow;
  lcd.setCursor(1, 1);
  lcd.print(Key);
  Serial.print(F("Ones place: "));
  Serial.println(Key);
  delay(500);

  setTime(Hournow, Minnow, 30, Daynow, Monthnow, Yearnow);
  rtc.adjust(DateTime(Yearnow, Monthnow, Daynow, Hournow, Minnow, 30));


  int starttime = millis();
  int nowtime = millis();
  while (nowtime <= starttime + 5000)
  {
    nowtime = millis();
    lcd.clear();
    lcd.print(String(month()) + "/" + String(day()) + "/" + String(year()));
    lcd.setCursor(0, 1);
    lcd.print(String(hour()) + ":" + String(minute()) + ":" + String(second()));
    delay(950);
  }
  lcd.clear();
}


// ************************************************
// Called by ISR every 15ms to drive the output
// ************************************************
void DriveOutput()
{
  long now = millis();
  // Set the output
  // "on time" is proportional to the PID output
  if (now - windowStartTime > WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  if ((onTime > 100) && (onTime > (now - windowStartTime)))
  {
    digitalWrite(co2reg, LOW); //OPEN CO2 solenoid
  }
  else
  {
    digitalWrite(co2reg, HIGH); //CLOSE CO2 solenoid
  }
}


// ************************************************
// Get pH reading from Atlas pH EZO
// ************************************************
void Get_pH()
{
  while (pH == -99) {
    if (input_string_complete) {                        //if a string from the PC has been received in its entirety
      Serial1.print(inputstring);                      //send that string to the Atlas Scientific product
      Serial1.print('\r');                             //add a <CR> to the end of the string
      inputstring = "";                                 //clear the string
      input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the PC
    }

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
        Serial.print(F("pH = "));
        Serial.println(pH, 3);
      }
      sensorstring = "";                                //clear the string
      sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
    }
  }
  Input = -1 * pH;
  pHDisplay = pH;
}

// ************************************************
// Get temperature reading from PT100
// ************************************************
void Get_Temperature()
{
  uint16_t rtd = max.readRTD();
  float ratio = rtd;
  ratio /= 32768;
  tempnow = max.temperature(100, RREF) + tempcorr;

  total = total - readings[readIndex];                      // Delete oldest temperature reading
  readings[readIndex] = tempnow;                            // Add new temperature reading to the array
  total = total + readings[readIndex];                      // Add the temperature reading to the total
  readIndex = readIndex + 1;                                // advance to the next position in the array

  if (readIndex >= numReadings) {                           // if we're at the end of the array...
    readIndex = 0;                                          // ...wrap around to the beginning
  }

  temp = total / numReadings;                               // calculate the average
  Serial.print(F("Temperature = ")); Serial.println(temp);
  Serial.print(F("Resistance = ")); Serial.println(RREF*ratio,5);
}

// ************************************************
// Send temperature compensation to Atlas pH EZO
// ************************************************
void Set_Temp_Comp()
{
  if (temp>0 && temp<100) {
    tempcomp = pretempcomp + String(temp, 2);
    }
  else {
    tempcomp = pretempcomp + 20;
    }    
  Serial.println(tempcomp);
  Serial1.print(tempcomp);                      //send that string to the Atlas Scientific product
  Serial1.print('\r');                             //add a <CR> to the end of the string
}

// ************************************************
// Run the Auto-Tuning cycle
// ************************************************
void RunAutoTune()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("   AUTOTUNING   "));
  lcd.setCursor(0, 1);
  lcd.print(F("pH=      T="));
  tuning = true;
  // Remember the mode we were in
  ATuneModeRemember = myPID.GetMode();

  // set up the auto-tune parameters
  aTune.SetNoiseBand(aTuneNoise);
  aTune.SetOutputStep(aTuneStep);
  aTune.SetLookbackSec((int)aTuneLookBack);
  while (tuning) {
    if (tuning)
    {
      byte val = (aTune.Runtime());
      Get_pH();
      lcd.setCursor(3, 1);
      lcd.print(pH, 3);
      Get_Temperature();
      lcd.setCursor(11, 1);
      lcd.print(temp, 2);
      Set_Temp_Comp();
      pH = -99;
      if (val != 0)
      {
        tuning = false;
      }
      if (!tuning)
      { //we're done, set the tuning parameters
        Kp = aTune.GetKp();
        Ki = aTune.GetKi();
        Kd = aTune.GetKd();

        // Re-tune the PID and revert to normal control mode
        myPID.SetTunings(Kp, Ki, Kd);
        myPID.SetMode(ATuneModeRemember);

        // Persist any changed parameters to EEPROM
        EEPROM_writeDouble(KpAddress, Kp);
        EEPROM_writeDouble(KiAddress, Ki);
        EEPROM_writeDouble(KdAddress, Kd);
      }
    }
    char atune_key = customKeypad.getKey();
    if (atune_key == "#") {
      aTune.Cancel();
      tuning = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("   CANCELLING   "));
      lcd.setCursor(0, 1);
      lcd.print(F("   AUTOTUNING   "));
      delay(3000);
    }

  }

}

// ************************************************
// Set chiller state
// ************************************************

void Set_Chiller()
{
  if (heat == 0) {
    unsigned long chiller_currentMillis = millis();
    if (chiller_currentMillis - chiller_previousMillis >= chiller_interval) { //pause 30 seconds between swtiching chiller on and off to prevent damage to chiller
      chiller_previousMillis = chiller_currentMillis;
      if (temp >= tempset + 0.05) {                                          //if the observed temperature is greater than or equal the temperature setpoint plus .05 degree
        Serial.println(F("chiller on"));                                     //print chiller state to serial
        digitalWrite(chiller, LOW);
        }
      if (temp <= tempset - 0.05) {                                          //see if temperature is lower than .05 below setpoint
        Serial.println(F("chiller off"));                                    //print chiller state to serial
        digitalWrite(chiller, HIGH);
      }
    }
    
  }
  if (heat == 1) {
    if (temp <= tempset + 0.05) {                                          //if the observed temperature is less than or equal the temperature setpoint plus .05 degree
        Serial.println(F("chiller on"));                                     //print chiller state to serial
        digitalWrite(chiller, LOW);
        }
      if (temp >= tempset - 0.05) {                                          //see if temperature is greater than or equal to .05 below setpoint
        Serial.println(F("chiller off"));                                    //print chiller state to serial
        digitalWrite(chiller, HIGH);
      }
    }
}

// ************************************************
// Change Kp value
// ************************************************

void Change_Kp()
{
  double KpTemp;
  KpTemp = Kp;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Kp:"));
  lcd.setCursor(0, 1);
  lcd.print(KpTemp);
  Key = NO_KEY;
  while (Key != '#') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      KpTemp = KpTemp + 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }
    if (Key == '4') {
      KpTemp = KpTemp - 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '2') {
      KpTemp = KpTemp + 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '5') {
      KpTemp = KpTemp - 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '3') {
      KpTemp = KpTemp + 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '6') {
      KpTemp = KpTemp - 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == 'A') {
      KpTemp = KpTemp + 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == 'B') {
      KpTemp = KpTemp - 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '7') {
      KpTemp = KpTemp + 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '*') {
      KpTemp = KpTemp - 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '8') {
      KpTemp = KpTemp + 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '0') {
      KpTemp = KpTemp - 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Kp:"));
  lcd.print(KpTemp);
  lcd.setCursor(0, 1);
  lcd.print(F("Keep:1 Discard:2"));

  while (Key != '2') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      Kp = KpTemp;
      EEPROM_writeDouble(KpAddress, Kp);
      myPID.SetTunings(Kp, Ki, Kd);
      Key = '2';
    }
  }
}

// ************************************************
// Change Ki value
// ************************************************

void Change_Ki()
{
  Serial.println(F("starting Ki adjustment routine"));
  double KiTemp;
  KiTemp = Ki;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Ki:"));
  lcd.setCursor(0, 1);
  lcd.print(KiTemp);
  Key = NO_KEY;
  Serial.print(F("Key: "));
  Serial.println(Key);
  while (Key != '#') {
    Serial.println(F("In the while loop to wait for key presses"));
    Key = customKeypad.waitForKey();
    Serial.print(F("Key: "));
    Serial.println(Key);
    if (Key == '1') {
      Serial.println(F("in the 1 if statement"));
      KiTemp = KiTemp + 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }
    if (Key == '4') {
      Serial.println(F("in the 4 if statement"));
      KiTemp = KiTemp - 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '2') {
      KiTemp = KiTemp + 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '5') {
      KiTemp = KiTemp - 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '3') {
      KiTemp = KiTemp + 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '6') {
      KiTemp = KiTemp - 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == 'A') {
      KiTemp = KiTemp + 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == 'B') {
      KiTemp = KiTemp - 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '7') {
      KiTemp = KiTemp + 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '*') {
      KiTemp = KiTemp - 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '8') {
      KiTemp = KiTemp + 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '0') {
      KiTemp = KiTemp - 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }
  }
  Serial.println(F("out of the Ki adjusting while loop"));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Ki:"));
  lcd.print(KiTemp);
  lcd.setCursor(0, 1);
  lcd.print(F("Keep:1 Discard:2"));

  while (Key != '2') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      Ki = KiTemp;
      EEPROM_writeDouble(KiAddress, Ki);
      myPID.SetTunings(Kp, Ki, Kd);
      Key = '2';
    }
  }
}

// ************************************************
// Change Kd value
// ************************************************

void Change_Kd()
{
  double KdTemp;
  KdTemp = Kd;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Kd:"));
  lcd.setCursor(0, 1);
  lcd.print(KdTemp);
  Key = NO_KEY;
  while (Key != '#') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      KdTemp = KdTemp + 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }
    if (Key == '4') {
      KdTemp = KdTemp - 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '2') {
      KdTemp = KdTemp + 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '5') {
      KdTemp = KdTemp - 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '3') {
      KdTemp = KdTemp + 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '6') {
      KdTemp = KdTemp - 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == 'A') {
      KdTemp = KdTemp + 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == 'B') {
      KdTemp = KdTemp - 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '7') {
      KdTemp = KdTemp + 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '*') {
      KdTemp = KdTemp - 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '8') {
      KdTemp = KdTemp + 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '0') {
      KdTemp = KdTemp - 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Kd:"));
  lcd.print(KdTemp);
  lcd.setCursor(0, 1);
  lcd.print(F("Keep:1 Discard:2"));

  while (Key != '2') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      Kd = KdTemp;
      EEPROM_writeDouble(KdAddress, Kd);
      myPID.SetTunings(Kp, Ki, Kd);
      Key = '2';
    }
  }
}

// ************************************************
// Log parameters to SD card
// ************************************************

void LogToSD() {
  unsigned long SD_currentMillis = millis();
  DateTime now = rtc.now();
  Serial.println("SD1");
  pinMode(10, OUTPUT);
  Serial.println("SD1");
  digitalWrite(10, HIGH);
  if (SD_currentMillis - SD_previousMillis >= SD_interval) {
    SD_previousMillis = SD_currentMillis;
    filename = String(now.year() - 2000) + "-" + now.month() + "-" + now.day();
    file_full = filename + ".txt";

    myFile = SD.open(file_full, FILE_WRITE);
    myFile.println("time,temp,temp setpoint,pH,pH setpoint");
    myFile.close();
  }

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  myFile = SD.open(file_full, FILE_WRITE);
  SDstring = "";
  SDstring += String(now.month(), DEC);
  SDstring += "/";
  SDstring += String(now.day(), DEC);
  SDstring += "/";
  SDstring += String(now.year(), DEC);
  SDstring += " ";
  SDstring += String(now.hour(), DEC);
  SDstring += ":";
  if (now.minute() < 10) {
    SDstring += "0";
  }
  SDstring += String(now.minute(), DEC);
  SDstring += ":";
  if (now.second() < 10) {
    SDstring += "0";
  }
  SDstring += String(now.second(), DEC);
  SDstring += ",";
  SDstring += tankid;
  SDstring += ",";
  SDstring += String(temp, 2);
  SDstring += ",";
  SDstring += String(tempset, 2);
  SDstring += ",";
  SDstring += String(pH, 3);
  SDstring += ",";
  SDstring += String(phset, 3);
  SDstring += ",";
  SDstring += String(onTime);
  SDstring += ",";
  SDstring += Kp;
  SDstring += ",";
  SDstring += Ki;
  SDstring += ",";
  SDstring += Kd;
  myFile.println(SDstring);
  myFile.close();
  Serial.println(SDstring);
}


void LCDupdate() {
  lcd.setCursor(3, 0);
  lcd.print(pHDisplay, 3);
  lcd.setCursor(11, 0);
  lcd.print(phset, 3);
  lcd.setCursor(0, 1) ;
  lcd.setCursor(2, 1);
  lcd.print(temp, 2);
  lcd.setCursor(11, 1);
  lcd.print(tempset, 2);
  lcd.setCursor(9,1);
  if (heat == 0) {
    lcd.print("C");
    }
  if (heat == 1) {
    lcd.print("H");
    }
}
