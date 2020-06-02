void handleInfo(EthernetClient client) {	
	printHeader(client, 200);
	client.println(tankid);
}