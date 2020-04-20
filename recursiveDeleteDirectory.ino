void recursiveDeleteDirectory(File dir) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    if (entry.isDirectory()) {
      	recursiveDeleteDirectory(entry, numTabs + 1, client);
    } else {
		SD.remove(entry.name());	
    }
	SD.rmdir(entry.name());
  }
  SD.rmdir(dir.name());
}