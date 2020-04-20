void recursiveDeleteDirectory(File dir) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    if (entry.isDirectory()) {
      	recursiveDeleteDirectory(entry);
    } else {
		SD.remove(entry.name());	
    }
	SD.rmdir(entry.name());
  }
  SD.rmdir(dir.name());
}