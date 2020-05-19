void handleDevice(String endpoint, EthernetClient client) {	
	// next path token will be file name
	int index = endpoint.indexOf("/", 1);
	String fileName = endpoint.substring(index+1);
	fileName.trim();
	fileName.concat(".txt");
	Serial.println("SD FILE NAME");
	Serial.println(fileName);
	printHeader(client, 200);

	volatile File myFile = SD.open(fileName, FILE_READ);
	Serial.println("Reading file: ");
	int CHUNK_SIZE = 128;
	volatile char tBuf[CHUNK_SIZE];
	for (int i = 0; i < sizeof(tBuf); i++) {
		tBuf[i] = ' ';
	}
	bool isHeader = true;
	bool USE_COMPRESSION = false;
	String prev;
	String current;
	if (myFile) {
		if (USE_COMPRESSION) {
			while (myFile.available()) {
				int clientCount = myFile.read(tBuf, CHUNK_SIZE);
				char* p;
				p = strtok(tBuf, "\n");
				if (p) {
					if (isHeader) {
						isHeader = false;
					} else {
						String q = String(p);
						int commaIndex = q.indexOf(',');
						current = q.substring(commaIndex);
						if (current.equals(prev)) {
							// Serial.println("EQUAL");
						} else {
							// Serial.print("LINE: ");
							// Serial.write((byte*)p, strlen(p));
							client.write((byte*)p, strlen(p));
						}
						prev = current;
					}
					if (!myFile.available()) {
						break;
					}
					myFile.seek(myFile.position() - (CHUNK_SIZE - strlen(p)) + 1);
				}
			}
			myFile.close();
		} else {
			Serial.println("FILE SIZE: ");
			Serial.println(myFile.size());
			long totalRead = 0;
			while (myFile.available()) {
				if (totalRead > 400000) {
					// transmission overloaded- force disconnect
					myFile.close();
					break;
				}
				int bytesAvailable = myFile.available();
				Serial.println("AVAILABLE: ");
				Serial.println(bytesAvailable);
				if (bytesAvailable > freeMemory()) {
					// splice large chunk into sizeable chunks
					Serial.println("Taking Splice Strategy");
					while (bytesAvailable > 0) {
						int tempFreeMemory = freeMemory() - 1000;
						Serial.println("\ntemp memory allocated: ");
						Serial.println(tempFreeMemory);
						char * buffer = malloc(tempFreeMemory);
						int lastByteRead = myFile.read(buffer, tempFreeMemory);
						int lastByteWritten = client.write(buffer, tempFreeMemory);
						Serial.println("\nlastByteRead ");
						Serial.println(lastByteRead);
						Serial.println("\nlastByteWritten: ");
						Serial.println(lastByteWritten);

						free(buffer);
						Serial.println("\nInside Splice Strategy: ");
						Serial.println(bytesAvailable);
						Serial.println("\nTotal Bytes Read: ");
						Serial.println(totalRead);
						bytesAvailable -= tempFreeMemory;
						totalRead += tempFreeMemory;
					}
				} else {
					char * buffer = malloc(bytesAvailable);
					myFile.read(buffer, bytesAvailable);
					client.write(buffer, bytesAvailable);
					free(buffer);
				}
				// myFile.read(tBuf, CHUNK_SIZE);
				// client.write((byte*)tBuf, CHUNK_SIZE);
				// Serial.write((byte*)tBuf, CHUNK_SIZE);
				Serial.println("\nfreeMemory: ");
				Serial.println(freeMemory());
				client.write(myFile.read());
				Serial.println("AVAILABLE: ");
				Serial.println(myFile.available());

			}
			Serial.println("MYFILE NO LONGER AVAILABLE");
			myFile.close();

		}
	}
	// if the file isn't open, pop up an error:
	else {
		Serial.print("error opening ");
		Serial.println(fileName);
	}
}