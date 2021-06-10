#include "PushingBox.h"

#include "Devices/EEPROM_TC.h"
#include "Devices/Ethernet_TC.h"
#include "Devices/PHProbe.h"
#include "Devices/Serial_TC.h"
#include "Devices/TempProbe_TC.h"
#include "TankControllerLib.h"
#include "TC_util.h"

//  class variables
PushingBox* PushingBox::_instance = nullptr;

//  class methods
/**
 * accessor for singleton
 */
PushingBox* PushingBox::instance() {
  if (!_instance) {
    _instance = new PushingBox();
  }
  return _instance;
}

void PushingBox::loop() {
  // is it time to send ?
  unsigned long now = millis();
  if (now >= nextSendTime) {
    sendData();
    unsigned long minutes = EEPROM_TC::instance()->getGoogleSheetInterval();
    if (minutes == 0xffff) {
      minutes = 20;
    }
    unsigned long interval = minutes * 60l * 1000l;
    // jump to the next multiple of interval
    nextSendTime = (now / interval + 1) * interval;
  }
  // are we still connected?
  if (client.connected()) {
    // if so, read response
    int next;
    while ((next = client.read()) != -1) {  // Flawfinder: ignore
      Serial.print((char)next);             // print response if any to serial
    }
  } else {
    // will this close every 15 ms?
    client.stop();
  }
}

void PushingBox::sendData() {
  int tankID = EEPROM_TC::instance()->getTankID();
  if (!tankID) {
    serial("Set Tank ID in order to send data to PushingBox");
    return;
  }
  char buffer[200];
  if (TankControllerLib::instance()->isInCalibration()) {
    char format[] =
        "GET /pushingbox?devid=%s&tankid=%i&tempData=C&pHdata=C HTTP/1.1\r\n"
        "Host: api.pushingbox.com\r\n"
        "Connection: close\r\n"
        "\r\n";
    snprintf(buffer, sizeof(buffer), format, DevID, tankID);
  } else {
    char format[] =
        "GET /pushingbox?devid=%s&tankid=%i&tempData=%.2f&pHdata=%.3f HTTP/1.1\r\n"
        "Host: api.pushingbox.com\r\n"
        "Connection: close\r\n"
        "\r\n";
    // look up tankid, temperature, ph
    float temperature = TempProbe_TC::instance()->getRunningAverage();
    float pH = PHProbe::instance()->getPh();
    snprintf(buffer, sizeof(buffer), format, DevID, tankID, temperature, pH);   
  }
  serial(buffer);
  serial("attempting to connect to PushingBox...");
  if (client.connected() || client.connect(server, 80)) {
    serial("connected");
    client.write(buffer, strnlen(buffer, sizeof(buffer)));
  } else {
    serial("connection failed");
  }
}
