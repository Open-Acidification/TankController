void printDirectoryToClient(File dir, int numTabs, EthernetClient client) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      client.print('\t');
    }
    client.print(entry.name());
    if (entry.isDirectory()) {
      client.println("/");
      printDirectoryToClient(entry, numTabs + 1, client);
    } else {
      // files have sizes, directories do not
      client.print("\t\t");
      client.println(entry.size(), DEC);
    }
    entry.close();
  }
}