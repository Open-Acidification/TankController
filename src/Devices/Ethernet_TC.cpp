#include "Ethernet_TC.h"

#include <avr/wdt.h>

#include "Serial_TC.h"

Ethernet_TC *Ethernet_TC::_instance = nullptr;

// Establishes the Ethernet connection and sets class variables
Ethernet_TC::Ethernet_TC() {
  pinMode(IO_PIN, OUTPUT);
  digitalWrite(IO_PIN, HIGH);
  serial("Attempting to connect to Ethernet");
  wdt_disable();
  if (Ethernet.begin(mac, 5000)) {
    IP = Ethernet.localIP();
    serial("DHCP address is %i.%i.%i.%i", IP[0], IP[1], IP[2], IP[3]);
  } else {
    serial("DHCP failed, trying %i.%i.%i.%i", defaultIP[0], defaultIP[1], defaultIP[2], defaultIP[3]);
    Ethernet.begin(mac, defaultIP);
    serial("Done with Ethernet setup");
    IP = defaultIP;
  }
  wdt_enable(WDTO_8S);
}

Ethernet_TC *Ethernet_TC::instance() {
  if (_instance == nullptr) {
    _instance = new Ethernet_TC;
  }
  return _instance;
}

void Ethernet_TC::renewDHCPLease() {
  uint32_t current_millis = millis();
  if ((current_millis - previous_lease) >= LEASE_INTERVAL || current_millis < previous_lease) {
    Ethernet.maintain();
    previous_lease = current_millis;
  }
  numAttemptedDHCPReleases++;
}
