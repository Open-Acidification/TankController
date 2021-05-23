#include "PushingBox.h"

#include "Devices/Ethernet_TC.h"
#include "Devices/EEPROM_TC.h"
#include "Devices/Serial_TC.h"

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
  // is client ready?
  if (!client) {
    // will this output every 15 ms?
    serial("Ethernet client not ready");
    return;
  }
  // is it time to send ?
  unsigned long now = millis();
  if (now >= nextSendTime) {
    sendDataHTTPRequest();
    unsigned long interval = EEPROM_TC::instance()->getGoogleSheetInterval();
    // jump to the next multiple of interval
    nextSendTime = (now / interval + 1) * interval;
  }
  // are we still connected? 
  if (client.connected()) {
    // if so , read response
    while (client.available()) {
      char c = client.read(); //save http header to c
      Serial.print(c); //print http header to serial monitor
    }
  } else {
    // will this close every 15 ms?
    client.close();
  }
}

void PushingBox::sendDataHTTPRequest() {
  serial("attepting to connect to pushing box...");
  if(!client.connected() && !client.connect(server, 80)) {
    serial(F("connection failed"));
    return;
  }
  serial(F("connected"));
  char format[] = 
    "GET /pushingbox?devid=%s&tankid=%i&tempData=%.2f&pHdata=%.3f HTTP/1.1\r\n" 
    "Host: api.pushingbox.com\r\n" 
    "Connection: close\r\n" 
    "\r\n";
  char buffer[200];
  // look up tankid, temperature, ph
  int tankId = EEPROM_TC::instance()->getTankID();
  snprintf(buffer, sizeof(buffer), format, DevID, tankId, temperature, pH);
  client.write(buffer, strnlen(buffer, sizeof(buffer)));
}
