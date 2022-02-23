#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "Devices/EthernetServer_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "SetTime.h"
#include "TankController.h"
/**
 * Trivial test to confirm that EthernetServer_TC compiles,
 * is of proper class, and has proper port
 */
unittest(echo) {
  EthernetServer_TC* server = EthernetServer_TC::instance();
  EthernetClient_CI client;
  assertEqual("EthernetServer_TC", server->className());
  assertEqual(80, server->getPort());
  server->setHasClientCalling(true);
  delay(1);        // so that start time is not zero!
  server->loop();  // accept connection
  client = server->getClient();
  assertTrue(client);
  assertEqual(0, client.writeBuffer()->size());
  const char request[] =
      "GET /echo?value=%22foo%22 HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();  // generate response
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertTrue(pBuffer->size() > 100);
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain;charset=UTF-8\r\n"
      "Content-Encoding: identity\r\n"
      "Content-Language: en-US\r\n"
      "Content-Length: 3\r\n"
      "\r\n"
      "foo";
  assertEqual(expectedResponse, response);
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();  // notify server that client stopped
}

unittest(display) {
  EthernetServer_TC* server = EthernetServer_TC::instance();
  EthernetClient_CI client;
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  client = server->getClient();
  TankController* tc = TankController::instance();
  tc->loop();  // for main menu to idle
  const char request[] =
      "GET /api/1/display HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertTrue(pBuffer->size() > 100);
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain;charset=UTF-8\r\n"
      "Content-Encoding: identity\r\n"
      "Content-Language: en-US\r\n"
      "Content-Length: 36\r\n"
      "\r\n"
      "pH=0.000   8.100\r\n"
      "T= 0.00 h 20.00 \r\n";
  assertEqual(expectedResponse, response);
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
}

unittest(keypress) {
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());
  EthernetServer_TC* server = EthernetServer_TC::instance();
  EthernetClient_CI client;
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  client = server->getClient();
  const char request[] =
      "POST /api/1/key?value=2 HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertEqual(52, pBuffer->size());
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] =
      "HTTP/1.1 303 See Other\r\n"
      "Location: /api/1/display\r\n"
      "\r\n";
  assertEqual(expectedResponse, response);
  tc->loop();  // Loop to handle the UI press
  assertEqual("Change settings ", lcd->getLines().at(0));
  delay(60000);  // IDLE_TIMEOUT
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
}

unittest(current) {
  // Fake DateTime
  DateTime_TC feb(2022, 2, 22, 20, 50, 00);
  feb.setAsCurrent();

  EthernetServer_TC* server = EthernetServer_TC::instance();
  EthernetClient_CI client;
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  client = server->getClient();
  const char request[] =
      "GET /api/1/current HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertTrue(pBuffer->size() > 100);
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain;charset=UTF-8\r\n"
      "Content-Encoding: identity\r\n"
      "Content-Language: en-US\r\n"
      "Content-Length: 241\r\n"
      "\r\n"
      "{\"IPAddress\":\"192.168.1.10\","
      "\"MAC\":\"90:A2:DA:FB:F6:F1\","
      "\"FreeMemory\":\"16 bytes\","
      "\"GoogleSheetInterval\":65535,"
      "\"LogFile\":\"20220222.csv\","
      "\"PHSlope\":\"\","
      "\"Kp\":100000.0,"
      "\"Ki\":0.0,"
      "\"Kd\":0.0,"
      "\"PID\":\"ON\","
      "\"TankID\":0,"
      "\"Uptime\":\"0d 0h 1m 1s\","
      "\"Version\":\"22.02.2\"}\r\n";
  assertEqual(expectedResponse, response);
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
}

unittest(badRequest) {
  TankController* tc = TankController::instance();
  EthernetServer_TC* server = EthernetServer_TC::instance();
  EthernetClient_CI client;
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  client = server->getClient();
  const char request[] =
      "POST /api/1/key?value=foo HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertTrue(pBuffer->size() == 28);
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] = "HTTP/1.1 400 Bad Request\r\n\r\n";
  assertEqual(expectedResponse, response);
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
}

unittest_main()
