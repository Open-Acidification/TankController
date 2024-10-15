import 'package:flutter_test/flutter_test.dart';
import 'package:log_file_client/utils/csv_reader.dart';
import 'package:test/test.dart' as test_package;

//  how to run the test
//  flutter test test/csv_reader_test.dart

void main() {
  TestWidgetsFlutterBinding.ensureInitialized();
  test_package.test('Constructors', () {
    final CsvReaderForTest newCsvReaderTest = CsvReaderForTest('test1.csv');
    final CsvReaderForAppWeb newCsvReaderAppWeb = CsvReaderForAppWeb('test2.csv');
    final CsvReaderForAppLocal newCsvReaderAppLocal = CsvReaderForAppLocal('test3.csv');

    expect(newCsvReaderTest.filePath, equals('test1.csv'));
    expect(newCsvReaderAppWeb.filePath, equals('test2.csv'));
    expect(newCsvReaderAppLocal.filePath, equals('test3.csv'));
  });

  test_package.test('fetchCsvData with test class', () async {
    final reader = CsvReaderForTest('test/test_assets/csv_reader_test.csv');
    final data = await reader.fetchCsvData();
    expect(data, 'time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd\r\n01/20/2023 16:18:21,  99, 0.00, 10.00, 0.000, 8.645,    6,    700.0,    100.0,      0.0\r\n01/20/2023 16:18:22,  98, 1.23, 10.00, 7.123, 8.645,    8,    710.0,    110.0,      1.0\r\n01/20/2023 16:18:23,  97, 2.34, 10.00, 6.789, 8.645,    9,    720.0,    120.0,      2.0\r\n01/20/2023 16:18:24,  96, 3.45, 10.00, 5.456, 8.645,   10,    730.0,    130.0,      3.0\r\n01/20/2023 16:18:25,  95, 4.56, 10.00, 4.123, 8.645,   11,    740.0,    140.0,      4.0\r\n');
  });

  // THIS TEST IS COMMENTED OUT BECAUSE test suite uses TestWidgetsFlutterBinding so https requests don't work
  // But also this class isn't currently being used due to CORS issues

  // test_package.test('fetchCsvData with appweb class', () async {
  //   final reader = CsvReaderForAppWeb('/logs/20230120.csv');
  //   final data = await reader.fetchCsvData();

  //   // Split the lines to not have to check all of them
  //   final lines = data.split('\n');

  //   // Here we're only checking the first three rows of the CSV
  //   expect(lines[0], 'time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd');
  //   expect(lines[1], '01/20/2023 16:18:21,  99, 0.00, 11.00, 0.000, 8.645,    6,    700.0,    100.0,      0.0');
  //   expect(lines[2], '01/20/2023 16:18:22,  99, 0.00, 11.00, 0.000, 8.645,    8,    700.0,    100.0,      0.0');
  // });

  test_package.test('fetchCsvData with applocal class', () async {
    final reader = CsvReaderForAppLocal('/logs/20230120.csv');
    final data = await reader.fetchCsvData();

    // Split the lines to not have to check all of them
    final lines = data.split('\n');

    // Here we're only checking the first three rows of the CSV
    expect(lines[0], 'time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd');
    expect(lines[1], '01/20/2023 16:18:21,  99, 0.00, 11.00, 0.000, 8.645,    6,    700.0,    100.0,      0.0');
    expect(lines[2], '01/20/2023 16:18:22,  99, 0.00, 11.00, 0.000, 8.645,    8,    700.0,    100.0,      0.0');
  });

  test_package.test('formatDateString', () {
    final reader = CsvReaderForTest('test');
    
    final data1 = reader.formatDateString('01/20/2023 16:18:21');
    expect(data1, equals('2023-01-20 16:18:21'));

    final data2 = reader.formatDateString('02/15/2021 14:22:45');
    expect(data2, equals('2021-02-15 14:22:45'));

    final data3 = reader.formatDateString('03/10/2022 09:30:12');
    expect(data3, equals('2022-03-10 09:30:12'));

    final data4 = reader.formatDateString('04/05/2020 18:45:33');
    expect(data4, equals('2020-04-05 18:45:33'));
  });

  test_package.test('csvTable', () async {
    final reader = CsvReaderForTest('test/test_assets/csv_reader_test.csv');
    final data = await reader.csvTable();
    expect(data, 
    [['time','tankid','temp','temp setpoint','pH','pH setpoint','onTime','Kp','Ki','Kd'],
    [DateTime.parse('2023-01-20 16:18:21'), 99, 0.00, 10.00, 0.000, 8.645, 6, 700.0, 100.0, 0.0],
    [DateTime.parse('2023-01-20 16:18:22'), 98, 1.23, 10.00, 7.123, 8.645, 8, 710.0, 110.0, 1.0],
    [DateTime.parse('2023-01-20 16:18:23'), 97, 2.34, 10.00, 6.789, 8.645, 9, 720.0, 120.0, 2.0],
    [DateTime.parse('2023-01-20 16:18:24'), 96, 3.45, 10.00, 5.456, 8.645, 10, 730.0, 130.0, 3.0],
    [DateTime.parse('2023-01-20 16:18:25'), 95, 4.56, 10.00, 4.123, 8.645, 11, 740.0, 140.0, 4.0]
    ],);
  });

}
