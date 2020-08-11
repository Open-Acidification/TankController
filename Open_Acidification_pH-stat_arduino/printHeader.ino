void printHeader(EthernetClient client, int code) {
  switch (code) {
    case 200:
      client.println(F("HTTP/1.1 200 OK"));
      client.println(F("Content-Type: text/plain; charset=UTF-8"));
      client.println(F("Connection: keep-alive"));  // the connection will be closed after completion of the response
      client.println(F("Keep-Alive: timeout=5, max=1000"));
      client.println(F("X-Content-Type-Options: nosniff"));
      client.println();
      break;
    case 400:
      client.println(F("HTTP/1.1 400 Bad Request"));
    case 416:
      client.println(F("HTTP/1.1 416 Requested Range Not Satisfiable"));
      break;
    default:
      client.println(F("HTTP/1.1 404 Not Found"));
  }
}