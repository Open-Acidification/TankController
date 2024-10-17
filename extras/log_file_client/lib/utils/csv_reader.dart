import 'dart:async';
import 'dart:io';

import 'package:csv/csv.dart';
import 'package:flutter/services.dart';
import 'package:http/http.dart' as http;

abstract class CsvReader {
  CsvReader(this.filePath);
  final String filePath;

  Future<String> fetchCsvData();

  String formatDateString(String dateString) {
    final parts = dateString.split(RegExp('[ /:]'));
    if (parts.length == 6) {
      final formattedDateString = '${parts[2]}-${parts[0]}-${parts[1]} ${parts[3]}:${parts[4]}:${parts[5]}';
      return formattedDateString;
    } else {
      throw Exception('Unable to format date string $dateString.');
    }
  }

  Future<List<List>> csvTable() async {
    final data = await fetchCsvData();
    final List<List<dynamic>> csvTable = CsvToListConverter().convert(data);

    for (int i = 1; i < csvTable.length; i++) {
      csvTable[i][0] = DateTime.tryParse(formatDateString(csvTable[i][0].toString())) ?? csvTable[i][0];
    }

    return csvTable;
  }
}

class CsvReaderForTest extends CsvReader {
  CsvReaderForTest(super.filePath);

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

class CsvReaderForAppWeb extends CsvReader {
  // Fetches data from the https website. Causes CORS issues.
  CsvReaderForAppWeb(super.filePath);

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

class CsvReaderForAppLocal extends CsvReader {
  // Fetches data from the local file system (logs/). To bypass CORS issue.
  CsvReaderForAppLocal(super.filePath);

  @override
  Future<String> fetchCsvData() async {
    final correctedFilePath = filePath.startsWith('/') ? filePath.substring(1) : filePath;
    final csv = await rootBundle.loadString(correctedFilePath);
    return csv;
  }
}
