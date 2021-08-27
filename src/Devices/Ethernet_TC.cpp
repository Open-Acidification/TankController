#include "Ethernet_TC.h"

#include <avr/wdt.h>

#include "Devices/EEPROM_TC.h"
#include "Devices/Keypad_TC.h"
#include "Serial_TC.h"

Ethernet_TC *Ethernet_TC::_instance = nullptr;

// Establishes the Ethernet connection and sets class variables
Ethernet_TC::Ethernet_TC() {
  pinMode(SD_SELECT_PIN, OUTPUT);
  digitalWrite(SD_SELECT_PIN, HIGH);
  readMac();
  serial(F("Attempting to connect to Ethernet"));
  wdt_disable();
  int key = Keypad_TC::instance()->getKey();
  long timeout = key == NO_KEY ? 60000 : 1;
  if (Ethernet.begin(mac, timeout)) {
    IP = Ethernet.localIP();
    serial(F("DHCP address is %i.%i.%i.%i"), IP[0], IP[1], IP[2], IP[3]);
    isUsingDHCP = true;
  } else {
    // update IP by adding tank ID to last octet
    defaultIP[3] += EEPROM_TC::instance()->getTankID();
    serial(F("DHCP failed, trying %i.%i.%i.%i"), defaultIP[0], defaultIP[1], defaultIP[2], defaultIP[3]);
    Ethernet.begin(mac, defaultIP);
    serial(F("Done with Ethernet setup"));
    IP = defaultIP;
  }
  wdt_enable(WDTO_8S);
}

Ethernet_TC *Ethernet_TC::instance(bool reset) {
  if (reset && _instance) {
    delete _instance;
    _instance = nullptr;
  }
  if (_instance == nullptr) {
    _instance = new Ethernet_TC;
  }
  return _instance;
}

void Ethernet_TC::loop() {
  if (isUsingDHCP) {
    // "just call it on every loop() invocation" https://www.arduino.cc/en/Reference/EthernetMaintain
    Ethernet.maintain();
  }
  numAttemptedDHCPReleases++;
}

void Ethernet_TC::readMac(bool forceReset) {
  uint8_t bytes[6];
  EEPROM_TC::instance()->getMac(bytes);
  if (forceReset || bytes[0] != '#') {
    randomSeed(analogRead(0) + millis());
    bytes[0] = '#';
    bytes[3] = random(256);
    bytes[4] = random(256);
    bytes[5] = random(256);
    EEPROM_TC::instance()->setMac(bytes);
  }
  mac[3] = bytes[3];
  mac[4] = bytes[4];
  mac[5] = bytes[5];
  serial(F("MAC address is %02x:%02x:%02x:%02x:%02x:%02x"), mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}
