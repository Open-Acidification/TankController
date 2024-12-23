import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:log_file_client/components/csv_view.dart';
import 'package:log_file_client/components/graph_view.dart';
import 'package:log_file_client/main.dart';
import 'package:log_file_client/pages/home_page.dart';
import 'package:log_file_client/utils/http_client.dart';
import 'package:syncfusion_flutter_charts/charts.dart';

void main() {
  TestWidgetsFlutterBinding.ensureInitialized();

  testWidgets('MyApp has a title and HomePage', (WidgetTester tester) async {
    await tester.pumpWidget(
      MyApp(
        httpClient: HttpClientTest(),
      ),
    );
    expect(find.text('Tank Monitor'), findsOneWidget);
    expect(find.byType(HomePage), findsOneWidget);
  });

  testWidgets('HomePage displays CircularProgressIndicator initially',
      (WidgetTester tester) async {
    await tester.pumpWidget(
      MaterialApp(
        home: HomePage(
          httpClient: HttpClientTest(),
        ),
      ),
    );

    expect(find.byType(CircularProgressIndicator), findsOneWidget);
  });

  testWidgets('Drawer displays log list after loading',
      (WidgetTester tester) async {
    // Build the HomePage widget
    await tester.pumpWidget(
      MaterialApp(
        home: HomePage(
          httpClient: HttpClientTest(),
        ),
      ),
    );

    // Wait for the fetchList method to complete
    await tester.pumpAndSettle();

    // Open the drawer
    await tester.tap(find.byTooltip('Open navigation menu'));
    await tester.pumpAndSettle();

    // Verify that the drawer is open
    expect(find.byType(Drawer), findsOneWidget);

    // Verify that the log list is displayed in the drawer
    expect(find.text('test1.csv'), findsOneWidget);
    expect(find.text('test2.csv'), findsOneWidget);
    expect(find.text('test3.csv'), findsOneWidget);
  });

  testWidgets('Drawer opens log file when selected',
      (WidgetTester tester) async {
    // Build the HomePage widget
    await tester.pumpWidget(
      MaterialApp(
        home: HomePage(
          httpClient: HttpClientTest(),
        ),
      ),
    );

    // Wait for the fetchList method to complete
    await tester.pumpAndSettle();

    // Open the drawer
    await tester.tap(find.byTooltip('Open navigation menu'));
    await tester.pumpAndSettle();

    // Tap on the first log file
    await tester.tap(find.text('test1.csv'));
    await tester.pumpAndSettle();

    // Verify that the GraphView widget is displayed
    expect(find.byType(GraphView), findsOneWidget);
  });

  testWidgets('CsvView displays table with log data from CSV file',
      (WidgetTester tester) async {
    // Build the CsvView widget
    await tester.pumpWidget(
      MaterialApp(
        home: Scaffold(
          body: CsvView(
            csvPath: 'sample_short.csv',
            httpClient: HttpClientTest(),
          ),
        ),
      ),
    );

    // Check that table is loading
    expect(find.byType(FutureBuilder<List<LogDataLine>>), findsOneWidget);
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

    expect(find.text('1.23'), findsOneWidget);
    expect(find.text('2.34'), findsOneWidget);
    expect(find.text('3.45'), findsOneWidget);
    expect(find.text('4.56'), findsOneWidget);

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
  });

  testWidgets('GraphView widget test', (WidgetTester tester) async {
    // Build GraphView widget
    await tester.pumpWidget(
      MaterialApp(
        home: Scaffold(
          body: GraphView(
            csvPath: 'sample_short.csv',
            httpClient: HttpClientTest(),
          ),
        ),
      ),
    );

    // Verify that a CircularProgressIndicator is shown while loading
    expect(find.byType(CircularProgressIndicator), findsOneWidget);

    // Wait for the FutureBuilder to complete
    await tester.pumpAndSettle();

    // Verify that the SfCartesianChart is rendered
    expect(find.byType(SfCartesianChart), findsOneWidget);

    // Verify that the chart contains the correct line series for temperature and pH
    expect(find.text('temp'), findsOneWidget);
    expect(find.text('pH'), findsOneWidget);

    // Check for LineSeries widget presence
    expect(
      find.byWidgetPredicate(
        (widget) => widget is LineSeries,
      ),
      findsNWidgets(4),
    );
  });
}
