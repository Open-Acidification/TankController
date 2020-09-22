
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
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <MemoryFree.h>
#include <PID_AutoTune_v0.h>
#include <PID_v1.h>
#include <SD.h>
#include <SPI.h>
#include <TimeLib.h>
#include <TrueRandom.h>
#include <Wire.h>
#include <avr/wdt.h>
#include <stdio.h>

#include "RTClib.h"
#include "TankControllerLib.h"

TankController tank;

const String DEV_ID = "v172D35C152EDA6C";  // DeviceID from Pushingbox

const float RREF = 430.0;
Adafruit_MAX31865 max = Adafruit_MAX31865(45, 43, 41, 39);
RTC_PCF8523 rtc;

const String SOFTWARE_VERSION = "0.2.0";  // Software Version

const char API_SERVER[] = "api.pushingbox.com";  // pushingbox API server
IPAddress ip(192, 168, 1, 2);                    // Arduino IP address. Only used when DHCP is turned off.
EthernetClient client;                           // define 'client' as object
EthernetServer ethernet_server(80);
int request_char_counter = 0;  // for parsing html requests
String data;                   // GET query with data

// Time Constants /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const unsigned long SECOND_IN_MILLIS = 1000;
const unsigned long MINUTE_IN_MILLIS = 60 * SECOND_IN_MILLIS;
const unsigned long HOUR_IN_MILLIS = 60 * MINUTE_IN_MILLIS;
const unsigned long DAY_IN_MILLIS = 24 * HOUR_IN_MILLIS;

const unsigned long ONE_SECOND_DELAY_IN_MILLIS = 1 * SECOND_IN_MILLIS;
const unsigned long THREE_SECOND_DELAY_IN_MILLIS = 3 * SECOND_IN_MILLIS;
const unsigned long FIVE_SECOND_DELAY_IN_MILLIS = 5 * SECOND_IN_MILLIS;
const unsigned long SEVEN_SECOND_DELAY_IN_MILLIS = 7 * SECOND_IN_MILLIS;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long interval = 20 * MINUTE_IN_MILLIS;  // interval at which to update Google Sheets (milliseconds)
unsigned long previous_millis =
    0 - interval +
    30 * SECOND_IN_MILLIS;  // will store last time Google Sheets was updated (-interval+30000 sets first upload for 30 seconds after startup. This eases troubleshooting)
unsigned long chiller_previous_millis = 0;  // will store last time chiller state was checked
const unsigned long CHILLER_INTERVAL =
    30 * SECOND_IN_MILLIS;                 // interval at which to change chiller state                 // interval at which to start a new log file (formerly 1 day)
unsigned long sensor_previous_millis = 0;  // will store last time sensor readings were taken
unsigned long sensor_interval;             // interval at which to start a new log file (milliseconds)
unsigned long second_previous_millis = 0;
const unsigned long second_interval = 1 * SECOND_IN_MILLIS;
unsigned long granularity;
unsigned long max_data_age;
const unsigned long LEASE_INTERVAL = 4 * DAY_IN_MILLIS;  // Interval at which to renew DHCP lease
unsigned long previous_lease = 0;

const byte ROWS = 4;        // four rows
const byte COLS = 4;        // four columns
String input_string = "";   // a string to hold incoming data from the PC
String sensor_string = "";  // a string to hold the data from the EZO pH circuit
const String PRE_MID_CAL_STRING = "Cal,mid,";
const String PRE_LOW_CAL_STRING = "Cal,low,";
String mid_cal_string = "";
String slope = "";
boolean input_string_complete = false;   // have we received all the data from the PC
boolean sensor_string_complete = false;  // have we received all the data from the Atlas Scientific product
double pH;                               // used to hold a floating point number that is the pH
double ph_display;                       // used to hold a floating point number that is the pH
double temp;
double temp_set;
double ph_set;
double amplitude_set;
double frequency_set;
double temp_corr = 0;
double heat;
double input, output, set_point;
double Kp;
double Ki;
double Kd;
double amplitude;
double frequency;
long on_time = 0;
const int RECORD_LENGTH = 70;   // length of each record to be logged
const int WINDOW_SIZE = 10000;  // 10 second Time Proportional output window
long window_start_time = millis();
float mid_buffer = 0;
char key = NO_KEY;
char exit_ph = NO_KEY;
int tank_id;
boolean eth_connect = true;
byte mac[6] = {0x90, 0xA2, 0xDA, 0x00, 0x00, 0x00};
char mac_str[18];
boolean pid_run = true;
const int CHILLER = 47;
const int CO2_REG = 49;
const int GOAL_RECORD_LENGTH = 10;
long ph_interval;
long ph_delay;
int ph_series_size;
long ph_series_pointer;
long temp_interval;
long temp_delay;
int temp_series_size;
long temp_series_pointer;

const int HTML_REQUEST_BUFFER_SIZE = 500;
char html_request_buffer[HTML_REQUEST_BUFFER_SIZE];

// Temperature Smoothing/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int NUM_READINGS = 10;

double readings[NUM_READINGS];  // the readings from the analog input
volatile int read_index = 0;    // the index of the current reading
volatile double total = 0;      // the running total
volatile double temp_now = 0;   // the average
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// EEPROM addresses for persisted data///////////////////////////////////////////////////////////////////////////////////////////////////////////
const int PH_ADDRESS = 0;
const int TEMP_ADDRESS = 4;
const int TANKID_ADDRESS = 8;
const int TEMP_CORR_ADDRESS = 12;
const int KP_ADDRESS = 20;
const int KI_ADDRESS = 28;
const int KD_ADDRESS = 36;
const int MAC_ADDRESS = 44;
const int HEAT_ADDRESS = 52;
const int AMPLITUDE_ADDRESS = 56;
const int FREQUENCY_ADDRESS = 60;
const int GRANULARITY_ADDRESS = 64;   // granularity for SD logging interval
const int MAX_DATA_AGE_ADDRESS = 68;  // max data age for SD card
const int PH_SERIES_SIZE_ADDRESS = 72;
const int PH_SERIES_POINTER_ADDRESS = 76;
const int TEMP_SERIES_SIZE_ADDRESS = 80;
const int TEMP_SERIES_POINTER_ADDRESS = 84;
const int PH_INTERVAL_ADDRESS = 88;
const int PH_DELAY_ADDRESS = 92;
const int TEMP_INTERVAL_ADDRESS = 96;
const int TEMP_DELAY_ADDRESS = 100;

// End EEPROM addresses for persisted data///////////////////////////////////////////////////////////////////////////////////////////////////////

String file_name;

const char STD_KEYS[ROWS][COLS] = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};
const byte ROW_PINS[ROWS] = {34, 36, 38, 40};
const byte COL_PINS[COLS] = {42, 44, 46, 48};
Keypad custom_keypad = Keypad(makeKeymap(STD_KEYS), ROW_PINS, COL_PINS, ROWS, COLS);
PID my_pid(&input, &output, &set_point, Kp, Ki, Kd, DIRECT);  // Starting the PID, Specify the links and initial tuning parameters
File my_file;
const int RS = 24, EN = 22, D4 = 26, D5 = 28, D6 = 30, D7 = 32;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

// ************************************************
// Auto Tune Variables and constants
// ************************************************
byte a_tune_mode_remember = 2;

const double ATUNE_STEP = 500;
const double ATUNE_NOISE = 1;
const unsigned int ATUNE_LOOK_BACK = 20;

boolean tuning = false;

PID_ATune aTune(&input, &output);

// Stuff for retreiving time/////////////////////////////////////////////////////////////////////////////////////////////////////////
IPAddress time_server(132, 163, 97, 1);  // utcnist.colorado.edu
const int TIME_ZONE = -7;                // Pacific Daylight Time (USA)
EthernetUDP udp;
unsigned int local_port = 8888;  // local port to listen for UDP packets
// End Time////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Custom Characters for splash////////////////////////////////
const byte OL1[8] = {0b00000, 0b00000, 0b00001, 0b00011, 0b00011, 0b00111, 0b00111, 0b00110};

const byte OL2[8] = {0b00111, 0b11111, 0b11111, 0b10011, 0b11101, 0b11110, 0b00101, 0b11011};

const byte OL3[8] = {0b11100, 0b11111, 0b01111, 0b10111, 0b10110, 0b01110, 0b10101, 0b11011};

const byte OL4[8] = {0b00000, 0b00000, 0b10000, 0b11000, 0b11000, 0b11100, 0b11100, 0b11100};

const byte OL5[8] = {0b00111, 0b00111, 0b00111, 0b00011, 0b00011, 0b00001, 0b00000, 0b00000};

const byte OL6[8] = {0b11011, 0b10101, 0b01110, 0b01101, 0b11101, 0b11110, 0b11111, 0b00111};

const byte OL7[8] = {0b11011, 0b10100, 0b01111, 0b10111, 0b11001, 0b11111, 0b11111, 0b11100};

const byte OL8[8] = {0b01100, 0b11100, 0b11100, 0b11000, 0b11000, 0b10000, 0b00000, 0b00000};

void serialEvent() {                          // if the hardware serial port_0 receives a char
  input_string = Serial.readStringUntil(13);  // read the string until we see a <CR>
  input_string_complete = true;               // set the flag used to tell if we have received a completed string from the PC
}

void serialEvent3() {                           // if the hardware serial port_3 receives a char
  sensor_string = Serial1.readStringUntil(13);  // read the string until we see a <CR>
  sensor_string_complete = true;                // set the flag used to tell if we have received a completed string from the PC
}

// ************************************************
// Timer Interrupt Handlers
// ************************************************
SIGNAL(TIMER2_OVF_vect) {
  // DriveOutput();
}