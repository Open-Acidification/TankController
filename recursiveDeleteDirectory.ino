void recursiveDeleteDirectory(File dir, EthernetClient client) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
	client.println("REMOVING: ");
	client.println(dir.name());
    if (entry.isDirectory()) {
      	recursiveDeleteDirectory(entry, client);
    } else {
		SD.remove(entry.name());	
    }
	SD.rmdir(entry.name());
  }
  SD.rmdir(dir.name());
}