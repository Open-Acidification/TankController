#pragma once
#include <Arduino.h>

#include "Ethernet_TC.h"

class PushingBox {
  public:
  // class methods
  static PushingBox* instance();

  // instance methods
  EthernetClient *getClient() {
    return &client;
  }
  byte *getServer() {
    return server;
  }
  void sendData(int tankId, float temperature, float pH);

  private:
  // class variables
  static PushingBox* _instance;

  //instance variables
  EthernetClient client;
  const char *DevID = "v172D35C152EDA6C"; //DeviceID from Pushingbox
  byte server[4] = { 64, 233, 187, 99 };
};