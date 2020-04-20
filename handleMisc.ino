void handleMisc(EthernetClient client) {	  
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/html");
	client.println("Connection: close");  // the connection will be closed after completion of the response
	client.println("Refresh: 5");  // refresh the page automatically every 5 sec
	client.println();
	client.println("<!DOCTYPE HTML>");
	client.println("<html>");
	// output the value of each analog input pin
	// for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
	// 	int sensorReading = analogRead(analogChannel);
	// 	client.print("analog input ");
	// 	client.print(analogChannel);
	// 	client.print(" is ");
	// 	client.print(sensorReading);
	// 	client.println("<br />");
	// }
	client.println("</html>");
}