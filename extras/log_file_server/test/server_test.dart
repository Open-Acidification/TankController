import 'package:http/http.dart';
import 'package:test/test.dart';

void main() {
  final port = '8082';
  final host = 'http://localhost:$port';

  // Start the server in a different shell before running tests

  test('HEAD for /logs/missing.log', () async {
    final response = await head(Uri.parse('$host/logs/missing.log'));
    expect(response.statusCode, 404);
  });

  test('HEAD for /logs/empty.log', () async {
    final response = await head(Uri.parse('$host/logs/empty.log'));
    expect(response.statusCode, 200);
    expect(response.headers['content-length'], '0');
  });

  test('HEAD for /logs/ten.log', () async {
    final response = await head(Uri.parse('$host/logs/ten.log'));
    expect(response.statusCode, 200);
    expect(response.headers['content-length'], '10');
  });

  test('HEAD for /logs/../foo.log', () async {
    final response = await head(Uri.parse('$host/logs/../foo.log'));
    expect(response.statusCode, 404);
  });

  test('Get snapshot from /logs/snapshot/snapshotTest.log', () async {
    final response =
        await get(Uri.parse('$host/logs/snapshot/snapshotTest.log'));
    expect(response.statusCode, 200);
    // print(response.body);

    expect(response.body.length, 360);
    expect(response.body.contains('W'), isFalse);
  });
}
