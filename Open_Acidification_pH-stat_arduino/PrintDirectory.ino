void PrintDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println(F("/"));
      PrintDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print(F("\t\t"));
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
