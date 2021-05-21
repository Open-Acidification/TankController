#include "PushingBox.h"

#include "Devices/Ethernet_TC.h"
#include "Serial_TC.h"

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

void PushingBox::sendData(int tankId, float temperature, float pH, EthernetClient &client) {
  data = "GET /pushingbox?devid=" + DevID;
  data += "&tankid=" + String(tankId);
  data += "&tempData=" + String(temperature, 2);
  data += "&pHdata=" + String(pH, 3);
  data += " HTTP/1.1";
  Ethernet_TC::instance()->sendPushingBoxData(data, server, client);
}
