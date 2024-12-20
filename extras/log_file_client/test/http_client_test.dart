import 'package:log_file_client/utils/http_client.dart';
import 'package:test/test.dart';

//  how to run the test
//  flutter test test/http_client_test.dart

void main() {
  test('Log constructor', () {
    final Log newLog = Log('Test1', 'Test1.test');

    expect(newLog.name, equals('Test1'));
    expect(newLog.uri, equals('Test1.test'));
  });

  group('fetchData in test class', () {
    final client = HttpClientTest();

    test('sample HTML data', () async {
      final data = await client.fetchData('logs/index.html');
      expect(
        data,
        '<html><body><ul><li><a href="/test1.csv">/logs/test1.csv</a></li><li><a href="/test2.csv"">/logs/test2.csv</a></li><li><a href="/test3.csv">/logs/test3.csv</a></li></ul></body></html>',
      );
    });

    test('sample CSV data', () async {
      final data = await client.fetchData('sample_short.csv');
      expect(
        data,
        '''
time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd
01/20/2023 16:18:21,  99, 0.00, 10.00, 0.000, 8.645,    6,    700.0,    100.0,      0.0
01/20/2023 16:18:22,  99, 1.23, 10.00, 7.123, 8.645,    8,    710.0,    110.0,      1.0
01/20/2023 16:18:23,  99, 2.34, 10.00, 6.789, 8.645,    9,    720.0,    120.0,      2.0
01/20/2023 16:18:24,  99, 3.45, 10.00, 5.456, 8.645,   10,    730.0,    130.0,      3.0
01/20/2023 16:18:25,  99, 4.56, 10.00, 4.123, 8.645,   11,    740.0,    140.0,      4.0''',
      );
    });
  });

  group('formatDateString', () {
    final client = HttpClientTest();

    test('formats date strings correctly', () {
      final data1 = client.formatDateString('01/20/2023 16:18:21');
      expect(data1, equals('2023-01-20 16:18:21'));

      final data2 = client.formatDateString('02/15/2021 14:22:45');
      expect(data2, equals('2021-02-15 14:22:45'));

      final data3 = client.formatDateString('03/10/2022 09:30:12');
      expect(data3, equals('2022-03-10 09:30:12'));

      final data4 = client.formatDateString('04/05/2020 18:45:33');
      expect(data4, equals('2020-04-05 18:45:33'));
    });

    test('returns "InvalidDate" for invalid date strings', () {
      final data1 = client.formatDateString('11-11-2019 18:45:33');
      expect(data1, equals('InvalidDate'));

      final data2 = client.formatDateString('invalid date string');
      expect(data2, equals('InvalidDate'));
    });
  });

  group('getLogList', () {
    final client = HttpClientTest();

    test('parses HTML with valid list items', () async {
      final logList = await client.getLogList();

      // Check that the correct number of log items is returned
      expect(logList.length, equals(3));

      // Validate the parsed log entries
      expect(logList[0].name, equals('test1.csv'));
      expect(logList[0].uri, equals('/test1.csv'));

      expect(logList[1].name, equals('test2.csv'));
      expect(logList[1].uri, equals('/test2.csv'));

      expect(logList[2].name, equals('test3.csv'));
      expect(logList[2].uri, equals('/test3.csv'));
    });

    test('returns an empty list if no CSV links are present', () async {
      // Set up a different HTML in the client with no CSV links
      client.testHTML =
          '<html><body><ul><li><a href="/logs/test1">/logs/test1</a></li></ul></body></html>';

      final logList = await client.getLogList();

      // Expect an empty list
      expect(logList, isEmpty);
    });

    test('handles malformed HTML without throwing an error', () async {
      // Set up malformed HTML
      client.testHTML =
          '<html><body><ul><li><a>/logs/test1.csv</a></li></body></ul>';

      expect(() async => client.getLogList(), returnsNormally);
    });
  });

  group('getLogData', () {
    final client = HttpClientTest();

    test('accurately parses a sample CSV file', () async {
      final logTable = await client.getLogData('sample_short.csv');

      // Validate structure and date parsing for sample_short.csv
      expect(logTable!.length, equals(5));

      expect(
        logTable,
        [
          LogDataLine(
            DateTime.parse('2023-01-20 16:18:21'),
            99,
            0.00,
            10.00,
            0.000,
            8.645,
            6,
            700.0,
            100.0,
            0.0,
          ),
          LogDataLine(
            DateTime.parse('2023-01-20 16:18:22'),
            99,
            1.23,
            10.00,
            7.123,
            8.645,
            8,
            710.0,
            110.0,
            1.0,
          ),
          LogDataLine(
            DateTime.parse('2023-01-20 16:18:23'),
            99,
            2.34,
            10.00,
            6.789,
            8.645,
            9,
            720.0,
            120.0,
            2.0,
          ),
          LogDataLine(
            DateTime.parse('2023-01-20 16:18:24'),
            99,
            3.45,
            10.00,
            5.456,
            8.645,
            10,
            730.0,
            130.0,
            3.0,
          ),
          LogDataLine(
            DateTime.parse('2023-01-20 16:18:25'),
            99,
            4.56,
            10.00,
            4.123,
            8.645,
            11,
            740.0,
            140.0,
            4.0,
          ),
        ],
      );
    });

    test('handles empty CSV file without errors', () async {
      final logTable = await client.getLogData('empty.csv');

      // Expect a null value for an empty CSV file
      expect(logTable, isNull);
    });
  
    test('handles CSV with invalid values', () async {
      final logTable = await client.getLogData('invalid_values.csv');

      // temp and pH values should be null
      expect(logTable![0].temp, isNull);
      expect(logTable[0].pH, isNull);
    });

  });

}
