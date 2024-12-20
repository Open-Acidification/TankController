import 'dart:async';

import 'package:shelf/shelf.dart';
import 'package:shelf/shelf_io.dart' as io;
import 'package:shelf_proxy/shelf_proxy.dart';

void main() async {
  // Proxy to the Flutter development server
  final flutterProxy = proxyHandler('http://localhost:8081');

  // Proxy to the data server
  final serverProxy = proxyHandler('http://localhost:8082');

  // Define a handler that routes requests based on the path
  FutureOr<Response> handler(Request request) {
    if (request.url.path.startsWith('api/')) {
      return serverProxy(request);
    } else {
      return flutterProxy(request);
    }
  }

  // Wrap the handler with logRequests for debugging
  final loggedHandler = logRequests().addHandler(handler);

  // Start the server
  await io.serve(loggedHandler, 'localhost', 8080);
  print(
    'Proxy server listening on 8080 and forwarding to 8081 (app) '
    'and 8082 (data server).\nChange your web browser to http://localhost:8080/.',
  );
}
