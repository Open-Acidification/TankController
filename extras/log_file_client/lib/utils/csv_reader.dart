import 'dart:async';
import 'dart:io';

import 'package:http/http.dart' as http;

abstract class CsvReader {
  final String filePath;

  CsvReader(this.filePath);

  Future<String> fetchCsvData();
  
}

// Subclass CsvReaderForTest
class CsvReaderForTest extends CsvReader {
  CsvReaderForTest(String filePath) : super(filePath);

  @override
  Future<String> fetchCsvData() async {
    try {
      // Read the file from the local file system asynchronously
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
      return response.body; // Return the raw CSV data
    } else {
      throw Exception('Failed to load CSV data');
    }
  }
}


// import 'package:http/http.dart' as http;

// abstract class CsvReader {
//   final String filePath;
//   CsvReader(this.filePath);

//   Future<String> fetchCsvData();


// }

// class CsvReaderForTest implements CsvReader {
//   CsvReaderForTest(String filePath) : super(filePath);

//   @override
//   Future<String> fetchCsvData() async {
//     // Implement test-specific CSV fetching logic here
//     return 'Test CSV data';
//   }
// }

// class CsvReaderForApp implements CsvReader {
//   CsvReaderForApp(this.filePath);
//   @override
//   final String filePath;

  // @override
  // Future<String> fetchCsvData() async {
  //   final url = Uri.https('oap.cs.wallawalla.edu', filePath);
  //   final http.Response response = await http.get(url);

  //   if (response.statusCode == 200) {
  //     return response.body; // Return the raw CSV data
  //   } else {
  //     throw Exception('Failed to load CSV data');
  //   }
  // }
// }