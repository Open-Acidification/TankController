import 'package:http/http.dart';
import 'package:test/test.dart';

void main() {
  final port = '8080';
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

  test('Write to /logs/deleteMe.log', () async {
    final uri = Uri.parse('$host/logs/deleteMe.log');
    final line1 = 'This is data for line 1\n';
    final line2 = 'This is data for line 2\n';

    // delete the log file
    var response = await delete(uri);
    expect(response.statusCode, 200);
    // confirm that the log file is deleted
    response = await head(uri);
    expect(response.statusCode, 404);
    // append to the log file
    response = await post(uri, body: line1);
    expect(response.statusCode, 200);
    // confirm the file's new size
    response = await head(uri);
    expect(response.statusCode, 200);
    expect(response.headers['content-length'], line1.length.toString());
    // append another line to the log file
    response = await post(uri, body: line2);
    expect(response.statusCode, 200);
    // confirm the file's new size
    response = await head(uri);
    expect(response.statusCode, 200);
    expect(
      response.headers['content-length'],
      (line1.length + line2.length).toString(),
    );
    // read the file and confirm the contents
    response = await get(uri);
    expect(response.statusCode, 200);
    expect(response.body, '$line1$line2');
    expect(
      response.headers['content-length'],
      (line1.length + line2.length).toString(),
    );
  });
}
