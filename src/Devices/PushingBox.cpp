#include "PushingBox.h"

#include "Devices/EEPROM_TC.h"
#include "Devices/Ethernet_TC.h"
#include "Devices/PHProbe.h"
#include "Devices/Serial_TC.h"
#include "Devices/TempProbe_TC.h"

//  class variables
PushingBox* PushingBox::_instance = nullptr;

//  class methods
/**
 * accessor for singleton
 */
PushingBox* PushingBox::instance() {
  if (!_instance) {
    _instance = new PushingBox();
    Ethernet_TC::instance();
  }
  return _instance;
}

void PushingBox::loop() {
  // is it time to send ?
  unsigned long now = millis();
  if (now >= nextSendTime) {
    sendData();
    unsigned long interval = EEPROM_TC::instance()->getGoogleSheetInterval();
    // jump to the next multiple of interval
    nextSendTime = (now / interval + 1) * interval;
  }
  // are we still connected?
  if (client.connected()) {
    // if so, read response
    int next;
    while ((next = client.read()) != -1) {
      Serial.print((char)next);  // print response if any to serial
    }
  } else {
    // will this close every 15 ms?
    client.stop();
  }
}

void PushingBox::sendData() {
  serial("attepting to connect to pushing box...");
  if (!client.connected() && !client.connect(server, 80)) {
    serial("connection failed");
    return;
  }
  serial("connected");
  char format[] =
      "GET /pushingbox?devid=%s&tankid=%i&tempData=%.2f&pHdata=%.3f HTTP/1.1\r\n"
      "Host: api.pushingbox.com\r\n"
      "Connection: close\r\n"
      "\r\n";
  char buffer[200];
  // look up tankid, temperature, ph
  int tankId = EEPROM_TC::instance()->getTankID();
  float temperature = TempProbe_TC::instance()->getRunningAverage();
  float pH = PHProbe::instance()->getPh();
  snprintf(buffer, sizeof(buffer), format, DevID, tankId, temperature, pH);
  client.write(buffer, strnlen(buffer, sizeof(buffer)));
}
