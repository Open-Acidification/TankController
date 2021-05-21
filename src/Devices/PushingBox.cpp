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
    Ethernet_TC::instance();
  }
  return _instance;
}

void PushingBox::sendData(int tankId, float temperature, float pH) {
  char format[] = "GET /pushingbox?devid=%s&tankid=%i&tempData=%.2f&pHdata=%.3f HTTP/1.1";
  char buffer[128];
  snprintf(buffer, sizeof(buffer), format, DevID, tankId, temperature, pH);
  if(client.connect(server, 80)) {
    client.write(buffer, strnlen(buffer, sizeof(buffer)));
  }
}
