import 'package:csv/csv.dart';
import 'package:http/http.dart';
import 'package:test/test.dart';

import '../bin/log_file_server.dart';

void main() {
  final port = '8082';
  final host = 'http://localhost:$port';

  // Start the server in a different shell before running tests
  test('Get snapshot from /api/snapshotTest.log', () async {
    final response = await get(
      Uri.parse('$host/api/snapshotTest.log?now=2025-01-09T16:05:00'),
    );
    expect(response.statusCode, 200);

    final logTable = const CsvToListConverter(fieldDelimiter: '\t', eol: '\n')
        .convert(response.body);
    expect(logTable.length, 144); // 144 lines (12 hours by 5 mins default)

    expect(response.body.contains('W'), isFalse);
  });

  test('Get snapshot using parameters', () async {
    final response = await get(
      Uri.parse(
        '$host/api/snapshotTest.log?now=2025-01-09T16:05:00&length=10&granularity=1',
      ),
    );
    expect(response.statusCode, 200);

    expect(response.body.contains('W'), isFalse);

    final logTable = const CsvToListConverter(fieldDelimiter: '\t', eol: '\n')
        .convert(response.body);
    expect(logTable.length, 10); // 10 lines
  });

  test('trimToTimeRange trims normal list', () async {
    final testList = [
      ['v0', 0, 'I', '2023-10-01 00:00:00'],
      ['v0', 0, 'I', '2023-10-01 00:01:00'],
      ['v0', 0, 'I', '2023-10-01 00:02:00'],
      ['v0', 0, 'I', '2023-10-01 00:03:00'],
      ['v0', 0, 'I', '2023-10-01 00:04:00'],
      ['v0', 0, 'I', '2023-10-01 00:05:00'],
      ['v0', 0, 'I', '2023-10-01 00:06:00'],
      ['v0', 0, 'I', '2023-10-01 00:07:00'],
      ['v0', 0, 'I', '2023-10-01 00:08:00'],
      ['v0', 0, 'I', '2023-10-01 00:09:00'],
    ];

    final newList =
        trimToTimeRange(testList, 5, DateTime.parse('2023-10-01 00:10:00'));

    expect(newList.length, 5);
    expect(newList[0][3], '2023-10-01 00:05:00');
    expect(newList[4][3], '2023-10-01 00:09:00');
  });

  test('trimToTimeRange trims empty list', () async {
    final testList = [[]];

    final newList =
        trimToTimeRange(testList, 5, DateTime.parse('2023-10-01 00:10:00'));

    expect(newList.length, 0);
  });

  test('trimToTimeRange trims list shorter than target range', () async {
    final testList = [
      ['v0', 0, 'I', '2023-10-01 00:07:00'],
      ['v0', 0, 'I', '2023-10-01 00:08:00'],
      ['v0', 0, 'I', '2023-10-01 00:09:00'],
    ];

    final newList =
        trimToTimeRange(testList, 5, DateTime.parse('2023-10-01 00:10:00'));

    expect(newList.length, 3);
    expect(newList[0][3], '2023-10-01 00:07:00');
    expect(newList[2][3], '2023-10-01 00:09:00');
  });

  test('trimToTimeRange trims list with missing minutes', () async {
    final testList = [
      ['v0', 0, 'I', '2023-10-01 00:00:00'],
      ['v0', 0, 'I', '2023-10-01 00:01:00'],
      ['v0', 0, 'I', '2023-10-01 00:02:00'],
      ['v0', 0, 'I', '2023-10-01 00:03:00'],
      ['v0', 0, 'I', '2023-10-01 00:04:00'],
      ['v0', 0, 'I', '2023-10-01 00:05:00'],
      ['v0', 0, 'I', '2023-10-01 00:06:00'],
      ['v0', 0, 'I', '2023-10-01 00:09:00'],
    ];

    final newList =
        trimToTimeRange(testList, 5, DateTime.parse('2023-10-01 00:10:00'));

    expect(newList.length, 3);
    expect(newList[0][3], '2023-10-01 00:05:00');
    expect(newList[2][3], '2023-10-01 00:09:00');
  });

  test('trimToTimeRange returns empty list when last data is too old',
      () async {
    final testList = [
      ['v0', 0, 'I', '2023-10-01 00:00:00'],
      ['v0', 0, 'I', '2023-10-01 00:01:00'],
      ['v0', 0, 'I', '2023-10-01 00:02:00'],
      ['v0', 0, 'I', '2023-10-01 00:03:00'],
      ['v0', 0, 'I', '2023-10-01 00:04:00'],
      ['v0', 0, 'I', '2023-10-01 00:05:00'],
      ['v0', 0, 'I', '2023-10-01 00:06:00'],
      ['v0', 0, 'I', '2023-10-01 00:09:00'],
    ];

    final newList =
        trimToTimeRange(testList, 5, DateTime.parse('2023-10-01 00:20:00'));

    expect(newList, equals([]));
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
