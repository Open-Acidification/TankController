/**
 * Adafruit MAX31865
 * The Adafruit MAX31865 is an external device that connects to a thermocouple (temperature
 * sensor), and has pins that can be used to read a resistance, temperature, or fault. The
 * associated library has a simple API as described below.
 *
 * Device I/O:
 * This device has 5 pins: Vin (3V or 5V power supply, the same as the
 * microcontroler), GND (Ground), CLK (Clock), SDO (MOSI: Master Out Slave In),
 * SDI (MISO: Master In Slave Out), and CS (Slave Select).
 *
 * Connection with the thermocouple:
 * The Adafruit MAX31865 can connect to three different types of thermocouples,
 * two wire, three wire, or four wire. You'll need to look at the sensor's
 * documentation to find the wiring requirements. The board has four pins used
 * for connection with the thermocouple, they are labeled on the board: RTD+
 * (Resistance Positive), RTD- (Resistance Negative), F+ (Fault Plus), and F-
 * (Fault Negative).
 *
 * Using Adafruit MAX31865
 * There are 5 functions needed to use Adafruit MAX31865: the constructor,
 * begin(), readRTD() (Read Resistance), temperature(), readFault(), and
 * clearFault().
 *
 * Here is a description of those functions:
 *
 * Constructor:
 * To use the constructor you will have to set which pins you want to use, the
 * functions are labeled below. Adafruit_MAX31865 thermo = Adafruit_MAX31865(CS
 * (Slave Select), DI (MOSI), DO (MISO), CLK (Clock));
 *
 * Begin:
 * Call this after calling the constructor and before anything else. Returns
 * true if successful. Default is a 2 wire thermocouple but you can specify 2,3,
 * and 4 wire by passing in MAX31865_2WIRE, MAX31865_3WIRE, and MAX31865_4WIRE.
 * bool successful = thermo.begin(max31865_numwires_t x = MAX31865_2WIRE);
 *
 * Read Resistance:
 * Returns the resistance of the device.
 * uint16_t rtd = thermo.readRTD();
 *
 * Get Temperature:
 * To get the temperature you will need to know the Nominal Resistance, and the
 * Resistor Reference of your temperature sensor. This must be set according to
 * your thermocouple. Read your thermocouple
 *  manual to find these values.
 * float temp = thermo.temperature(float RTDnominal, float refResistor)
 *
 * Read Fault:
 * Returns a fault number in hex. You can compare with returned fault with the
 * default fault values given below uint8_t fault = thermo.readFault();
 *
 * Faults:
 * MAX31865_FAULT_HIGHTHRESH (RTD High Threshold)
 * MAX31865_FAULT_LOWTHRESH (RTD Low Threshold)
 * MAX31865_FAULT_REFINLOW (REFIN- > 0.85 x Bias)
 * MAX31865_FAULT_REFINHIGH (REFIN- < 0.85 x Bias - FORCE- open)
 * MAX31865_FAULT_RTDINLOW (RTDIN- < 0.85 x Bias - FORCE- open)
 * MAX31865_FAULT_OVUV (Under/Over Voltage)
 *
 * Clear Fault:
 * Once Fault is read clear fault so that it returns to its default state of 0
 * which is no fault. thermo.clearFault();
 *
 * How to use the Device:
 * To use the device you must call the constructor, call begin, and then you can
 * either call readRTD(), or temperature(). Once you have read either, or both
 * you can check for a fault, use the data accordingly, and then clear the fault
 * before reading again.
 *
 * How to use this TempProbe_TC class:
 * Call the constructor TempProbe_TC tempProbe = TempProbe_TC();
 * Since this class is build for Tank Controller class, it will automatically
 * set the device to a three wire thermocouple, set the pins that the tank
 * controller has delegated for it, and it has values for RTDnominal, and the
 * refResistor. This class is pretty straightforward and you can the functions
 * and how to call them below.
 */

#pragma once
#include "Arduino.h"

#ifdef MOCK_PINS_COUNT
#include <Adafruit_MAX31865_CI.h>
#else
#include <Adafruit_MAX31865.h>
#endif

class TempProbe_TC {
public:
  static TempProbe_TC* instance();

  uint16_t getResistance() {
    return thermo.readRTD();
  }

  float getTemperature() {
    return thermo.temperature(RTDnominal, refResistor);
  }

  uint8_t readFault() {
    return thermo.readFault();
  }

  void clearFault() {
    thermo.clearFault();
  }

private:
  //  Class variables
  static TempProbe_TC* _instance;
  const int RTDnominal = 100;
  const int refResistor = 430;
  Adafruit_MAX31865 thermo = Adafruit_MAX31865(45, 43, 41, 39);

  // Methods
  TempProbe_TC();
};
