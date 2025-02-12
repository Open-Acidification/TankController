import 'dart:async' show Future;
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
  ..post('/api/<path>', _post);

// Get snapshots of log files
Future<Response> _get(Request req, String path) async {
  final file = File('$rootDir/${path.split('/').last}');

  final uri = req.requestedUri;
  final snapshotLength = uri.queryParameters['length'] == null
      ? 720
      : int.parse(uri.queryParameters['length']!);
  final snapshotGranularity = uri.queryParameters['granularity'] == null
      ? 5
      : int.parse(uri.queryParameters['granularity']!);

  if (!file.existsSync()) {
    return Response.notFound(null);
  }

  final body = file.readAsStringSync();

  // Remove non-informational lines
  List<List> logTable =
      const CsvToListConverter(fieldDelimiter: '\t', eol: '\n').convert(body);
  logTable.removeWhere((row) => row[2] != 'I');

  // Remove old lines
  if (!(logTable.isEmpty || logTable.length < snapshotLength)) {
    logTable.removeRange(0, logTable.length - snapshotLength);
  }

  // Condense to granularity and return
  logTable = condenseToGranularity(logTable, snapshotGranularity);
  final finalBody = const ListToCsvConverter(fieldDelimiter: '\t', eol: '\n')
      .convert(logTable);
  return Response.ok(finalBody);
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

  final file = File('$rootDir/$path');
  file.createSync(exclusive: false);
  file.writeAsStringSync(
    body,
    mode: FileMode.writeOnlyAppend,
  );
  return Response.ok(null);
}

List<List> condenseToGranularity(List<List> logTable, int snapshotGranularity) {
  if (snapshotGranularity > 1) {
    final List<List> condensedLogTable = [];
    for (var i = logTable.length; i > 0; i -= snapshotGranularity) {
      condensedLogTable.insert(0, logTable[i - 1]);
    }
    return condensedLogTable;
  } else {
    return logTable;
  }
}

void main(List<String> args) async {
  // assign rootDir from args
  if (args.isNotEmpty) {
    rootDir = args[0];
  }
  // Should not be needed in production, but it is useful for testing.
  await Directory(rootDir).create(recursive: true);

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
