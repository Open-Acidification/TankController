import 'dart:async';

import 'package:csv/csv.dart';
import 'package:html/dom.dart';
import 'package:html/parser.dart';
import 'package:http/http.dart' as http;
import 'package:intl/intl.dart';
import 'package:log_file_client/utils/sample_data.dart';

class Project {
  Project(this.name, this.logs);

  final String name;
  final List<Log> logs;
}

class Log {
  Log(this.name, this.uri);
  final String name;
  final String uri;
}

class LogDataLine {
  LogDataLine(
    this.version,
    this.tankid,
    this.time,
    this.tempTarget,
    this.tempMean,
    this.tempStdDev,
    this.phTarget,
    this.phCurrent,
    this.onTime,
  );
  final String version;
  final int tankid;
  final DateTime time;
  final double? tempTarget;
  final double? tempMean;
  final double? tempStdDev;
  final double? phTarget;
  final double? phCurrent;
  final int? onTime;

  @override
  String toString() {
    return 'LogDataLine(version: $version, tankid: $tankid, time: $time, tempTarget: $tempTarget, tempMean: $tempMean, tempStdDev: $tempStdDev, phTarget: $phTarget, phCurrent: $phCurrent, onTime: $onTime)';
  }

  @override
  bool operator ==(Object other) {
    if (identical(this, other)) {
      return true;
    }
    if (other is! LogDataLine) {
      return false;
    }
    return other.version == version &&
        other.tankid == tankid &&
        other.time == time &&
        other.tempTarget == tempTarget &&
        other.tempMean == tempMean &&
        other.tempStdDev == tempStdDev &&
        other.phTarget == phTarget &&
        other.phCurrent == phCurrent &&
        other.onTime == onTime;
  }

  @override
  int get hashCode => Object.hash(
        version,
        tankid,
        time,
        tempTarget,
        tempMean,
        tempStdDev,
        phTarget,
        phCurrent,
        onTime,
      );
}

class TankSnapshot {
  TankSnapshot(
    this.log,
    this.latestData,
    this.pH,
    this.temperature,
    this.pHSetpoint,
    this.temperatureSetpoint,
    this.maxPH,
    this.minPH,
    this.maxTemp,
    this.minTemp,
  );
  final Log log;
  final List<LogDataLine?> latestData;
  final double? pH;
  final double? temperature;
  final double? pHSetpoint;
  final double? temperatureSetpoint;
  final double? maxPH;
  final double? minPH;
  final double? maxTemp;
  final double? minTemp;
}

abstract class HttpClient {
  HttpClient();

  Future<String> fetchData(String filePath);

  Future<List<Project>> getProjectList() async {
    // Fetch data from server
    final data = await fetchData('logs');

    // Get list items from HTML
    final listItems = parseLogListFromHTML(data);

    // Parse projects from logs
    final Map<String, List<Log>> projects = {};
    for (int i = 0; i < listItems.length; i++) {
      // Skip items that don't follow the naming convention
      if (listItems[i][0].split('-').length < 2) {
        continue;
      }
      final projectName = listItems[i][0].split('-')[0];
      if (projects[projectName] != null) {
        projects[projectName]!
            .add(Log(parseLogName(listItems[i][0]), listItems[i][1]));
      } else {
        projects[projectName] = [
          Log(parseLogName(listItems[i][0]), listItems[i][1]),
        ];
      }
    }

    // Return list items as a list of projects
    return projects.entries.map((e) => Project(e.key, e.value)).toList();
  }

  Future<TankSnapshot> getTankSnapshot(Log log) async {
    final data = await fetchData('api/${log.uri}');

    final loglines = parseLogData(data);

    if (loglines.isEmpty) {
      return TankSnapshot(
        log,
        [],
        null,
        null,
        null,
        null,
        null,
        null,
        null,
        null,
      );
    } else {
      return TankSnapshot(
        log,
        loglines,
        loglines[loglines.length - 1].phCurrent,
        loglines[loglines.length - 1].tempMean,
        loglines[loglines.length - 1].phTarget,
        loglines[loglines.length - 1].tempTarget,
        loglines.map((e) => e.phCurrent).reduce((a, b) => a! > b! ? a : b),
        loglines.map((e) => e.phCurrent).reduce((a, b) => a! < b! ? a : b),
        loglines.map((e) => e.tempMean).reduce((a, b) => a! > b! ? a : b),
        loglines.map((e) => e.tempMean).reduce((a, b) => a! < b! ? a : b),
      );
    }
  }

  Future<List<LogDataLine>> getLogData(String filePath) async {
    final data = await fetchData(filePath);

    return parseLogData(data);
  }

  List<LogDataLine> parseLogData(String data) {
    if (data.trim().isEmpty) {
      return [];
    }

    final List<List<dynamic>> logTable =
        const CsvToListConverter(fieldDelimiter: '\t', eol: '\n').convert(data);

    // Filter out header and empty rows
    logTable.removeWhere((row) => row.isEmpty);
    logTable.removeWhere((row) => row[0].substring(0, 1) != 'v');

    // Parse the date strings
    for (int i = 0; i < logTable.length; i++) {
      final DateFormat format = DateFormat('yyyy-MM-dd H:mm');
      final parsedDate = format.tryParse(logTable[i][3].trim());

      if (parsedDate == null) {
        throw FormatException(
          'Invalid date format in row ${i + 1}: ${logTable[i][3].toString()}',
        );
      }

      logTable[i][3] = parsedDate;
    }

    // Convert the list of lists to a list of LogDataLine objects
    final List<LogDataLine> logData = logTable
        .where((row) => row[2] == 'I') // Filter out non-data rows
        .map(
          (e) => LogDataLine(
            e[0].trim(),
            e[1].toInt(),
            e[3],
            e[5] is double ? e[5] : (e[5] is int ? e[5].toDouble() : null),
            e[6] is double ? e[6] : (e[6] is int ? e[6].toDouble() : null),
            e[7] is double ? e[7] : (e[7] is int ? e[7].toDouble() : null),
            e[8] is double ? e[8] : (e[8] is int ? e[8].toDouble() : null),
            e[9] is double ? e[9] : (e[9] is int ? e[9].toDouble() : null),
            e[10] is int ? e[10] : (e[10] is double ? e[10].toInt() : null),
          ),
        )
        .toList();

    return logData;
  }

  List<List<String>> parseLogListFromHTML(String data) {
    final result = <List<String>>[];
    for (final Element e in parse(data).getElementsByTagName('a')) {
      if (e.attributes.containsKey('href')) {
        final name = e.innerHtml;
        if (e.attributes['href']!.endsWith('.log') && name.contains('-')) {
          result.add([
            name,
            e.attributes['href']!,
          ]);
        }
      }
    }
    return result;
  }

  String parseLogName(String name) {
    return name.split('-').sublist(1).join('-').split('.').first;
  }
}

class HttpClientProd extends HttpClient {
  // Fetches data from the server.
  HttpClientProd();

  @override
  Future<String> fetchData(String filePath) async {
    Uri url;
    if (Uri.base.toString() == 'https://oap.cs.wallawalla.edu/') {
      url = Uri.https('oap.cs.wallawalla.edu/', filePath);
    } else {
      url = Uri.http('localhost:8080', filePath);
    }

    final http.Response response = await http.get(url);

    if (response.statusCode != 200) {
      throw response.reasonPhrase!;
    }
    return response.body;
  }
}

class HttpClientTest extends HttpClient {
  // Fetches data from a hard-coded string (short) or the local Dart file (1440 sample lines) for testing purposes
  HttpClientTest();

  late String testHTML = '''
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
</html>''';

  @override
  Future<String> fetchData(String filePath) async {
    if (filePath == 'logs' || filePath == 'logs/index.html') {
      return testHTML;
    } else if (filePath == 'sample_short.log' ||
        filePath == 'api/sample_short.log' ||
        filePath == '/logs/sample_short.log') {
      return '''
Version	Tank ID	Severity	Date Time	Message	Temperature Target	Temperature Mean	Temperature Std Dev	pH Target	pH	Uptime	MAC Address	pH Slope	Ignoring Bad pH Calibration	Temperature Correction	Ignoring Bad Temperature Calibration	Heat (1) or Chill (0)	KD	KI	KP	pH Flat (0) Ramp (1) Sine (2)	pH Target	pH Ramp Start Time	pH Ramp End Time	pH Ramp Start Value	pH Sine Start Time	pH Sine Period	pH Sine Amplitude	Temperature Flat (0) Ramp (1) Sine (2)	Temperature Target	Temperature Ramp Start Time	Temperature Ramp End Time	Temperature Ramp Start Value	Temperature Sine Start Time	Temperature Sine Period	Temperature Sine Amplitude	Google Sheet Interval
v25.1.1        	89	I	2025-01-23 15:38		20.11	20	0	7	0	60																										
v25.1.1        	89	I	2025-01-23 15:39		20.18	21	0	7	0	120																										
v25.1.1        	89	I	2025-01-23 15:40		20.24	20	0	7	6.9	180																										
v25.1.1        	89	I	2025-01-23 15:43		20.38	20	0	7	0	60																										
v25.1.1        	89	I	2025-01-23 15:44		20.44	20	0	7	0	121																										
''';
    } else if (filePath == 'sample_long.log' ||
        filePath == '/logs/sample_long.log') {
      return sampleData();
    } else if (filePath == 'api/sample_long.log') {
      return sampleSnapshotData();
    } else if (filePath == 'calibration.log' ||
        filePath == 'api/calibration.log') {
      return '''
v1.0	80	I	2025-01-07 11:09:30		31.25	C	C	6.38	C	420''';
    } else if (filePath == 'warnings.log' || filePath == 'api/warnings.log') {
      return '''
v1.0	80	I	2025-01-07 11:20:30		31.25	30.81	0.22	6.38	6.3	1080
v1.0	80	I	2025-01-07 11:21:30		31.25	30.99	0.13	6.38	6.38	1140
v1.0	80	W	2025-01-07 11:21							202	90:A2:DA:FD:C2:38	Requesting...	0	1.82	1	0	6	101	90	1	7.5	12761	30761	0	12761	0	0	2	18	6100	0	0	6100	32400	6	600
v1.0	80	I	2025-01-07 11:22:30		31.25	31.38	0.065	6.38	6.39	1200
v1.0	80	I	2025-01-07 11:23:30		31.25	31.22	0.015	6.38	6.34	1260
''';
    } else if (filePath == 'empty.log' || filePath == 'api/empty.log') {
      return '';
    } else if (filePath == 'api/ProjectA-tank-24.log' ||
        filePath == 'api/fostja-tank-1.log') {
      return '''
v1.0	24	I	2025-01-09 16:09:16		21.45	21.91	0.23	6.25	6.24	86220
v1.0	24	I	2025-01-09 16:10:16		21.45	21.34	0.055	6.25	6.18	86280
v1.0	24	I	2025-01-09 16:11:16		21.45	20.98	0.235	6.25	6.33	86340
''';
    } else if (filePath == 'api/ProjectA-tank-70.log') {
      return '''
v1.0	70	I	2025-01-09 16:04:29		23.29	23.27	0.01	7.22	7.34	86220
v1.0	70	I	2025-01-09 16:05:29		23.29	23.26	0.015	7.22	7.1	86280
v1.0	70	I	2025-01-09 16:06:29		23.29	23.67	0.19	7.22	7.29	86340
''';
    } else {
      throw Exception('Failed to fetch data from $filePath');
    }
  }
}
