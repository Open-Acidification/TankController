import 'package:log_file_client/utils/http_client.dart';
import 'package:test/test.dart';

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
        '<html><body><ul><li><a href="/test1.log">/logs/test1.log</a></li><li><a href="/test2.log"">/logs/test2.log</a></li><li><a href="/test3.log">/logs/test3.log</a></li></ul></body></html>',
      );
    });

    test('sample log data', () async {
      final data = await client.fetchData('sample_short.log');
      expect(
        data,
        '''
1.0	80	I	2025-01-07 11:02:30		31.25	31.11	0.07	6.38	6.41	0
1.0	80	I	2025-01-07 11:03:30		31.25	31.25	0.0	6.38	6.38	60
1.0	80	I	2025-01-07 11:04:30		31.25	31.43	0.09	6.38	6.36	120
1.0	80	I	2025-01-07 11:05:30		31.25	31.54	0.145	6.38	6.46	180
1.0	80	I	2025-01-07 11:06:30		31.25	31.42	0.085	6.38	6.35	240''',
      );
    });
  });

  group('getLogList', () {
    final client = HttpClientTest();

    test('parses HTML with valid list items', () async {
      final logList = await client.getLogList();

      // Check that the correct number of log items is returned
      expect(logList.length, equals(3));

      // Validate the parsed log entries
      expect(logList[0].name, equals('test1.log'));
      expect(logList[0].uri, equals('/test1.log'));

      expect(logList[1].name, equals('test2.log'));
      expect(logList[1].uri, equals('/test2.log'));

      expect(logList[2].name, equals('test3.log'));
      expect(logList[2].uri, equals('/test3.log'));
    });

    test('returns an empty list if no log links are present', () async {
      // Set up a different HTML in the client with no log links
      client.testHTML =
          '<html><body><ul><li><a href="/logs/test1">/logs/test1</a></li></ul></body></html>';

      final logList = await client.getLogList();

      // Expect an empty list
      expect(logList, isEmpty);
    });

    test('handles malformed HTML without throwing an error', () async {
      // Set up malformed HTML
      client.testHTML =
          '<html><body><ul><li><a>/logs/test1.log</a></li></body></ul>';

      expect(() async => client.getLogList(), returnsNormally);
    });
  });

  group('getLogData', () {
    final client = HttpClientTest();

    test('accurately parses a sample log file', () async {
      final logTable = await client.getLogData('sample_short.log');

      // Validate structure and date parsing for sample_short.log
      expect(logTable!.length, equals(5));

      expect(
        logTable,
        [
          LogDataLine(
            1.0,
            80,
            DateTime.parse('2025-01-07 11:02:30'),
            31.25,
            31.11,
            0.07,
            6.38,
            6.41,
            0,
          ),
          LogDataLine(
            1.0,
            80,
            DateTime.parse('2025-01-07 11:03:30'),
            31.25,
            31.25,
            0.0,
            6.38,
            6.38,
            60,
          ),
          LogDataLine(
            1.0,
            80,
            DateTime.parse('2025-01-07 11:04:30'),
            31.25,
            31.43,
            0.09,
            6.38,
            6.36,
            120,
          ),
          LogDataLine(
            1.0,
            80,
            DateTime.parse('2025-01-07 11:05:30'),
            31.25,
            31.54,
            0.145,
            6.38,
            6.46,
            180,
          ),
          LogDataLine(
            1.0,
            80,
            DateTime.parse('2025-01-07 11:06:30'),
            31.25,
            31.42,
            0.085,
            6.38,
            6.35,
            240,
          ),
        ],
      );
    });

    test('handles empty log file without errors', () async {
      final logTable = await client.getLogData('empty.log');

      // Expect a null value for an empty log file
      expect(logTable, isNull);
    });

    test('handles log file with calibration values', () async {
      final logTable = await client.getLogData('calibration.log');

      // temp and pH values should be null
      expect(logTable![0].tempMean, isNull);
      expect(logTable[0].tempStdDev, isNull);
      expect(logTable[0].phCurrent, isNull);
    });

    test('handles log file with warning logs', () async {
      final logTable = await client.getLogData('warnings.log');

      expect(logTable!.length, equals(4));

      expect(
        logTable,
        [
          LogDataLine(
            1.0,
            80,
            DateTime.parse('2025-01-07 11:20:30'),
            31.25,
            30.81,
            0.22,
            6.38,
            6.3,
            1080,
          ),
          LogDataLine(
            1.0,
            80,
            DateTime.parse('2025-01-07 11:21:30'),
            31.25,
            30.99,
            0.13,
            6.38,
            6.38,
            1140,
          ),
          LogDataLine(
            1.0,
            80,
            DateTime.parse('2025-01-07 11:22:30'),
            31.25,
            31.38,
            0.065,
            6.38,
            6.39,
            1200,
          ),
          LogDataLine(
            1.0,
            80,
            DateTime.parse('2025-01-07 11:23:30'),
            31.25,
            31.22,
            0.015,
            6.38,
            6.34,
            1260,
          ),
        ],
      );
    });
  });
}
