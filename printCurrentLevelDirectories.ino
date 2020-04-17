void printCurrentLevelDirectories(String dirName, EthernetClient client) {
	File dir = SD.open(dirName);
	while (true) {

		File entry =  dir.openNextFile();
		if (!entry) {
		// no more files
		break;
		}
		if (entry.isDirectory()) { // WILL PRINT SYSTEM~1 AT ROOT LEVEL
			client.println(entry.name());
		}
		entry.close();
	}
}