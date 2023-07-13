#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "Devices/EthernetServer_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "Devices/PHProbe.h"
#include "Devices/PID_TC.h"
#include "Devices/TempProbe_TC.h"
#include "Devices/TemperatureControl.h"
#include "SetTime.h"
#include "TankController.h"

unittest_setup() {
  GODMODE()->resetClock();
  PID_TC::instance()->setTunings(0.0, 0.0, 0.0);
}

/**
 * Trivial test to confirm that EthernetServer_TC compiles,
 * is of proper class, and has proper port
 */
unittest(echo) {
  EthernetServer_TC* server = EthernetServer_TC::instance();
  assertEqual("EthernetServer_TC", server->className());
  assertEqual(80, server->getPort());
  server->setHasClientCalling(true);
  delay(1);        // so that start time is not zero!
  server->loop();  // accept connection
  EthernetClient_CI client = server->getClient();
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
      "Access-Control-Allow-Origin: *\r\n"
      "Content-Length: 3\r\n"
      "\r\n"
      "foo";
  assertEqual(expectedResponse, response);
  assertEqual(FINISHED, server->getState());
  server->loop();  // Process finished state
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();  // notify server that client stopped
}

unittest(display) {
  EthernetServer_TC* server = EthernetServer_TC::instance();
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  EthernetClient_CI client = server->getClient();
  TankController* tc = TankController::instance();
  tc->loop(false);  // for main menu to idle
  const char request[] =
      "GET /api/1/display HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  tc->loop(false);  // for targets to take effect
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
      "Access-Control-Allow-Origin: *\r\n"
      "Content-Length: 36\r\n"
      "\r\n"
      "pH 0.000   8.100\r\n"
      "T  0.00 H 20.00 \r\n";
  assertEqual(expectedResponse, response);
  assertEqual(FINISHED, server->getState());
  server->loop();  // Process finished state
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
}

unittest(keypress) {
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());
  EthernetServer_TC* server = EthernetServer_TC::instance();
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  EthernetClient_CI client = server->getClient();
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
  assertEqual(84, pBuffer->size());
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] =
      "HTTP/1.1 303 See Other\r\n"
      "Location: /api/1/display\r\n"
      "Access-Control-Allow-Origin: *\r\n"
      "\r\n";
  assertEqual(expectedResponse, response);
  assertEqual(FINISHED, server->getState());
  tc->loop(false);  // Loop to handle the UI press
  assertEqual("Change settings ", lcd->getLines().at(0));
  delay(60000);  // IDLE_TIMEOUT
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
}

unittest(currentData) {
  // Fake DateTime
  DateTime_TC feb(2022, 2, 22, 20, 50, 00);
  feb.setAsCurrent();
  PHProbe::instance()->setPh(8.125);                            // actual
  PHControl::instance()->setBaseTargetPh(8.25);                 // target
  TempProbe_TC::instance()->setTemperature(21.25, true);        // actual
  TemperatureControl::instance()->setTargetTemperature(21.75);  // target
  TankController::instance()->loop(false);                      // for targets to take effect
  PID_TC::instance()->setTunings(5000.55, 1234.46, 987.44);

  EthernetServer_TC* server = EthernetServer_TC::instance();
  server->setHasClientCalling(true);
  delay(1000);
  server->loop();
  EthernetClient_CI client = server->getClient();
  const char request[] =
      "GET /api/1/data HTTP/1.1\r\n"
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
      "Access-Control-Allow-Origin: *\r\n"
      "Content-Length: 320\r\n"
      "\r\n"
      "{"
      "\"pH\":8.125,"
      "\"Target_pH\":8.25,"
      "\"Temperature\":21.25,"
      "\"TargetTemperature\":21.75,"
      "\"IPAddress\":\"192.168.1.10\","
      "\"MAC\":\"90:A2:DA:FB:F6:F1\","
      "\"FreeMemory\":\"1024 bytes\","
      "\"GoogleSheetInterval\":65535,"
      "\"LogFile\":\"20220222.csv\","
      "\"PHSlope\":\"\","
      "\"Kp\":5000.6,"
      "\"Ki\":1234.5,"
      "\"Kd\":987.4,"
      "\"PID\":\"ON\","
      "\"TankID\":0,"
      "\"Uptime\":\"0d 0h 0m 1s\","
      "\"Version\":\"23.7.0 \""
      "}\r\n";
  assertEqual(expectedResponse, response);
  assertEqual(FINISHED, server->getState());
  server->loop();  // Process finished state
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
}

unittest(badRequest) {
  TankController* tc = TankController::instance();
  EthernetServer_TC* server = EthernetServer_TC::instance();
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  EthernetClient_CI client = server->getClient();
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
  assertEqual(FINISHED, server->getState());
  server->loop();  // Process finished state
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
}

unittest(rootDir) {
  TankController* tc = TankController::instance();
  EthernetServer_TC* server = EthernetServer_TC::instance();
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  EthernetClient_CI client = server->getClient();
  const char request[] =
      "GET /api/1/rootdir HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();
  assertEqual(COUNTING_FILES, server->getState());
  server->loop();
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertEqual(164, pBuffer->size());
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
      "Access-Control-Allow-Origin: *\r\n"
      "Content-Length: 49\r\n"
      "\r\n";
  assertEqual(expectedResponse, response);
  assertEqual(LISTING_FILES, server->getState());
  server->loop();
  pBuffer = client.writeBuffer();
  assertEqual(49, pBuffer->size());
  response.clear();
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char nextExpectedResponse[] = "Root directory not supported by CI framework.\r\n\r\n";
  assertEqual(nextExpectedResponse, response);
  assertEqual(FINISHED, server->getState());
  server->loop();  // Process finished state
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
}

unittest(fileNotFound) {
  TankController* tc = TankController::instance();
  EthernetServer_TC* server = EthernetServer_TC::instance();
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  EthernetClient_CI client = server->getClient();
  const char request[] =
      "GET /17760704.log HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertEqual(26, pBuffer->size());
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] = "HTTP/1.1 404 Not Found\r\n\r\n";
  assertEqual(expectedResponse, response);
  assertEqual(FINISHED, server->getState());
  server->loop();  // Process finished state
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
}

unittest(options) {
  TankController* tc = TankController::instance();
  EthernetServer_TC* server = EthernetServer_TC::instance();
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  EthernetClient_CI client = server->getClient();
  const char request[] =
      "OPTIONS /index.html HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertEqual(205, pBuffer->size());
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
      "Access-Control-Allow-Origin: *\r\n"
      "Access-Control-Allow-Methods: OPTIONS, GET, HEAD, POST, PUT\r\n\r\n";
  assertEqual(expectedResponse, response);
  assertEqual(FINISHED, server->getState());
  server->loop();  // Process finished state
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
}

unittest(timeout) {
  TankController* tc = TankController::instance();
  EthernetServer_TC* server = EthernetServer_TC::instance();
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  EthernetClient_CI client = server->getClient();
  server->loop();
  assertEqual(READ_REQUEST, server->getState());
  delay(50);
  server->loop();
  delay(50);
  server->loop();
  delay(50);
  server->loop();
  assertEqual(READ_REQUEST, server->getState());
  delay(5000);  // Wait for timeout
  server->loop();
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertEqual(51, pBuffer->size());
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] = "HTTP/1.1 408 Request Timeout\r\nConnection: close\r\n\r\n";
  assertEqual(expectedResponse, response);
  assertEqual(FINISHED, server->getState());
  server->loop();  // Process finished state
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
}

unittest(notImplemented) {
  TankController* tc = TankController::instance();
  EthernetServer_TC* server = EthernetServer_TC::instance();
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  EthernetClient_CI client = server->getClient();
  const char request[] =
      "FOO /index.html HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertEqual(32, pBuffer->size());
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] = "HTTP/1.1 501 Not Implemented\r\n\r\n";
  assertEqual(expectedResponse, response);
  assertEqual(FINISHED, server->getState());
  server->loop();  // Process finished state
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
}

unittest(PUT_Kd) {
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());

  PID_TC* singleton = PID_TC::instance();
  assertEqual(0.0, singleton->getKd());

  EthernetServer_TC* server = EthernetServer_TC::instance();
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  EthernetClient_CI client = server->getClient();
  const char request[] =
      "PUT /api/1/data?Kd=1.0 HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertEqual(81, pBuffer->size());
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] =
      "HTTP/1.1 303 See Other\r\n"
      "Location: /api/1/data\r\n"
      "Access-Control-Allow-Origin: *\r\n"
      "\r\n";
  assertEqual(expectedResponse, response);
  assertEqual(FINISHED, server->getState());
  tc->loop();  // Loop to handle the UI press
  assertEqual("MainMenu", tc->stateName());
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
  assertEqual(1.0, singleton->getKd());
}

unittest(PUT_Ki) {
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());

  PID_TC* singleton = PID_TC::instance();
  assertEqual(0.0, singleton->getKi());

  EthernetServer_TC* server = EthernetServer_TC::instance();
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  EthernetClient_CI client = server->getClient();
  const char request[] =
      "PUT /api/1/data?Ki=100.50 HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertEqual(81, pBuffer->size());
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] =
      "HTTP/1.1 303 See Other\r\n"
      "Location: /api/1/data\r\n"
      "Access-Control-Allow-Origin: *\r\n"
      "\r\n";
  assertEqual(expectedResponse, response);
  assertEqual(FINISHED, server->getState());
  tc->loop();  // Loop to handle the UI press
  assertEqual("MainMenu", tc->stateName());
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
  assertEqual(100.5, singleton->getKi());
}

unittest(PUT_Kp) {
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());

  PID_TC* singleton = PID_TC::instance();
  assertEqual(0.0, singleton->getKp());

  EthernetServer_TC* server = EthernetServer_TC::instance();
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  EthernetClient_CI client = server->getClient();
  const char request[] =
      "PUT /api/1/data?Kp=1000.125 HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertEqual(81, pBuffer->size());
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] =
      "HTTP/1.1 303 See Other\r\n"
      "Location: /api/1/data\r\n"
      "Access-Control-Allow-Origin: *\r\n"
      "\r\n";
  assertEqual(expectedResponse, response);
  assertEqual(FINISHED, server->getState());
  tc->loop();  // Loop to handle the UI press
  assertEqual("MainMenu", tc->stateName());
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
  assertEqual(1000.125, singleton->getKp());
}

unittest(home) {
  EthernetServer_TC* server = EthernetServer_TC::instance();
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  EthernetClient_CI client = server->getClient();
  const char request[] =
      "GET / HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertEqual(99, pBuffer->size());
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] =
      "HTTP/1.1 303 See Other\r\n"
      "Location: http://oap.cs.wallawalla.edu/\r\n"
      "Access-Control-Allow-Origin: *\r\n"
      "\r\n";
  assertEqual(expectedResponse, response);
  server->loop();
  assertEqual(NOT_CONNECTED, server->getState());
  client.stop();
  server->loop();
}

unittest_main()
