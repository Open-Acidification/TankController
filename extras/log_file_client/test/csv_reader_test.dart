import 'package:log_file_client/utils/csv_reader.dart';
import 'package:test/test.dart';

//  how to run the test
//  flutter test test/csv_reader_tests.dart

void main() {
  test('Constructors', () {
    final CsvReaderForTest newCsvReaderTest = CsvReaderForTest('test1.csv');
    final CsvReaderForApp newCsvReaderApp = CsvReaderForApp('test2.csv');

    expect(newCsvReaderTest.filePath, equals('test1.csv'));
    expect(newCsvReaderApp.filePath, equals('test2.csv'));
  });

  test('fetchCsvData with test class', () async {
    final reader = CsvReaderForTest('test/test_assets/csv_reader_test.csv');
    final data = await reader.fetchCsvData();
    expect(data, 'time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd\r\n01/20/2023 16:18:21,  99, 0.00, 10.00, 0.000, 8.645,    6,    700.0,    100.0,      0.0\r\n01/20/2023 16:18:22,  98, 1.23, 10.00, 7.123, 8.645,    8,    710.0,    110.0,      1.0\r\n01/20/2023 16:18:23,  97, 2.34, 10.00, 6.789, 8.645,    9,    720.0,    120.0,      2.0\r\n01/20/2023 16:18:24,  96, 3.45, 10.00, 5.456, 8.645,   10,    730.0,    130.0,      3.0\r\n01/20/2023 16:18:25,  95, 4.56, 10.00, 4.123, 8.645,   11,    740.0,    140.0,      4.0');
  });

  test('fetchCsvData with app class', () async {
    final reader = CsvReaderForApp('/logs/20230120.csv'); // Update the file path as needed
    final data = await reader.fetchCsvData();

    // Split the lines to not have to check all of them
    final lines = data.split('\n');

    // Here we're only checking the first three rows of the CSV
    expect(lines[0], 'time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd');
    expect(lines[1], '01/20/2023 16:18:21,  99, 0.00, 11.00, 0.000, 8.645,    6,    700.0,    100.0,      0.0');
    expect(lines[2], '01/20/2023 16:18:22,  99, 0.00, 11.00, 0.000, 8.645,    8,    700.0,    100.0,      0.0');
  });

}
