void handleInfo(char* body, EthernetClient client) {	
	printHeader(client, 200);
	client.println(tankid);
}