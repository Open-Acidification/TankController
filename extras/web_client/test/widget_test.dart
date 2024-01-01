import 'package:flutter_test/flutter_test.dart';
import 'package:tank_manager/main.dart';
import 'package:tank_manager/model/app_data.dart';
import 'package:tank_manager/model/tank.dart';
import 'package:tank_manager/model/tc_interface.dart';

void main() {
  setUp(() {
    TcInterface.useMock();
  });

  tearDown(() {
    // Reset so that we no longer have a tank
    AppData.instance().clearTank();
  });

  testWidgets('Keypad buttons work as expected', (WidgetTester tester) async {
    // Build our app and trigger a frame.
    await tester.pumpWidget(const MyApp());
    final buttons = [
      '1',
      '2',
      '3',
      'A',
      '4',
      '5',
      '6',
      'B',
      '7',
      '8',
      '9',
      'C',
      '*',
      '0',
      '#',
      'D',
    ];

    // Create a tank
    final appData = AppData.instance();
    appData.currentTank = Tank('test_tank', '192.168.0.1');

    // Force button visibility for tester
    // Verify a buttons exists
    // Verify no button result is already displayed
    // Tap a button and check display
    for (final b in buttons) {
      await tester.ensureVisible(find.text(b));

      expect(find.text(b), findsOneWidget);
      expect(find.text('pH=7.352   7.218\nT=10.99 C 11.00$b'), findsNothing);

      await tester.tap(find.text(b));
      await tester.pump();
      expect(find.text('pH=7.352   7.218\nT=10.99 C 11.00$b'), findsOneWidget);
    }
  });

  testWidgets('Current Data is displayed', (WidgetTester tester) async {
    // Build our app and trigger a frame.
    await tester.pumpWidget(const MyApp());

    // Create a tank
    final appData = AppData.instance();
    appData.currentTank = Tank('test_tank', '192.168.0.1');

    // Verify Current Data is not displayed
    // Navigate to Current Data
    // Verify Current Data is displayed
    await tester.ensureVisible(find.text('Current Data'));
    expect(find.text('Current Data'), findsOneWidget);
    expect(find.text('90:A2:DA:0F:45:C0'), findsNothing);
    expect(find.text('FreeMemory'), findsNothing);
    await tester.tap(find.text('Current Data'));
    await tester.pump();
    expect(find.text('90:A2:DA:0F:45:C0'), findsOneWidget);
    expect(find.text('FreeMemory'), findsOneWidget);
  });

  testWidgets('Files are displayed', (WidgetTester tester) async {
    // Build our app and trigger a frame.
    await tester.pumpWidget(const MyApp());

    // Create a tank
    final appData = AppData.instance();
    appData.currentTank = Tank('test_tank', '192.168.0.1');

    // Verify files are not displayed
    // Navigate to Files
    // Verify files are displayed
    await tester.ensureVisible(find.text('Files'));
    expect(find.text('Files'), findsOneWidget);
    expect(find.text('20220217.csv'), findsNothing);
    expect(find.text('20220809.log'), findsNothing);
    expect(find.text('8005 KB'), findsNothing);
    await tester.tap(find.text('Files'));
    await tester.pump();
    expect(find.text('20220217.csv'), findsOneWidget);
    expect(find.text('20220809.log'), findsOneWidget);
    expect(find.text('8005 KB'), findsWidgets);
  });

  testWidgets('No tank selected', (WidgetTester tester) async {
    // Build our app and trigger a frame.
    await tester.pumpWidget(const MyApp());

    // Don't create a tank

    // Verify error message is not displayed
    // Navigate to Current Data
    // Verify error message is displayed
    await tester.ensureVisible(find.text('Current Data'));
    expect(find.text('Current Data'), findsOneWidget);
    expect(find.text('Error: Choose tank from menu'), findsNothing);
    await tester.tap(find.text('Current Data'));
    await tester.pump();
    expect(find.text('Error: Choose tank from menu'), findsOneWidget);

    // Navigate back to Keypad
    // Verify error message is not displayed
    // Navigate to Files
    // Verify error message is displayed
    await tester.ensureVisible(find.text('Keypad'));
    await tester.tap(find.text('Keypad'));
    await tester.pump();
    await tester.ensureVisible(find.text('Files'));
    expect(find.text('Files'), findsOneWidget);
    expect(find.text('Error: Choose tank from menu'), findsNothing);
    await tester.tap(find.text('Files'));
    await tester.pump();
    expect(find.text('Error: Choose tank from menu'), findsOneWidget);
  });
}
