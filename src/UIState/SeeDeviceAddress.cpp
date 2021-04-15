/**
 * SeeDeviceAddress.cpp
 */

#include "SeeDeviceAddress.h"

#include "../Devices/Ethernet_TC.h"
#include "../Devices/LiquidCrystal_TC.h"
#include "Ethernet.h"

void SeeDeviceAddress::loop() {
  IPAddress IP = Ethernet_TC::instance()->getIP();
  byte* mac = Ethernet_TC::instance()->getMac();
  char line0[170];
  char line1[17];
  sprintf(line0, "%03d.%03d.%03d.%03d", IP[0], IP[1], IP[2], IP[3]);
  sprintf(line1, "%02X%02X:%02X%02X:%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  LiquidCrystal_TC::instance()->writeLine(line0, 0);
  LiquidCrystal_TC::instance()->writeLine(line1, 1);
}
