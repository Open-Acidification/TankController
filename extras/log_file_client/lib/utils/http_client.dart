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

  Future<List<List>> getCsvTable(String filePath) async {
    final data = await fetchData(filePath);
    final List<List<dynamic>> csvTable =
        const CsvToListConverter(eol: '\n').convert(data);

    for (int i = 1; i < csvTable.length; i++) {
      csvTable[i][0] =
          DateTime.tryParse(formatDateString(csvTable[i][0].toString())) ??
              'InvalidDate';
    }

    return csvTable;
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
    } else if (filePath == 'empty.csv') {
      return '';
    } else {
      throw Exception('Failed to fetch data from $filePath');
    }
  }
}
