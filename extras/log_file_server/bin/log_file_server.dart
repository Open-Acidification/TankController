import 'dart:async' show Future, Timer;
import 'dart:io';

import 'package:csv/csv.dart';
import 'package:shelf/shelf.dart';
import 'package:shelf/shelf_io.dart';
import 'package:shelf_router/shelf_router.dart';

String rootDir =
    Platform.localHostname == 'oap-vm' ? '/var/opt/oap/logs' : './test/logs';

// Configure routes.
final _router = Router()
  ..get('/api/<path>', _get)
  ..post('/logs/<path>', _post);

// Get snapshots of log files
Future<Response> _get(Request req, String path) async {
  final file = File('$rootDir/${path.split('/').last}');

  final uri = req.requestedUri;
  final snapshotLength = uri.queryParameters['length'] == null
      ? 720
      : int.parse(uri.queryParameters['length']!);

  if (!file.existsSync()) {
    return Response.notFound(null);
  }

  final body = file.readAsStringSync();
  final List<List<dynamic>> logTable =
      const CsvToListConverter(fieldDelimiter: '\t', eol: '\n').convert(body);
  logTable.removeWhere((row) => row[2] != 'I');

  if (logTable.isEmpty || logTable.length < snapshotLength) {
    return Response.ok(body);
  } else {
    logTable.removeRange(0, logTable.length - snapshotLength);
    final shortBody = const ListToCsvConverter(fieldDelimiter: '\t', eol: '\n')
        .convert(logTable);
    return Response.ok(shortBody);
  }
}

Future<Response> _post(Request req, String path) async {
  // validate received data
  final mimeType = req.mimeType;
  if (mimeType != 'text/plain') {
    return Response.badRequest(body: 'Content-Type must be text/plain!');
  }
  if (req.isEmpty) {
    return Response.badRequest(body: 'Body must not be empty!');
  }

  // get body
  final length = req.contentLength;
  final body = await req.readAsString();
  if (length != body.length) {
    return Response.badRequest(
      body: 'Content-Length of $length '
          'did not match body.length of ${body.length}!',
    );
  }

  // // get remote address
  // var connectionInfo =
  //     req.context['shelf.io.connection_info'] as HttpConnectionInfo;
  // var remoteAddress = connectionInfo.remoteAddress.address;
  // print('remoteAddress = "$remoteAddress" (${remoteAddress.runtimeType})');

  final file = File('$rootDir/$path');
  file.createSync(exclusive: false);
  file.writeAsStringSync(
    body,
    mode: FileMode.writeOnlyAppend,
  );
  return Response.ok(null);
}

/*
 * read the root directory and create an index.html file
 */
Future<void> _createIndex() async {
  final sink = File('$rootDir/index.html').openWrite();
  sink.write('<html><body><ul>');
  await for (final file in Directory(rootDir).list()) {
    if (file is File) {
      final path = file.path.substring(rootDir.length + 1);
      sink.write('<li><a href="/logs/$path">/logs/$path</a></li>');
    }
  }
  sink.write('</ul></body></html>');
  // close the file
  await sink.close();
}

void main(List<String> args) async {
  // assign rootDir from args
  if (args.isNotEmpty) {
    rootDir = args[0];
  }
  await Directory(rootDir).create(recursive: true);
  await _createIndex();
  Timer.periodic(Duration(hours: 1), (timer) async {
    await _createIndex();
  });

  // Use any available host or container IP (usually `0.0.0.0`).
  final ip = InternetAddress.anyIPv4;

  // Configure a pipeline that logs requests.
  final handler =
      Pipeline().addMiddleware(logRequests()).addHandler(_router.call);

  // For running in containers, we respect the PORT environment variable.
  final port = int.parse(Platform.environment['PORT'] ?? '8082');
  final server = await serve(handler, ip, port);
  print('Server listening on port ${server.port}');
}
