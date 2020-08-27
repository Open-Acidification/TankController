void HandleLines(String endpoint, EthernetClient client) {
  // lines endpoint format: lines/year/month/day/hour
  String directoryName;
  if (endpoint.length() >= 6) {
    directoryName = endpoint.substring(6);  // remove "/lines/" prefix
  } else {
    directoryName = "/";
  }
  int slashIndex = endpoint.lastIndexOf("/");
  String hourName = endpoint.substring(slashIndex);
  directoryName.concat(hourName + ".txt");
  char* pch;
  int slashes = -1;
  pch = strtok(endpoint.c_str(), "/");
  while (pch != NULL) {
    pch = strtok(NULL, "/");
    slashes += 1;
  }
  switch (slashes) {
    case 4:
      // hour: return csv of the specific hour
      PrintHeader(client, 200);
      Serial.print(F("PRINTING LINE FILE: "));
      Serial.println(directoryName);
      long lineCount = ReadLineFromSd(directoryName.c_str(), 0, 10);
      Serial.print(F("LINE COUNT: "));
      Serial.println(lineCount);
      client.println(lineCount);
      break;
    default:
      PrintHeader(client, 400);
      client.println(F("REQUIRES FULL YEAR/MONTH/DAY/HOUR SPECIFICATION"));
      break;
  }
}
