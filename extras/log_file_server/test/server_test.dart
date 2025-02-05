import 'package:http/http.dart';
import 'package:test/test.dart';

void main() {
  final port = '8082';
  final host = 'http://localhost:$port';

  // Start the server in a different shell before running tests
  test('Get snapshot from /api/snapshotTest.log', () async {
    final response = await get(Uri.parse('$host/api/snapshotTest.log'));
    expect(response.statusCode, 200);

    expect(response.body.length, 46194); // 720 lines (12 hours default)
    expect(response.body.contains('W'), isFalse);
  });

  test('Get snapshot using length parameter', () async {
    final response =
        await get(Uri.parse('$host/api/snapshotTest.log?length=10'));
    expect(response.statusCode, 200);

    expect(response.body.contains('W'), isFalse);
    expect(response.body.length, 630); // 10 lines
  });
}
