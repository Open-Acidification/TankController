import 'package:csv/csv.dart';
import 'package:http/http.dart';
import 'package:test/test.dart';

import '../bin/log_file_server.dart';

void main() {
  final port = '8082';
  final host = 'http://localhost:$port';

  // Start the server in a different shell before running tests
  test('Get snapshot from /api/snapshotTest.log', () async {
    final response = await get(Uri.parse('$host/api/snapshotTest.log'));
    expect(response.statusCode, 200);

    final logTable = const CsvToListConverter(fieldDelimiter: '\t', eol: '\n')
        .convert(response.body);
    expect(logTable.length, 144); // 144 lines (12 hours by 5 mins default)

    expect(response.body.contains('W'), isFalse);
  });

  test('Get snapshot using parameters', () async {
    final response = await get(
        Uri.parse('$host/api/snapshotTest.log?length=10&granularity=1'));
    expect(response.statusCode, 200);

    expect(response.body.contains('W'), isFalse);

    final logTable = const CsvToListConverter(fieldDelimiter: '\t', eol: '\n')
        .convert(response.body);
    expect(logTable.length, 10); // 10 lines
  });

  test('condenseToGranularity', () async {
    final testList = [
      ['2023-10-01 00:00:00', 'I', 'test'],
      ['2023-10-01 00:00:01', 'I', 'test'],
      ['2023-10-01 00:00:02', 'I', 'test'],
      ['2023-10-01 00:00:03', 'I', 'test'],
      ['2023-10-01 00:00:04', 'I', 'test'],
      ['2023-10-01 00:00:05', 'I', 'test'],
      ['2023-10-01 00:00:06', 'I', 'test'],
      ['2023-10-01 00:00:07', 'I', 'test'],
      ['2023-10-01 00:00:08', 'I', 'test'],
      ['2023-10-01 00:00:09', 'I', 'test'],
    ];

    final newList = condenseToGranularity(testList, 5);
    expect(newList.length, 2);
    expect(newList[0][0], '2023-10-01 00:00:04');
    expect(newList[1][0], '2023-10-01 00:00:09');
  });
}
