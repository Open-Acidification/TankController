void PrintSpecifiedLines(String dirName, EthernetClient client, long starting_line, long num_lines) {
  File my_file = SD.open(dirName);
  if (my_file) {
    Serial.print(F("starting_line: "));
    Serial.println(starting_line);
    Serial.print(F("num_lines: "));
    Serial.println(num_lines);
    int printed_lines = 0;
    // seek starting byte
    long starting_byte = starting_line * (RECORD_LENGTH + 2);
    if (my_file.seek(starting_byte)) {
      Serial.println(F("SUCCESSFUL SEEK"));
      PrintHeader(client, 200);
      client.println(F("time,tank_id,temp,temp setpoint,pH,pH setpoint,on_time"));
      Serial.println(F("time,tank_id,temp,temp setpoint,pH,pH setpoint,on_time"));
    } else {
      Serial.println(F("FAILED SEEK"));
      PrintHeader(client, 416);
      // if the starting byte is greater than file size, print an error:
      client.print(F("starting byte is greater than file size at file: "));
      client.println(dirName);
      // close the file:
      my_file.close();
      return;
    }
    // print specified lines
    while (printed_lines++ < num_lines) {
      // fill up buffer
      char line[100];
      memset(line, 0, 100);
      for (int i = 0; i < 100 && my_file.available(); i++) {
        byte read = my_file.read();
        if ((char)read == '\n') {
          break;
        }
        line[i] = read;
      }
      // write buffer to client
      client.write(line, strlen(line));
      Serial.write(line, strlen(line));
    }
    // close the file:
    my_file.close();
  } else {
    // if the file didn't open, print an error:
    PrintHeader(client, 404);
    client.print(F("error opening file at directory"));
    client.println(dirName);
  }
}
