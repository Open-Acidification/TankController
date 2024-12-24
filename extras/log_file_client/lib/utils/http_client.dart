import 'dart:async';

import 'package:csv/csv.dart';
import 'package:html/parser.dart';
import 'package:http/http.dart' as http;
import 'package:log_file_client/utils/sample_data.dart';

class Log {
  Log(this.name, this.uri);
  final String name;
  final String uri;
}

class LogDataLine {
  LogDataLine(
    this.time,
    this.tankid,
    this.temp,
    this.tempSetpoint,
    this.pH,
    this.pHSetpoint,
    this.onTime,
    this.kp,
    this.ki,
    this.kd,
  );
  final DateTime time;
  final int tankid;
  final double? temp;
  final double? tempSetpoint;
  final double? pH;
  final double? pHSetpoint;
  final int? onTime;
  final double? kp;
  final double? ki;
  final double? kd;

  @override
  String toString() {
    return 'LogDataLine(tankid: $tankid, time: $time, temp: $temp, tempSetpoint: $tempSetpoint, '
        'pH: $pH, pHSetpoint: $pHSetpoint, onTime: $onTime, kp: $kp, ki: $ki, kd: $kd)';
  }

  @override
  bool operator ==(Object other) {
    if (identical(this, other)) {
      return true;
    }
    if (other is! LogDataLine) {
      return false;
    }
    return time == other.time &&
        tankid == other.tankid &&
        temp == other.temp &&
        tempSetpoint == other.tempSetpoint &&
        pH == other.pH &&
        pHSetpoint == other.pHSetpoint &&
        onTime == other.onTime &&
        kp == other.kp &&
        ki == other.ki &&
        kd == other.kd;
  }

  @override
  int get hashCode => Object.hash(
        time,
        tankid,
        temp,
        tempSetpoint,
        pH,
        pHSetpoint,
        onTime,
        kp,
        ki,
        kd,
      );
}

abstract class HttpClient {
  HttpClient();

  Future<String> fetchData(String filePath);

  String formatDateString(String dateString) {
    final parts = dateString.split(RegExp('[ /:]'));
    if (parts.length == 6) {
      final formattedDateString =
          '${parts[2]}-${parts[0]}-${parts[1]} ${parts[3]}:${parts[4]}:${parts[5]}';
      return formattedDateString;
    } else {
      return 'InvalidDate';
    }
  }

  Future<List<Log>> getLogList() async {
    // Fetch data from server
    final data = await fetchData('logs/index.html');

    // Get list items from HTML
    final listItems = parse(data)
        .getElementsByTagName('li')
        .map((e) {
          if (e.children.isNotEmpty &&
              e.children[0].attributes.containsKey('href')) {
            final innerHtml = e.children[0].innerHtml;
            final name = innerHtml.substring(innerHtml.lastIndexOf('/') + 1);
            if (e.children[0].attributes['href']!.endsWith('.csv')) {
              return [name, e.children[0].attributes['href']!];
            }
          }
          return null;
        })
        .where((item) => item != null)
        .toList();

    // Return list items as a list of logs
    return listItems.map((e) => Log(e![0], e[1])).toList();
  }

  Future<List<LogDataLine>?> getLogData(String filePath) async {
    final data = await fetchData(filePath);

    if (data.trim().isEmpty) {
      return null;
    }

    final List<List<dynamic>> csvTable =
        const CsvToListConverter(eol: '\n').convert(data);

    // Parse the date strings in the first column of each row
    for (int i = 1; i < csvTable.length; i++) {
      final parsedDate =
          DateTime.tryParse(formatDateString(csvTable[i][0].toString()));

      if (parsedDate == null) {
        throw FormatException(
          'Invalid date format in row ${i + 1}: ${csvTable[i][0].toString()}',
        );
      }

      csvTable[i][0] = parsedDate;
    }

    // Convert the list of lists to a list of LogDataLine objects
    final List<LogDataLine> logData = csvTable
        .sublist(1)
        .map(
          (e) => LogDataLine(
            e[0],
            e[1],
            e[2] is double ? e[2] : (e[2] is int ? e[2].toDouble() : null),
            e[3] is double ? e[3] : (e[3] is int ? e[3].toDouble() : null),
            e[4] is double ? e[4] : (e[4] is int ? e[4].toDouble() : null),
            e[5] is double ? e[5] : (e[5] is int ? e[5].toDouble() : null),
            e[6] is int ? e[6] : (e[6] is double ? e[6].toInt() : null),
            e[7] is double ? e[7] : (e[7] is int ? e[7].toDouble() : null),
            e[8] is double ? e[8] : (e[8] is int ? e[8].toDouble() : null),
            e[9] is double ? e[9] : (e[9] is int ? e[9].toDouble() : null),
          ),
        )
        .toList();

    return logData;
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
  // Fetches data from a hard-coded string (short) or the local Dart file (1000 sample lines) for testing purposes
  HttpClientTest();

  late String testHTML =
      '<html><body><ul><li><a href="/test1.csv">/logs/test1.csv</a></li><li><a href="/test2.csv"">/logs/test2.csv</a></li><li><a href="/test3.csv">/logs/test3.csv</a></li></ul></body></html>';

  @override
  Future<String> fetchData(String filePath) async {
    if (filePath == 'logs/index.html') {
      return testHTML;
    } else if (filePath == 'sample_short.csv') {
      return '''
time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd
01/20/2023 16:18:21,  99, 0.00, 10.00, 0.000, 8.645,    6,    700.0,    100.0,      0.0
01/20/2023 16:18:22,  99, 1.23, 10.00, 7.123, 8.645,    8,    710.0,    110.0,      1.0
01/20/2023 16:18:23,  99, 2.34, 10.00, 6.789, 8.645,    9,    720.0,    120.0,      2.0
01/20/2023 16:18:24,  99, 3.45, 10.00, 5.456, 8.645,   10,    730.0,    130.0,      3.0
01/20/2023 16:18:25,  99, 4.56, 10.00, 4.123, 8.645,   11,    740.0,    140.0,      4.0''';
    } else if (filePath == 'sample_long.csv') {
      return sampleData();
    } else if (filePath == 'invalid_date.csv') {
      return '''
      time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd
      01-20-2023 16:18:21,  99, 0.00, 10.00, 0.000, 8.645,    6,    700.0,    100.0,      0.0
      01/20/2023 16:18:22,  99, 1.23, 10.00, 7.123, 8.645,    8,    710.0,    110.0,      1.0
      ''';
    } else if (filePath == 'invalid_values.csv') {
      return '''
time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd
01/20/2023 16:49:44,  99, C, 11.00, C, 8.645,  959,    700.0,    100.0,      0.0
''';
    } else if (filePath == 'empty.csv') {
      return '';
    } else {
      throw Exception('Failed to fetch data from $filePath');
    }
  }
}
