JsonDocument printCurrentLevelDirectories(File dir, EthernetClient client, int level) {
	StaticJsonDocument<512> doc;
	JsonArray filesArray = doc.createNestedArray(dir.name());
	Serial.println(dir.name());
	dir.seek(0);
	dir.rewindDirectory();
	while (true) {

		File entry =  dir.openNextFile();
		Serial.println(entry.name());
		if (!entry) {
			// no more files
			break;
		}
		char* hasLetterS = strchr(entry.name(), 'S');
		if (entry.isDirectory() && !hasLetterS) { // WILL NOT PRINT SYSTEM~1 AT ROOT LEVEL
			if (level == 3) {
				filesArray.add(entry.name());
			} else {
				StaticJsonDocument<512> directories = printCurrentLevelDirectories(entry, client, level+1);
				filesArray.add(directories);
			}
		}
		entry.close();
	}
	dir.rewindDirectory();
	return doc;
}
