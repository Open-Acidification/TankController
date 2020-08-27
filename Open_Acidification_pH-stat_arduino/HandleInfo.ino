void HandleInfo(EthernetClient client) {
  PrintHeader(client, 200);
  client.println(tankid);
}
