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

  test('Get snapshot from /api/snapshotTest.log', () async {
    final response = await get(Uri.parse('$host/api/snapshotTest.log'));
    expect(response.statusCode, 200);

    expect(response.body.contains('W'), isFalse);
    expect(response.body.length, 22735); // 360 lines
  });

  test('Get snapshot using length parameter', () async {
    final response =
        await get(Uri.parse('$host/api/snapshotTest.log?length=10'));
    expect(response.statusCode, 200);

    expect(response.body.contains('W'), isFalse);
    expect(response.body.length, 630); // 10 lines
  });
}
