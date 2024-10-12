import 'dart:async';
import 'dart:io';

import 'package:http/http.dart' as http;

abstract class CsvReader {
  final String filePath;

  CsvReader(this.filePath);

  Future<String> fetchCsvData();

  String formatDateString(String dateString) {
    final parts = dateString.split(RegExp('[ /:]'));
    if (parts.length == 6) {
      final formattedDateString = '${parts[2]}-${parts[0]}-${parts[1]} ${parts[3]}:${parts[4]}:${parts[5]}';
      return formattedDateString;
    } else {
      throw Exception('Unable to format date string.');
    }
  }

  Future<List<List>> csvTable() {
    return fetchCsvData().then((String data) {
      final List<List> table = [];

      final List<String> rows = data.split('\n');
      for (int i = 0; i < rows.length; i++) {
        final List<String> stringCells = rows[i].replaceAll('\r', '').split(',');

        // Convert from strings to useful types
        List<dynamic> cells = List.generate(stringCells.length, (int j) {
          if (i == 0) {
            return stringCells[j];
          } else {
            switch (j) {
              case 0:
                return DateTime.tryParse(formatDateString(stringCells[j])) ?? stringCells[j];
              case 1:
              case 6:
                return int.tryParse(stringCells[j]) ?? stringCells[j];
              case 2:
              case 3:
              case 4:
              case 5:
              case 7:
              case 8:
              case 9:
                return double.tryParse(stringCells[j]) ?? stringCells[j];
            }
          }
        });

        table.add(cells);
      }

      return table;
    });
  }
  
}

class CsvReaderForTest extends CsvReader {
  CsvReaderForTest(String filePath) : super(filePath);

  @override
  Future<String> fetchCsvData() async {
    try {
      final file = File(filePath);
      final String contents = await file.readAsString();
      return contents;
    } catch (e) {
      throw Exception('Failed to load CSV file: $e');
    }
  }
}

class CsvReaderForApp extends CsvReader {
  CsvReaderForApp(String filePath) : super(filePath);

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
