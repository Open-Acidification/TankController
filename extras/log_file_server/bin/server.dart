import 'dart:async' show Future;
import 'dart:io';

import 'package:shelf/shelf.dart';
import 'package:shelf/shelf_io.dart';
import 'package:shelf_router/shelf_router.dart';

// Configure routes.
final _router = Router()
  ..head('/logs/<path>', _headPath)
  ..post('/logs/<path>', _postPath);

Future<Response> _headPath(Request req, String path) async {
  return Response.ok('', headers: {'content-length': '99'});
}

Future<Response> _postPath(Request req, String path) async {
  // validate received data
  var mimeType = req.mimeType;
  if (mimeType != 'text/plain') {
    return Response.badRequest(body: 'Content-Type must be text/plain!');
  }
  if (req.isEmpty) {
    return Response.badRequest(body: 'Body must not be empty!');
  }

  // get body
  var length = req.contentLength;
  var body = await req.readAsString();
  if (length != body.length) {
    return Response.badRequest(
        body: 'Content-Length of $length '
            'did not match body.length of ${body.length}!');
  }
  print('body = "$body"');

  // get remote address
  var connectionInfo =
      req.context['shelf.io.connection_info'] as HttpConnectionInfo;
  var remoteAddress = connectionInfo.remoteAddress.address;
  print('remoteAddress = "$remoteAddress" (${remoteAddress.runtimeType})');

  return Response.ok('');
}

void main(List<String> args) async {
  // Use any available host or container IP (usually `0.0.0.0`).
  final ip = InternetAddress.anyIPv4;

  // Configure a pipeline that logs requests.
  final handler = Pipeline().addMiddleware(logRequests()).addHandler(_router);

  // For running in containers, we respect the PORT environment variable.
  final port = int.parse(Platform.environment['PORT'] ?? '8080');
  final server = await serve(handler, ip, port);
  print('Server listening on port ${server.port}');
}
