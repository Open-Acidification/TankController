#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include <vector>

#include "Devices/EthernetServer_TC.h"
#include "DateTime_TC.h"
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
  assertEqual("EthernetServer_TC", server->className());
  assertEqual(80, server->getPort());
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  client = server->getClient();
  assertTrue(client);
  assertEqual(0, client.writeBuffer()->size());
  // Fake a SetTime UI display we know for now
  TankController* tc = TankController::instance();
  DateTime_TC june(2021, 06, 01, 20, 57, 15);
  june.setAsCurrent();
  SetTime* state = new SetTime(tc);
  tc->setNextState(state, true);
  const char request[] =
      "GET /api/1/display HTTP/1.1\r\n"
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
      "Content-Length: 36\r\n"
      "\r\n"
	  "Set Year (YYYY):\r\n"
      "   2021->     0 \r\n";
  assertEqual(expectedResponse, response);
  client.stop();
  server->loop();  // notify server that client stopped
  assertEqual(NOT_CONNECTED, server->getState());
  assertEqual("SetTime", tc->stateName());
  // Go through SetTime State (Maybe should use another one?)
  state->setValue(2020);
  state->setValue(03);
  state->setValue(18);
  state->setValue(13);
  state->setValue(15);
  tc->loop();  // transition to 3 second Wait state
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest(keypress) {
  TankController* tc = TankController::instance();
  assertEqual("MainMenu", tc->stateName());
  EthernetServer_TC* server = EthernetServer_TC::instance();
  EthernetClient_CI client;
  server->setHasClientCalling(true);
  delay(1);
  server->loop();
  client = server->getClient();

  const char request[] =
      "PUT /api/1/key?value=%22A%22 HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();  // generate response
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertTrue(pBuffer->size() == 62);
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] =
      "HTTP/1.1 303 See Other\r\n"
      "Location: localhost:80/api/1/display\r\n";
  assertEqual(expectedResponse, response);
  tc->loop(); // Loop to handle the UI press
  server->setHasClientCalling(true); // Get new client since old one stopped
//   delay(1);
  server->loop();
  client = server->getClient();
  pBuffer = client.writeBuffer(); // Maybe unnecessary?
  const char secondRequest[] =
	  "GET /api/1/display HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  assertEqual(0, pBuffer->size());
  client.pushToReadBuffer(secondRequest);
  server->loop();  // generate response
  assertTrue(pBuffer->size() > 100);
  String secondResponse;
  while (!pBuffer->empty()) {
    secondResponse.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedSecondResponse[] =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain;charset=UTF-8\r\n"
      "Content-Encoding: identity\r\n"
      "Content-Language: en-US\r\n"
      "Content-Length: 36\r\n"
      "\r\n"
	  "Set pH Set Point\r\n"
      "  8.100->     0 \r\n";
  assertEqual(expectedSecondResponse, secondResponse);
  assertEqual("SetPHSetPoint", tc->stateName());
  client.stop(); // Unnecessary since the response already stopped client?
  server->loop();  // notify server that client stopped
  delay(60L * 1000L); // idle timeout to avoid more keypress requests
  tc->loop(); // changes state to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
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
      "PUT /api/1/key?value=%22foo%22 HTTP/1.1\r\n"
      "Host: localhost:80\r\n"
      "Accept: text/plain;charset=UTF-8\r\n"
      "Accept-Encoding: identity\r\n"
      "Accept-Language: en-US\r\n"
      "\r\n";
  client.pushToReadBuffer(request);
  server->loop();  // generate response
  deque<uint8_t>* pBuffer = client.writeBuffer();
  assertTrue(pBuffer->size() == 26);
  String response;
  while (!pBuffer->empty()) {
    response.concat(pBuffer->front());
    pBuffer->pop_front();
  }
  const char expectedResponse[] =
      "HTTP/1.1 400 Bad Request\r\n";
  assertEqual(expectedResponse, response);
  client.stop(); // Unnecessary since the response already stopped client?
  server->loop();  // notify server that client stopped
}

unittest_main()
