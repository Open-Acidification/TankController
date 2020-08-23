void recursiveDeleteDirectory(String dirName, EthernetClient client) {
  File dir = SD.open(dirName);
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }
    client.println(F("REMOVING: "));
    if (entry.isDirectory()) {
      client.println(dirName);
      recursiveDeleteDirectory(dirName + "/" + entry.name(), client);
    } else {
      client.println(dirName + "/" + entry.name());
      SD.remove(dirName + "/" + entry.name());
    }
    SD.rmdir(entry.name());
  }
  SD.rmdir(dirName);
}