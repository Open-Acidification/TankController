import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:log_file_client/components/csv_view.dart';
import 'package:log_file_client/main.dart';
import 'package:log_file_client/pages/home_page.dart';
import 'package:log_file_client/utils/log_list.dart';

void main() {
  TestWidgetsFlutterBinding.ensureInitialized();

  testWidgets('MyApp has a title and HomePage', (WidgetTester tester) async {
    await tester.pumpWidget(const MyApp());
    expect(find.text('Tank Monitor'), findsOneWidget);
    expect(find.byType(HomePage), findsOneWidget);
  });

  testWidgets('HomePage displays CircularProgressIndicator initially',
      (WidgetTester tester) async {
    await tester.pumpWidget(
      MaterialApp(
        home: const HomePage(),
      ),
    );

    expect(find.byType(CircularProgressIndicator), findsOneWidget);
  });

  testWidgets('Drawer displays log list after loading',
      (WidgetTester tester) async {
    // Mock the log list
    final logList = [
      Log('log1.csv', '/log1.csv'),
      Log('log2.csv', '/log2.csv'),
    ];

    // Build the HomePage widget with a mock LogListReader
    await tester.pumpWidget(
      MaterialApp(
        home: HomePage(
          logListReader: MockLogListReader(logList),
        ),
      ),
    );

    // Wait for the fetchList method to complete
    await tester.pumpAndSettle();

    // Open the drawer
    await tester.tap(find.byTooltip('Open navigation menu'));
    await tester.pumpAndSettle();

    // Verify that the log list is displayed in the drawer
    expect(find.text('log1.csv'), findsOneWidget);
    expect(find.text('log2.csv'), findsOneWidget);
  });

  testWidgets('Drawer opens log file when selected',
      (WidgetTester tester) async {
    // Mock the log list
    final logList = [
      Log('log1.csv', '/log1.csv'),
      Log('log2.csv', '/log2.csv'),
    ];

    // Build the HomePage widget with a mock LogListReader
    await tester.pumpWidget(
      MaterialApp(
        home: HomePage(
          logListReader: MockLogListReader(logList),
        ),
      ),
    );

    // Wait for the fetchList method to complete
    await tester.pumpAndSettle();

    // Open the drawer
    await tester.tap(find.byTooltip('Open navigation menu'));
    await tester.pumpAndSettle();

    // Tap on the first log file
    await tester.tap(find.text('log1.csv'));
    await tester.pumpAndSettle();

    // Verify that the CsvView widget is displayed
    expect(find.byType(CsvView), findsOneWidget);
  });

  testWidgets('CsvView displays table with log data from CSV file',
      (WidgetTester tester) async {
    // Build the CsvView widget with mock CSV file
    await tester.pumpWidget(
      MaterialApp(
        home: Scaffold(
          body: CsvView(
            csvPath: 'sample_short.csv',
          ),
        ),
      ),
    );

    // Check that table is loading
    expect(find.byType(FutureBuilder<List>), findsOneWidget);
    expect(find.byType(CircularProgressIndicator), findsOneWidget);
    await tester.pumpAndSettle();
    expect(find.byType(CircularProgressIndicator), findsNothing);

    // Verify that there is no error message
    expect(find.text('Error'), findsNothing);
    expect(find.text('No data found'), findsNothing);

    // Verify that the Column widget is displayed
    await tester.pumpAndSettle();
    expect(find.byType(Column), findsOneWidget);

    // Verify that the table headers are displayed
    expect(find.text('time'), findsOneWidget);
    expect(find.text('tankid'), findsOneWidget);
    expect(find.text('temp'), findsOneWidget);
    expect(find.text('temp setpoint'), findsOneWidget);
    expect(find.text('pH'), findsOneWidget);
    expect(find.text('pH setpoint'), findsOneWidget);
    expect(find.text('onTime'), findsOneWidget);
    expect(find.text('Kp'), findsOneWidget);
    expect(find.text('Ki'), findsOneWidget);
    expect(find.text('Kd'), findsOneWidget);

    // Verify that the table data is displayed
    expect(find.byType(ListView), findsOneWidget);

    expect(find.text('2023-01-20 16:18:21.000'), findsOneWidget);
    expect(find.text('2023-01-20 16:18:22.000'), findsOneWidget);
    expect(find.text('2023-01-20 16:18:23.000'), findsOneWidget);
    expect(find.text('2023-01-20 16:18:24.000'), findsOneWidget);
    expect(find.text('2023-01-20 16:18:25.000'), findsOneWidget);

    expect(find.text('99'), findsNWidgets(5));

    expect(find.text('0.000'), findsNWidgets(2));
    expect(find.text('1.230'), findsOneWidget);
    expect(find.text('2.340'), findsOneWidget);
    expect(find.text('3.450'), findsOneWidget);
    expect(find.text('4.560'), findsOneWidget);

    expect(find.text('10.000'), findsNWidgets(5));

    expect(find.text('7.123'), findsOneWidget);
    expect(find.text('6.789'), findsOneWidget);
    expect(find.text('5.456'), findsOneWidget);
    expect(find.text('4.123'), findsOneWidget);

    expect(find.text('8.645'), findsNWidgets(5));

    expect(find.text('6'), findsOneWidget);
    expect(find.text('8'), findsOneWidget);
    expect(find.text('9'), findsOneWidget);
    expect(find.text('10'), findsOneWidget);
    expect(find.text('11'), findsOneWidget);

    expect(find.text('700'), findsOneWidget);
    expect(find.text('710'), findsOneWidget);
    expect(find.text('720'), findsOneWidget);
    expect(find.text('730'), findsOneWidget);
    expect(find.text('740'), findsOneWidget);

    expect(find.text('100'), findsOneWidget);
    expect(find.text('110'), findsOneWidget);
    expect(find.text('120'), findsOneWidget);
    expect(find.text('130'), findsOneWidget);
    expect(find.text('140'), findsOneWidget);

    expect(find.text('0'), findsOneWidget);
    expect(find.text('1'), findsOneWidget);
    expect(find.text('2'), findsOneWidget);
    expect(find.text('3'), findsOneWidget);
    expect(find.text('4'), findsOneWidget);
  });
}

class MockLogListReader extends LogListReader {
  MockLogListReader(this.logList);
  final List<Log> logList;

  @override
  Future<List<Log>> fetchList() async {
    return logList;
  }
}
