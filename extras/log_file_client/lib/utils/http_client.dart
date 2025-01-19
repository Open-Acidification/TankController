import 'dart:async';

import 'package:csv/csv.dart';
import 'package:html/dom.dart';
import 'package:html/parser.dart';
import 'package:http/http.dart' as http;
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
  final double version;
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
  TankSnapshot(this.log, this.latestData, this.pH, this.temperature);
  final Log log;
  final List<LogDataLine?> latestData;
  final double? pH;
  final double? temperature;
}

abstract class HttpClient {
  HttpClient();

  Future<String> fetchData(String filePath);

  Future<List<Project>> getProjectList() async {
    // Fetch data from server
    final data = await fetchData('logs/index.html');

    // Get list items from HTML
    final listItems = parseLogListFromHTML(data);

    // Parse projects from logs
    final Map<String, List<Log>> projects = {};
    for (int i = 0; i < listItems.length; i++) {
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
    final data = await fetchData('logs${log.uri}');

    final loglines = parseLogData(data);

    if (loglines.isEmpty) {
      return TankSnapshot(log, [], null, null);
    } else if (loglines.length < 360) {
      return TankSnapshot(
        log,
        loglines,
        loglines[loglines.length - 1].phCurrent,
        loglines[loglines.length - 1].tempMean,
      );
    } else {
      return TankSnapshot(
        log,
        loglines.sublist(loglines.length - 360, loglines.length),
        loglines[loglines.length - 1].phCurrent,
        loglines[loglines.length - 1].tempMean,
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

    // Parse the date strings
    for (int i = 0; i < logTable.length; i++) {
      final parsedDate = DateTime.tryParse(logTable[i][3]);

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
            e[0].toDouble(),
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
    for (final Element e in parse(data).getElementsByTagName('li')) {
      if (e.children.isNotEmpty &&
          e.children[0].attributes.containsKey('href')) {
        final innerHtml = e.children[0].innerHtml;
        final name = innerHtml.substring(innerHtml.lastIndexOf('/') + 1);
        if (e.children[0].attributes['href']!.endsWith('.log')) {
          result.add([
            name,
            '/${e.children[0].attributes['href']!.split('/').last}',
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

  late String testHTML =
      '<html><body><ul><li><a href="/logs/ProjectA-tank-24.log">/logs/ProjectA-tank-24.log</a></li><li><a href="/logs/ProjectA-tank-70.log">/logs/ProjectA-tank-70.log</a></li><li><a href="/logs/ProjectB-tank-58.log">/logs/ProjectB-tank-58.log</a></li><li><ahref="/logs/index.html">/logs/index.html</a></li></ul></body></html>';

  @override
  Future<String> fetchData(String filePath) async {
    if (filePath == 'logs/index.html') {
      return testHTML;
    } else if (filePath == 'sample_short.log') {
      return '''
1.0	80	I	2025-01-07 11:02:30		31.25	31.11	0.07	6.38	6.41	0
1.0	80	I	2025-01-07 11:03:30		31.25	31.25	0.0	6.38	6.38	60
1.0	80	I	2025-01-07 11:04:30		31.25	31.43	0.09	6.38	6.36	120
1.0	80	I	2025-01-07 11:05:30		31.25	31.54	0.145	6.38	6.46	180
1.0	80	I	2025-01-07 11:06:30		31.25	31.42	0.085	6.38	6.35	240''';
    } else if (filePath == 'sample_long.log') {
      return sampleData();
    } else if (filePath == 'calibration.log') {
      return '''
1.0	80	I	2025-01-07 11:09:30		31.25	C	C	6.38	C	420''';
    } else if (filePath == 'warnings.log') {
      return '''
1.0	80	I	2025-01-07 11:20:30		31.25	30.81	0.22	6.38	6.3	1080
1.0	80	I	2025-01-07 11:21:30		31.25	30.99	0.13	6.38	6.38	1140
1.0	80	W	2025-01-07 11:22:30							1200	FD:C7:B3:E5:DC:8A	0.99	
1.0	80	I	2025-01-07 11:22:30		31.25	31.38	0.065	6.38	6.39	1200
1.0	80	I	2025-01-07 11:23:30		31.25	31.22	0.015	6.38	6.34	1260
''';
    } else if (filePath == 'empty.log') {
      return '';
    } else if (filePath == 'snapshot/ProjectA-tank-24.log') {
      return '''
1.0	24	I	2025-01-09 16:09:16		21.45	21.91	0.23	6.25	6.24	86220
1.0	24	I	2025-01-09 16:10:16		21.45	21.34	0.055	6.25	6.18	86280
1.0	24	I	2025-01-09 16:11:16		21.45	20.98	0.235	6.25	6.33	86340
''';
    } else if (filePath == 'snapshot/ProjectA-tank-70.log') {
      return '''
1.0	70	I	2025-01-09 16:04:29		23.29	23.27	0.01	7.22	7.34	86220
1.0	70	I	2025-01-09 16:05:29		23.29	23.26	0.015	7.22	7.1	86280
1.0	70	I	2025-01-09 16:06:29		23.29	23.67	0.19	7.22	7.29	86340
''';
    } else {
      throw Exception('Failed to fetch data from $filePath');
    }
  }
}
