#pragma once
#include <Arduino.h>

#include "Ethernet_TC.h"

class PushingBox {
public:
  // class methods
  static PushingBox *instance();

  // instance methods
  EthernetClient *getClient() {
    return &client;
  }
  byte *getServer() {
    return server;
  }
  void loop();

private:
  // class variables
  static PushingBox *_instance;

  // instance variables
  EthernetClient client;
  const char *DevID = "v172D35C152EDA6C";  // DeviceID from Pushingbox
  unsigned long nextSendTime = 0;
  const char *server = "api.pushingbox.com";

  // instance method
  void sendData();
};
