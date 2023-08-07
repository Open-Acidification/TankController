#pragma once
#include <Arduino.h>

#include "wrappers/Ethernet_TC.h"

class PushingBox {
public:
  // class methods
  static PushingBox *instance(const char *pushingBoxID = nullptr);

  // instance methods
  PushingBox(const char *pushingBoxID);
  EthernetClient *getClient() {
    return &client;
  }
  const char *getServerDomain() {
    return serverDomain;
  }
  void loop();
  void setDeviceID(const char *id) {
    deviceID = id;
  }

#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  void resetNextSendTime() {
    nextSendTime = 70000;
  }
#endif
private:
  // class variables
  static PushingBox *_instance;

  // instance variables
  EthernetClient client;
  const char *deviceID = nullptr;  // DeviceID assigned by PushingBox and passed-in from TankController.ino
  // wait a bit for first reading (https://github.com/Open-Acidification/TankController/issues/179)
  uint32_t nextSendTime = 70000;
  const char *serverDomain = "api.pushingbox.com";

  // instance method
  void sendData();
};
