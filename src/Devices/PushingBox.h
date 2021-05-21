#pragma once
#include <Arduino.h>

#include "Ethernet_TC.h"

class PushingBox {
  public:
  // class methods
  static PushingBox* instance();

  // instance methods
  void sendData(int tankId, float temperature, float pH, EthernetClient &client);

  private:
  // class variables
  static PushingBox* _instance;

  //instance variables
  String data = "";
  String DevID = "v172D35C152EDA6C"; //DeviceID from Pushingbox
  byte server[4] = { 64, 233, 187, 99 }; // Google
};