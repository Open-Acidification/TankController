void printFileInDirectory(String dirName, EthernetClient client) {	
	File myFile = SD.open(dirName);
	if (myFile) {
		client.println("HTTP/1.1 200 OK");
		client.println("Content-Type: text/plain; charset=UTF-8");
		client.println("Connection: keep-alive");  // the connection will be closed after completion of the response
		client.println("Keep-Alive: timeout=5, max=1000");
		client.println("X-Content-Type-Options: nosniff");
		client.println();
		client.println("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime");
		Serial.println("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime");
		// read from the file until there's nothing else in it:
		while (myFile.available()) {
			byte read = myFile.read();
			client.write(read);
			Serial.write(read);
		}
		// close the file:
		myFile.close();
	} else {
		// if the file didn't open, print an error:
		client.println("HTTP/1.1 404 Not Found");
		client.print("error opening file at directory");
		client.println(dirName);
	}
}