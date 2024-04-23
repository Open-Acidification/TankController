import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:log_file_client/main.dart';
import 'package:log_file_client/tank_list.dart';

void main() {
  setUp(
    () {
      TankListReader.current = TankListReaderForTest();
    },
  );

  tearDown(() {
    TankListReader.current = null;
  });

  testWidgets('GridView Appears', (WidgetTester tester) async {
    await tester.pumpWidget(const MyApp());
    expect(find.text('Tank Monitor'), findsOneWidget);
    expect(find.byType(CircularProgressIndicator), findsOneWidget);
    expect(find.byType(GridView), findsNothing);
    await tester.pump();
    expect(find.byType(CircularProgressIndicator), findsNothing);
    expect(find.byType(GridView), findsOneWidget);
    expect(find.byKey(const ValueKey('tank-card-0')), findsOneWidget);
    expect(find.byKey(const ValueKey('tank-card-1')), findsOneWidget);
    expect(find.byKey(const ValueKey('tank-card-2')), findsOneWidget);
    expect(find.byKey(const ValueKey('tank-card-3')), findsNothing);
    expect(find.text('test1'), findsOneWidget);
    expect(find.text('test2'), findsOneWidget);
    expect(find.text('test3'), findsOneWidget);
    expect(find.text('test4'), findsNothing);
  });
}
