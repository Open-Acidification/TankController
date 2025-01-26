import 'package:http/http.dart';
import 'package:test/test.dart';

void main() {
  final port = '8082';
  final host = 'http://localhost:$port';

  // Start the server in a different shell before running tests

  test('Get snapshot from /api/snapshotTest.log', () async {
    final response = await get(Uri.parse('$host/api/snapshotTest.log'));
    expect(response.statusCode, 200);
    // print(response.body);

    expect(response.body.length, 360);
    expect(response.body.contains('W'), isFalse);
  });
}
