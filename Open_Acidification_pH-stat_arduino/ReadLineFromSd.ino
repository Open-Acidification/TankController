long ReadLineFromSd(char* file_name, int line, int lineLength) {
  long starting_byte = line * (lineLength + 2);
  long line_value;

  File temp_file = SD.open(file_name);
  if (temp_file.seek(starting_byte)) {
    // fill up buffer
    char line[lineLength];
    memset(line, 0, lineLength);
    int i;
    for (i = 0; i < lineLength && temp_file.available(); i++) {
      byte read = temp_file.read();
      if ((char)read == '\n') {
        break;
      }
      line[i] = read;
    }
    line[i] = '\0';
    line_value = atoi(line);
  } else {
    Serial.println(F("FAILED READ LINE SEEK"));
  }

  // close the file:
  temp_file.close();

  return line_value;
}
