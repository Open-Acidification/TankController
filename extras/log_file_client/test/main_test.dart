import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:log_file_client/main.dart';
import 'package:log_file_client/tank_grid_view.dart';

void main() {
  testWidgets('MyApp widget test', (WidgetTester tester) async {
    await tester.pumpWidget(const MyApp());
    // Add your assertions here
  });
  testWidgets('MyHomePage widget test', (WidgetTester tester) async {
    await tester.pumpWidget(const MyHomePage());
    expect(find.byType(TankGridView), findsOneWidget);
  });

  testWidgets('MyApp build test', (WidgetTester tester) async {
    await tester.pumpWidget(const MyApp());
    expect(find.byType(MaterialApp), findsOneWidget);
    expect(find.text('Tank Monitor'), findsOneWidget);
    expect(find.byType(MyHomePage), findsOneWidget);
  });
}
