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
      final data = await client.fetchData('logs');
      expect(
        data,
        '''
<html>
<head><title>Index of /logs/</title></head>
<body>
<h1>Index of /logs/</h1><hr><pre><a href="../">../</a>
<a href="20230120.csv">20230120.csv</a>                                       11-Oct-2024 00:44             2247845
<a href="20230120.log">20230120.log</a>                                       11-Oct-2024 00:44               28072
<a href="20230121.log">20230121.log</a>                                       11-Oct-2024 00:44               72127
<a href="fostja-tank-1.log">fostja-tank-1.log</a>                                  25-Jan-2025 00:49               48059
<a href="james.txt">james.txt</a>                                          23-Jan-2025 23:58                 196
<a href="stefan-tank-1.log">stefan-tank-1.log</a>                                  26-Jan-2025 06:02                  32
<a href="stefan-tank-2.log">stefan-tank-2.log</a>                                  26-Jan-2025 06:02                  32
</pre><hr></body>
</html>''',
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

  group('getProjectList', () {
    final client = HttpClientTest();

    test('Returns valid projects for valid HTML data', () async {
      final projects = await client.getProjectList();

      // Check that the correct number of projects is returned
      expect(projects.length, equals(2));

      // Validate the parsed projects
      expect(projects[0].name, equals('fostja'));
      expect(projects[0].logs.length, equals(1));

      expect(projects[1].name, equals('stefan'));
      expect(projects[1].logs.length, equals(2));
    });

    test('Returns correct logs for projects', () async {
      final projects = await client.getProjectList();

      // Check that the correct number of projects is returned
      expect(projects.length, equals(2));

      // Validate the parsed projects
      expect(projects[1].name, equals('stefan'));
      expect(projects[1].logs.length, equals(2));

      expect(projects[1].logs[0].name, equals('tank-1'));
      expect(projects[1].logs[0].uri, equals('stefan-tank-1.log'));
      expect(projects[1].logs[1].name, equals('tank-2'));
      expect(projects[1].logs[1].uri, equals('stefan-tank-2.log'));
    });

    test('Returns empty list when no <li> elements are present', () async {
      client.testHTML = '<ul></ul>';
      final projects = await client.getProjectList();

      expect(projects, isEmpty);
    });

    test('Returns empty list when HTML is empty', () async {
      client.testHTML = '';
      final projects = await client.getProjectList();

      expect(projects, isEmpty);
    });

    test('Ignores <li> elements not ending in .log', () async {
      client.testHTML = '''
      <ul>
        <li><a href="project1-log1.txt">project1-log1.txt</a></li>
        <li>Invalid Item</li>
      </ul>
    ''';

      final projects = await client.getProjectList();

      expect(projects, isEmpty);
    });

    test('Ignores .log files that do not follow naming convention', () async {
      client.testHTML = '''
      <ul>
        <li><a href="project1log1.log">project1log1.log</a></li>
        <li><a href="project2-log2.log">project2-log2.log</a></li>
        <li><a href="project3_log3.log">project3_log3.log</a></li>
      </ul>
    ''';

      final projects = await client.getProjectList();

      expect(projects.length, equals(1));
      expect(projects[0].name, equals('project2'));
    });
  });

  group('parseLogListFromHTML', () {
    final client = HttpClientTest();

    test('parses HTML with valid list items', () async {
      final logList = client.parseLogListFromHTML(client.testHTML);

      // Check that the correct number of log items is returned
      expect(logList.length, equals(3));

      // Validate the parsed log entries
      expect(logList[0][0], equals('fostja-tank-1.log'));
      expect(logList[0][1], equals('fostja-tank-1.log'));

      expect(logList[1][0], equals('stefan-tank-1.log'));
      expect(logList[1][1], equals('stefan-tank-1.log'));

      expect(logList[2][0], equals('stefan-tank-2.log'));
      expect(logList[2][1], equals('stefan-tank-2.log'));
    });

    test('returns an empty list if no log links are present', () async {
      // Set up a different HTML in the client with no log links
      final testHTML =
          '<html><body><a href="/logs/test1">/logs/test1</a></body></html>';

      final logList = client.parseLogListFromHTML(testHTML);

      // Expect an empty list
      expect(logList, isEmpty);
    });

    test('handles malformed HTML without throwing an error', () async {
      // Set up malformed HTML
      final testHTML =
          '<html><body><a>/logs/test1.log</body></ul>';

      expect(
        () async => client.parseLogListFromHTML(testHTML),
        returnsNormally,
      );
    });
  });

  group('getTankSnapshot', () {
    final client = HttpClientTest();

    test('Returns valid TankSnapshot for a short log file', () async {
      final log = Log('sample_short', 'sample_short.log');
      final snapshot = await client.getTankSnapshot(log);

      expect(snapshot, isNotNull);
      expect(snapshot.log, equals(log));
      expect(snapshot.latestData.length, equals(5));
      expect(snapshot.pH, equals(6.35));
      expect(snapshot.temperature, equals(31.42));
    });

    test('Returns valid TankSnapshot for a long log file', () async {
      final log = Log('sample_long', 'sample_long.log');
      final snapshot = await client.getTankSnapshot(log);

      expect(snapshot, isNotNull);
      expect(snapshot.log, equals(log));
      expect(snapshot.latestData.length, equals(360));
      expect(snapshot.pH, equals(6.64));
      expect(snapshot.temperature, equals(24.91));
    });

    test('Handles empty log file', () async {
      final log = Log('empty', 'empty.log');
      final snapshot = await client.getTankSnapshot(log);

      expect(snapshot, isNotNull);
      expect(snapshot.latestData, isEmpty);
      expect(snapshot.pH, isNull);
      expect(snapshot.temperature, isNull);
    });

    test('Handles calibrating tank', () async {
      final log = Log('calibration', 'calibration.log');
      final snapshot = await client.getTankSnapshot(log);

      expect(snapshot, isNotNull);
      expect(snapshot.latestData.length, equals(1));
      expect(snapshot.pH, isNull);
      expect(snapshot.temperature, isNull);
    });

    test('Handles log file with warnings', () async {
      final log = Log('warnings', 'warnings.log');
      final snapshot = await client.getTankSnapshot(log);

      expect(snapshot, isNotNull);
      expect(snapshot.latestData.length, equals(4));
      expect(snapshot.pH, equals(6.34));
      expect(snapshot.temperature, equals(31.22));
    });
  });

  group('getLogData / parseLogData', () {
    final client = HttpClientTest();

    test('accurately parses a sample log file', () async {
      final logTable = await client.getLogData('sample_short.log');

      // Validate structure and date parsing for sample_short.log
      expect(logTable.length, equals(5));

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

      // Expect an empty list for an empty log file
      expect(logTable, isEmpty);
    });

    test('handles log file with calibration values', () async {
      final logTable = await client.getLogData('calibration.log');

      // temp and pH values should be null
      expect(logTable[0].tempMean, isNull);
      expect(logTable[0].tempStdDev, isNull);
      expect(logTable[0].phCurrent, isNull);
    });

    test('handles log file with warning logs', () async {
      final logTable = await client.getLogData('warnings.log');

      expect(logTable.length, equals(4));

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

  group('parseLogName', () {
    final client = HttpClientTest();

    test('Valid input with standard convention', () {
      expect(client.parseLogName('projectA-tank1.log'), 'tank1');
    });

    test('Valid input with multiple hyphens in tank name', () {
      expect(
        client.parseLogName('projectB-tank-name-with-hyphen.log'),
        'tank-name-with-hyphen',
      );
    });

    test('Input without a valid tank name', () {
      expect(client.parseLogName('projectD-.log'), '');
    });

    test('Input missing project name', () {
      expect(client.parseLogName('-tank1.log'), 'tank1');
    });

    test('Input without hyphen separation', () {
      expect(client.parseLogName('invalidlogfile.log'), '');
    });

    test('Empty string input', () {
      expect(client.parseLogName(''), '');
    });

    test('Input without .log extension', () {
      expect(client.parseLogName('projectE-tank2'), 'tank2');
    });
  });
}
