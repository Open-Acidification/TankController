
/*
   Version #: 0.197
   (0.190: Adding Real Time Clock,
    0.191: Temperature compensation defeat & PT100 resistance to serial monitor,
    0.192: added fields to SD card output
    0.193: Fixed current time display output to take time from RTC
    0.194: Device ID to the beginning, software version sig figs)
    0.195: Fixing SD card writing stuff (was taking too long and overrunning pH reading time)
    0.196: Adding ability to switch between chilling and heating
    0.197: Adding calibration reset and calibration stats
   Author: Kirt L Onthank
   Date:2019/9/28
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
#include <ArduinoJson.h>
#include <string.h>
#include <stdio.h>

String DevID = "v172D35C152EDA6C"; //DeviceID from Pushingbox

#define RREF 430.0
Adafruit_MAX31865 max = Adafruit_MAX31865(45, 43, 41, 39);
RTC_PCF8523 rtc;

double softvers = 0.197;                                        //Software Version

//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Setting MAC Address
char APIServer[] = "api.pushingbox.com"; //pushingbox API server
IPAddress ip(192, 168, 1, 2); //Arduino IP address. Only used when DHCP is turned off.
EthernetClient client; //define 'client' as object
EthernetServer ethernetServer(80);
int requestCharCounter = 0; // for parsing html requests
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
float sensor_interval;                      // interval at which to start a new log file (milliseconds)
int maxDataAge;
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
String sloperaw = "";
String slope = "";
boolean input_string_complete = false;             //have we received all the data from the PC
boolean sensor_string_complete = false;            //have we received all the data from the Atlas Scientific product
boolean WaitForString = true;
boolean SlopeFlag = true;
double pH;                                         //used to hold a floating point number that is the pH
double pHDisplay;                                  //used to hold a floating point number that is the pH
double temp;
double tempset;
double phset;
double amplitudeSet;
double frequencySet;
double newph = 0;
double newtemp = 0;
double newAmplitude = 0;
double newFrequency = 0;
double realtemp;
double tempcorr = 0;
double heat;
double Input, Output, Setpoint;
double Kp;
double Ki;
double Kd;
double amplitude;
double frequency;
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
int LoopStart;

char htmlRequestBuffer[500];

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
const int amplitudeAddress = 56;
const int frequencyAddress = 60;
const int granularityAddress = 64; // granularity for SD logging interval
const int maxDataAgeAddress = 68; // max data age for SD card

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