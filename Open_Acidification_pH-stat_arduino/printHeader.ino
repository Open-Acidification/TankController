void printHeader(EthernetClient client, int code) {
  switch (code) {
    case 200:
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain; charset=UTF-8");
      client.println("Connection: keep-alive");  // the connection will be closed after completion of the response
      client.println("Keep-Alive: timeout=5, max=1000");
      client.println("X-Content-Type-Options: nosniff");
      client.println();
      break;
    case 400:
      client.println("HTTP/1.1 400 Bad Request");
    case 416:
      client.println("HTTP/1.1 416 Requested Range Not Satisfiable");
      break;
    default:
      client.println("HTTP/1.1 404 Not Found");
  }
}