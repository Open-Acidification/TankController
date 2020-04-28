JsonDocument printCurrentLevelDirectories(File dir, EthernetClient client, int level) {
	StaticJsonDocument<1024> doc;
	client.println(dir.name());
	client.println("CURRENT LEVEL:");
	client.println(level);
	JsonArray filesArray = doc.createNestedArray(dir.name());
	while (true) {

		File entry =  dir.openNextFile();
		if (!entry) {
			// no more files
			dir.rewindDirectory();
			break;
		}
		char* hasLetterS = strchr(entry.name(), 'S');
		if (entry.isDirectory() && !hasLetterS) { // WILL NOT PRINT SYSTEM~1 AT ROOT LEVEL
			client.println(entry.name());
			if (level == 3) {
				while (true) {
					File leafEntry = dir.openNextFile();
					if (!leafEntry) {
						dir.rewindDirectory();
						break;
					}
					filesArray.add(leafEntry.name());
				}
			} else {
				StaticJsonDocument<1024> directories = printCurrentLevelDirectories(entry, client, level+1);
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
	serializeJson(doc, client);
	client.println();
	return doc;
}
