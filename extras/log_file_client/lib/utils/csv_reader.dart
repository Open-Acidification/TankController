import 'dart:async';

import 'package:csv/csv.dart';
import 'package:http/http.dart' as http;
import 'package:log_file_client/utils/sample_data.dart';

abstract class CsvReader {
  CsvReader(this.filePath);
  final String filePath;

  Future<String> fetchCsvData();

  String formatDateString(String dateString) {
    final parts = dateString.split(RegExp('[ /:]'));
    if (parts.length == 6) {
      final formattedDateString =
          '${parts[2]}-${parts[0]}-${parts[1]} ${parts[3]}:${parts[4]}:${parts[5]}';
      return formattedDateString;
    } else {
      throw Exception('Unable to format date string $dateString.');
    }
  }

  Future<List<List>> csvTable() async {
    final data = await fetchCsvData();
    final List<List<dynamic>> csvTable =
        const CsvToListConverter(eol: '\n').convert(data);

    for (int i = 1; i < csvTable.length; i++) {
      csvTable[i][0] =
          DateTime.tryParse(formatDateString(csvTable[i][0].toString())) ??
              csvTable[i][0];
    }

    return csvTable;
  }
}

class CsvReaderForApp extends CsvReader {
  // Fetches data from the https website. Causes CORS issues.
  CsvReaderForApp(super.filePath);

  @override
  Future<String> fetchCsvData() async {
    final url = Uri.https('oap.cs.wallawalla.edu', filePath);
    final http.Response response = await http.get(url);

    if (response.statusCode == 200) {
      return response.body;
    } else {
      throw Exception('Failed to load CSV data');
    }
  }
}

class CsvReaderLocal extends CsvReader {
  // Fetches data from the hard-coded string (short) or the local Dart file (1000 sample lines).
  // For testing purposes
  CsvReaderLocal(super.filePath);

  @override
  Future<String> fetchCsvData() async {
    if (filePath == 'sample_short.csv') {
      return '''
time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd
01/20/2023 16:18:21,  99, 0.00, 10.00, 0.000, 8.645,    6,    700.0,    100.0,      0.0
01/20/2023 16:18:22,  99, 1.23, 10.00, 7.123, 8.645,    8,    710.0,    110.0,      1.0
01/20/2023 16:18:23,  99, 2.34, 10.00, 6.789, 8.645,    9,    720.0,    120.0,      2.0
01/20/2023 16:18:24,  99, 3.45, 10.00, 5.456, 8.645,   10,    730.0,    130.0,      3.0
01/20/2023 16:18:25,  99, 4.56, 10.00, 4.123, 8.645,   11,    740.0,    140.0,      4.0''';
    }
    if (filePath == 'sample_long.csv') {
      return sampleData();
    }
    throw Exception('Failed to load CSV file from path $filePath');
  }
}
