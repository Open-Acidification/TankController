void printCurrentLevelDirectories(String dirName, EthernetClient client) {
	File dir = SD.open(dirName);

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	JsonArray& coords = root.createNestedArray("files");
	coords.add(48.7507371, 7);
	coords.add(2.2625587);
	root.prettyPrintTo(client);
	while (true) {

		File entry =  dir.openNextFile();
		if (!entry) {
		// no more files
		break;
		}
		char* hasLetterS = strchr(entry.name(), 'S');
		if (entry.isDirectory() && !hasLetterS) { // WILL NOT PRINT SYSTEM~1 AT ROOT LEVEL
			client.println(entry.name());
		}
		entry.close();
	}
}