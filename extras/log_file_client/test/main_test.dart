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

    testWidgets('HomePage displays CircularProgressIndicator initially', (WidgetTester tester) async {
    await tester.pumpWidget(
      MaterialApp(
        home: const HomePage(),
      ),
    );

    expect(find.byType(CircularProgressIndicator), findsOneWidget);
  });

  testWidgets('Drawer displays log list after loading', (WidgetTester tester) async {
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

  testWidgets('Drawer opens log file when selected', (WidgetTester tester) async {
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
}

class MockLogListReader implements LogListReader {
  MockLogListReader(this.logList);
  final List<Log> logList;

  @override
  Future<List<Log>> fetchList() async {
    return logList;
  }
}
