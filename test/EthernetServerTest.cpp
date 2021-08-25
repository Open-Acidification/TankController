#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include <vector>

#include "Devices/EthernetServer_TC.h"
/**
 * Trivial test to confirm that EthernetServer_TC compiles,
 * is of proper class, and has proper port
 */
unittest(basic) {
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
  client.stop();
  server->loop();  // notify server that client stopped
}

unittest_main()
