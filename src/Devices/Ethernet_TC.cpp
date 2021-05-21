#include "Ethernet_TC.h"

#include "Serial_TC.h"

Ethernet_TC *Ethernet_TC::_instance = nullptr;

// Establishes the Ethernet connection and sets class variables
Ethernet_TC::Ethernet_TC() {
  if (Ethernet.begin(mac) == 0) {
    serial("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, defaultIP);
    IP = defaultIP;
  }
}

Ethernet_TC *Ethernet_TC::instance() {
  if (_instance == nullptr) {
    _instance = new Ethernet_TC;
  }
  return _instance;
}

void Ethernet_TC::sendPushingBoxData(String data, byte server[], EthernetClient &client) {
  if(client.connect(server, 80))
    client.write(data.c_str(), data.size());
}

void Ethernet_TC::renewDHCPLease() {
  unsigned long current_millis = millis();
  if ((current_millis - previous_lease) >= LEASE_INTERVAL || current_millis < previous_lease) {
    Ethernet.maintain();
    previous_lease = current_millis;
  }
  numAttemptedDHCPReleases++;
}
