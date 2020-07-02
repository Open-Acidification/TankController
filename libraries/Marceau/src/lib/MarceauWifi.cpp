#ifdef ESP8266

#include "Arduino.h"
#include "MarceauWifi.h"

Ticker sta_tick;

bool MarceauWifi::networkChanged = false;
bool MarceauWifi::online = false;
bool MarceauWifi::wifiScanRequested = false;
bool MarceauWifi::wifiScanReady = false;
MarceauSettings *MarceauWifi::settings;
WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;

MarceauWifi::MarceauWifi() {
  enabled = false;
  online = false;
  hostname = NULL;
  _defaultAPName = NULL;
  WiFi.mode(WIFI_OFF);
  wifiScanRequested = false;
}

void MarceauWifi::begin(MarceauSettings * _settings){
  settings = _settings;

  // Subscribe to state change events
  gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event){
      MarceauWifi::networkChanged = true;
      MarceauWifi::online = true;
  });
  disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event){
      MarceauWifi::networkChanged = true;
      MarceauWifi::online = false;
  });
  
  setupWifi();
  setupDNS();

  enabled = true;
}

void MarceauWifi::setDefaultAPName(char * apname){
  _defaultAPName = apname;
}

void MarceauWifi::defaultAPName(char * name){
  uint8_t mac[6];
  WiFi.softAPmacAddress(mac);
  if(_defaultAPName == NULL){
    sprintf(name, "Marceau-%02X%02X", mac[4], mac[5]);
  }else{
    strcpy(name, _defaultAPName);
  }
}

IPAddress MarceauWifi::getStaIp(){
  return WiFi.localIP();
}

int32_t MarceauWifi::getStaRSSI(){
  return WiFi.RSSI();
}

WiFiMode MarceauWifi::getWifiMode(){
  return WiFi.getMode();
}

void MarceauWifi::setupWifi(){
  // Don't let the ESP SDK persist the settings since we do this ourselves
  WiFi.persistent(false);

  // Put the WiFi into AP_STA mode
  WiFi.mode(WIFI_AP_STA);

  // Reinitialise the WiFi
  WiFi.disconnect();

  // Set the hostname for DHCP
  WiFi.hostname(settings->ap_ssid);

  // Set up the access point
  if(strlen(settings->ap_pass)){
    WiFi.softAP(settings->ap_ssid, settings->ap_pass);
  }else{
    WiFi.softAP(settings->ap_ssid);
  }

  if(strlen(settings->sta_ssid)){
    // Configure the fixed IP if we're not using DHCP
    if(!settings->sta_dhcp && settings->sta_fixedip && settings->sta_fixedgateway && settings->sta_fixednetmask){
      WiFi.config(IPAddress(settings->sta_fixedip), IPAddress(settings->sta_fixedgateway), IPAddress(settings->sta_fixednetmask));
    }

    // Set up the STA connection
    WiFi.begin(settings->sta_ssid, settings->sta_pass);
    // Check if it's connected after 10 seconds
    sta_tick.attach(10, MarceauWifi::staCheck);
  }else{
    WiFi.mode(WIFI_AP);
  }
}

void MarceauWifi::startWifiScan(){
  wifiScanRequested = true;
  WiFi.scanNetworks(true, true);
}

int MarceauWifi::getWifiScanData(ArduinoJson::JsonArray &msg, int done){
  bool rejected;
  bool sent;
  uint8_t chunk_size = 5;
  uint8_t item_index = 0;
  int count = WiFi.scanComplete();
  if(done == count -1) return -1;
  if(count >= 0){
    // We've got some results to send
    for (int i = 0; i < count; i++){
      // Check if we have already sent this SSID and filter if so
      rejected = false;
      for(int j=0; j< i; j++){
        if(WiFi.SSID(i) == WiFi.SSID(j)){
          rejected = true;
          break;
        }
      }
      // Filter this SSID if we've already sent it
      sent = (item_index < done);

      // Add it to the result if we're not filtering it
      if(WiFi.SSID(i) && WiFi.RSSI(i) && !rejected && !sent){
        JsonArray& net = msg.createNestedArray();
        net.add(WiFi.SSID(i));
        net.add(WiFi.encryptionType(i) != ENC_TYPE_NONE);
        net.add(WiFi.RSSI(i));
      }

      if(!rejected) item_index++;

      // Return if we've hit this chunk size
      if(item_index > done + chunk_size -1){
        return item_index;
      }
    }
    if(msg.size() == 0){
      return -1;
    }else{
      return item_index;
    }
  }
}

void MarceauWifi::setupDNS(){
  if(hostname == NULL){
    hostname = "local.marceau.com";
  }
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(53, hostname, IPAddress(192, 168, 4, 1));
}

void MarceauWifi::setHostname(char * _hostname){
  hostname = _hostname;
}

void MarceauWifi::staCheck(){
  sta_tick.detach();
  if(!(uint32_t)WiFi.localIP()){
    WiFi.mode(WIFI_AP);
  }
}

void MarceauWifi::loop(){
  if(!enabled) return;
  dnsServer.processNextRequest();
  if(wifiScanRequested && WiFi.scanComplete() >= 0){
    wifiScanRequested = false;
    wifiScanReady = true;
  }
}

#endif
