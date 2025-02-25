import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:log_file_client/components/graph_view.dart';
import 'package:log_file_client/components/project_card.dart';
import 'package:log_file_client/components/table_view.dart';
import 'package:log_file_client/components/tank_card.dart';
import 'package:log_file_client/components/tank_thumbnail.dart';
import 'package:log_file_client/components/toggle_button.dart';
import 'package:log_file_client/main.dart';
import 'package:log_file_client/pages/home_page.dart';
import 'package:log_file_client/pages/project_page.dart';
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

  testWidgets('HomePage displays projects', (WidgetTester tester) async {
    await tester.pumpWidget(
      MaterialApp(
        home: HomePage(
          httpClient: HttpClientTest(),
        ),
      ),
    );

    await tester.pumpAndSettle();

    // Verify that the Projects header is displayed
    expect(find.text('Projects'), findsOneWidget);

    // Verify that a GridView of ProjectCards is displayed
    expect(find.byType(GridView), findsOneWidget);
    expect(find.byType(ProjectCard), findsNWidgets(2));

    // Verify that the ProjectCard widgets contain the correct text
    expect(find.text('fostja'), findsOneWidget);
    expect(find.text('stefan'), findsOneWidget);
  });

  testWidgets('ProjectCard opens project page when selected',
      (WidgetTester tester) async {
    await tester.pumpWidget(
      MaterialApp(
        home: HomePage(
          httpClient: HttpClientTest(),
        ),
      ),
    );
    await tester.pumpAndSettle();

    // Open the project page by tapping on the ProjectCard widget
    await tester.tap(find.text('fostja'));
    await tester.pumpAndSettle();

    // Verify that the project page is displayed
    expect(find.byType(ProjectPage), findsOneWidget);
  });

  testWidgets('ProjectPage displays TankCards', (WidgetTester tester) async {
    tester.view.physicalSize = const Size(1920, 1080);
    tester.view.devicePixelRatio = 1.0;
    await tester.pumpWidget(
      MaterialApp(
        home: ProjectPage(
          project: Project('ProjectA', [
            Log('tank-24', 'ProjectA-tank-24.log'),
            Log('tank-70', 'ProjectA-tank-70.log'),
          ]),
          httpClient: HttpClientTest(),
        ),
      ),
    );
    await tester.pumpAndSettle();

    // Verify that the Tanks header is displayed
    expect(find.text('ProjectA Tanks'), findsOneWidget);

    // Verify that a GridView of TankCards is displayed
    expect(find.byType(GridView), findsOneWidget);
    expect(find.byType(TankCard), findsNWidgets(2));

    // Verify that the TankCard widgets contain the correct text
    expect(find.text('tank-24'), findsOneWidget);
    expect(find.text('tank-70'), findsOneWidget);
  });

  testWidgets('TankCards have thumbnail graphs', (WidgetTester tester) async {
    // Build the ProjectPage widget
    await tester.pumpWidget(
      MaterialApp(
        home: ProjectPage(
          project:
              Project('ProjectA', [Log('tank-24', 'ProjectA-tank-24.log')]),
          httpClient: HttpClientTest(),
        ),
      ),
    );
    await tester.pumpAndSettle();
    expect(find.byType(TankCard), findsOneWidget);

    // Verify that the TankThumbnail widget is displayed
    expect(find.byType(TankThumbnail), findsOneWidget);
  });
  testWidgets('TankCard opens graph when selected',
      (WidgetTester tester) async {
    tester.view.physicalSize = const Size(1920, 1080);
    tester.view.devicePixelRatio = 1.0;
    await tester.pumpWidget(
      MaterialApp(
        home: ProjectPage(
          project: Project('ProjectA', [
            Log('tank-24', 'ProjectA-tank-24.log'),
            Log('tank-70', 'ProjectA-tank-70.log'),
          ]),
          httpClient: HttpClientTest(),
        ),
      ),
    );
    await tester.pumpAndSettle();

    // Open the graph page by tapping on the TankCard widget
    await tester.tap(find.text('tank-24'));
    await tester.pumpAndSettle();

    // Verify that the graph page is displayed
    expect(find.byType(GraphView), findsOneWidget);
  });

  testWidgets('TableView displays table with log data from file',
      (WidgetTester tester) async {
    // Build the TableView widget
    await tester.pumpWidget(
      MaterialApp(
        home: Scaffold(
          body: TableView(
            filePath: 'sample_short.log',
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
    expect(find.text('Version'), findsOneWidget);
    expect(find.text('Tank ID'), findsOneWidget);
    expect(find.text('Time'), findsOneWidget);
    expect(find.text('Temp Target'), findsOneWidget);
    expect(find.text('Temp Mean'), findsOneWidget);
    expect(find.text('Temp Std Dev'), findsOneWidget);
    expect(find.text('pH Target'), findsOneWidget);
    expect(find.text('pH'), findsOneWidget);
    expect(find.text('onTime'), findsOneWidget);

    // Verify that the table data is displayed
    expect(find.byType(ListView), findsOneWidget);

    expect(find.text('2025-01-23 15:38:00.000'), findsOneWidget);
    expect(find.text('2025-01-23 15:39:00.000'), findsOneWidget);
    expect(find.text('2025-01-23 15:40:00.000'), findsOneWidget);
    expect(find.text('2025-01-23 15:43:00.000'), findsOneWidget);
    expect(find.text('2025-01-23 15:44:00.000'), findsOneWidget);

    expect(find.text('89'), findsNWidgets(5));

    expect(find.text('20.11'), findsOneWidget);
    expect(find.text('20.18'), findsOneWidget);
    expect(find.text('20.24'), findsOneWidget);
    expect(find.text('20.38'), findsOneWidget);
    expect(find.text('20.44'), findsOneWidget);

    expect(find.text('20.0'), findsNWidgets(4));

    expect(find.text('0.0'), findsNWidgets(9));

    expect(find.text('7.0'), findsNWidgets(5));

    expect(find.text('60'), findsNWidgets(2));
    expect(find.text('120'), findsOneWidget);
    expect(find.text('180'), findsOneWidget);
    expect(find.text('121'), findsOneWidget);
  });

  testWidgets('GraphView widget test', (WidgetTester tester) async {
    // Build GraphView widget
    await tester.pumpWidget(
      MaterialApp(
        home: Scaffold(
          body: GraphView(
            filePath: 'sample_short.log',
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

  testWidgets('ToggleButton widget test', (WidgetTester tester) async {
    // Build GraphView widget
    await tester.pumpWidget(
      MaterialApp(
        home: Scaffold(
          body: GraphView(
            filePath: 'sample_short.log',
            httpClient: HttpClientTest(),
          ),
        ),
      ),
    );

    // Verify that ToggleButton is shown while loading
    expect(find.byType(ToggleButton), findsOneWidget);
    expect(find.byType(CircularProgressIndicator), findsOneWidget);

    await tester.pumpAndSettle();

    // Check that ToggleButton removes line series
    expect(
      find.byWidgetPredicate(
        (widget) => widget is LineSeries && widget.initialIsVisible,
      ),
      findsNWidgets(4),
    );

    await tester.tap(find.byKey(const Key('pH')));
    await tester.pumpAndSettle();

    expect(
      find.byWidgetPredicate(
        (widget) => widget is LineSeries && widget.initialIsVisible,
      ),
      findsNWidgets(2),
    );

    // Check that ToggleButton adds line series
    await tester.tap(find.byKey(const Key('pH')));
    await tester.pumpAndSettle();
    expect(
      find.byWidgetPredicate(
        (widget) => widget is LineSeries && widget.initialIsVisible,
      ),
      findsNWidgets(4),
    );
  });
}
