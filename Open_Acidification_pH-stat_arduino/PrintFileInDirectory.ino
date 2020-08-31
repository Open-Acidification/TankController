void PrintFileInDirectory(String dirName, EthernetClient client) {
  File my_file = SD.open(dirName);
  if (my_file) {
    client.println(F("HTTP/1.1 200 OK"));
    client.println(F("Content-Type: text/plain; charset=UTF-8"));
    client.println(F("Connection: keep-alive"));  // the connection will be closed after completion of the response
    client.println(F("Keep-Alive: timeout=5, max=1000"));
    client.println(F("X-Content-Type-Options: nosniff"));
    client.println();
    client.println(F("time,tank_id,temp,temp setpoint,pH,pH setpoint,on_time"));
    Serial.println(F("time,tank_id,temp,temp setpoint,pH,pH setpoint,on_time"));
    // read from the file until there's nothing else in it:
    while (my_file.available()) {
      byte read = my_file.read();
      client.write(read);
      Serial.write(read);
    }
    // close the file:
    my_file.close();
  } else {
    // if the file didn't open, print an error:
    client.println(F("HTTP/1.1 404 Not Found"));
    client.print(F("error opening file at directory"));
    client.println(dirName);
  }
}
