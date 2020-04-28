JsonDocument printCurrentLevelDirectories(File dir, EthernetClient client, int level) {
	StaticJsonDocument<512> doc;
	client.println(dir.name());
	client.println("CURRENT LEVEL:");
	client.println(level);
	JsonArray filesArray = doc.createNestedArray(dir.name());
	while (true) {

		File entry =  dir.openNextFile();
		if (!entry) {
			// no more files
			break;
		}
		char* hasLetterS = strchr(entry.name(), 'S');
		if (entry.isDirectory() && !hasLetterS) { // WILL NOT PRINT SYSTEM~1 AT ROOT LEVEL
			client.println(entry.name());
			if (level == 3) {
				filesArray.add(entry.name());
			} else {
				StaticJsonDocument<512> directories = printCurrentLevelDirectories(entry, client, level+1);
				client.println("DIRECTORY");
				serializeJson(doc, client);
				serializeJson(directories, client);
				client.println();
				client.println("POST DIRECTORY");
				filesArray.add(directories);
			}
		}
		entry.close();
	}
	dir.rewindDirectory();
	serializeJson(doc, client);
	client.println();
	return doc;
}