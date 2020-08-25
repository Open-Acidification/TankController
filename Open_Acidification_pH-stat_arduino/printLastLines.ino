void PrintLastLines(String dirName, EthernetClient client, long lastLines) {
  File myFile = SD.open(dirName);
  if (myFile) {
    Serial.print(F("LASTLINES: "));
    Serial.println(lastLines);
    int printedLines = 0;
    // seek starting byte
    String lineDirName = dirName.substring(0, dirName.length() - 12) + dirName.substring(dirName.length() - 6);
    Serial.println(F("lineDirName"));
    Serial.println(lineDirName);
    long lineCount = ReadLineFromSD(lineDirName.c_str(), 0, 10);
    long startingLine = lineCount - lastLines;
    long startingByte = startingLine * (RECORD_LENGTH + 2);
    if (myFile.seek(startingByte)) {
      Serial.println(F("SUCCESSFUL SEEK"));
      PrintHeader(client, 200);
      client.println(F("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime"));
      Serial.println(F("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime"));
    } else {
      Serial.println(F("FAILED SEEK"));
      PrintHeader(client, 400);
      // if the starting byte is greater than file size, print an error:
      client.print(F("starting byte is greater than file size at file: "));
      client.println(dirName);
      // close the file:
      myFile.close();
      return;
    }
    // print specified lines
    while (printedLines++ < lastLines) {
      // fill up buffer
      char line[100];
      memset(line, 0, 100);
      for (int i = 0; i < 100 && myFile.available(); i++) {
        byte read = myFile.read();
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
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    client.println(F("error opening file at directory: "));
    client.println(dirName);
  }
}