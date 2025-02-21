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
Version	Tank ID	Severity	Date Time	Message	Temperature Target	Temperature Mean	Temperature Std Dev	pH Target	pH	Uptime	MAC Address	pH Slope	Ignoring Bad pH Calibration	Temperature Correction	Ignoring Bad Temperature Calibration	Heat (1) or Chill (0)	KD	KI	KP	pH Flat (0) Ramp (1) Sine (2)	pH Target	pH Ramp Start Time	pH Ramp End Time	pH Ramp Start Value	pH Sine Start Time	pH Sine Period	pH Sine Amplitude	Temperature Flat (0) Ramp (1) Sine (2)	Temperature Target	Temperature Ramp Start Time	Temperature Ramp End Time	Temperature Ramp Start Value	Temperature Sine Start Time	Temperature Sine Period	Temperature Sine Amplitude	Google Sheet Interval
v25.1.1        	89	I	2025-01-23 15:38		20.11	20	0	7	0	60																										
v25.1.1        	89	I	2025-01-23 15:39		20.18	21	0	7	0	120																										
v25.1.1        	89	I	2025-01-23 15:40		20.24	20	0	7	6.9	180																										
v25.1.1        	89	I	2025-01-23 15:43		20.38	20	0	7	0	60																										
v25.1.1        	89	I	2025-01-23 15:44		20.44	20	0	7	0	121																										
''',
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
      final testHTML = '<html><body><a>/logs/test1.log</body></ul>';

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
      expect(snapshot.pH, equals(0));
      expect(snapshot.temperature, equals(20));
      expect(snapshot.pHSetpoint, equals(7));
      expect(snapshot.temperatureSetpoint, equals(20.44));
      expect(snapshot.maxPH, equals(6.9));
      expect(snapshot.minPH, equals(0));
      expect(snapshot.maxTemp, equals(21));
      expect(snapshot.minTemp, equals(20));
    });

    test('Returns valid TankSnapshot for a long log file', () async {
      final log = Log('sample_long', 'sample_long.log');
      final snapshot = await client.getTankSnapshot(log);

      expect(snapshot, isNotNull);
      expect(snapshot.log, equals(log));
      expect(snapshot.latestData.length, equals(360));
      expect(snapshot.pH, equals(7));
      expect(snapshot.temperature, equals(21));
      expect(snapshot.pHSetpoint, equals(7.5));
      expect(snapshot.temperatureSetpoint, equals(16.48));
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
            'v25.1.1',
            89,
            DateTime.parse('2025-01-23 15:38:00'),
            20.11,
            20.0,
            0.0,
            7.0,
            0.0,
            60,
          ),
          LogDataLine(
            'v25.1.1',
            89,
            DateTime.parse('2025-01-23 15:39:00'),
            20.18,
            21.0,
            0.0,
            7.0,
            0.0,
            120,
          ),
          LogDataLine(
            'v25.1.1',
            89,
            DateTime.parse('2025-01-23 15:40:00'),
            20.24,
            20.0,
            0.0,
            7.0,
            6.9,
            180,
          ),
          LogDataLine(
            'v25.1.1',
            89,
            DateTime.parse('2025-01-23 15:43:00'),
            20.38,
            20.0,
            0.0,
            7.0,
            0.0,
            60,
          ),
          LogDataLine(
            'v25.1.1',
            89,
            DateTime.parse('2025-01-23 15:44:00'),
            20.44,
            20.0,
            0.0,
            7.0,
            0.0,
            121,
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
            'v1.0',
            80,
            DateTime.parse('2025-01-07 11:20:00'),
            31.25,
            30.81,
            0.22,
            6.38,
            6.3,
            1080,
          ),
          LogDataLine(
            'v1.0',
            80,
            DateTime.parse('2025-01-07 11:21:00'),
            31.25,
            30.99,
            0.13,
            6.38,
            6.38,
            1140,
          ),
          LogDataLine(
            'v1.0',
            80,
            DateTime.parse('2025-01-07 11:22:00'),
            31.25,
            31.38,
            0.065,
            6.38,
            6.39,
            1200,
          ),
          LogDataLine(
            'v1.0',
            80,
            DateTime.parse('2025-01-07 11:23:00'),
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
