// import 'package:flutter/material.dart';
// import 'package:flutter_test/flutter_test.dart';
import 'package:log_file_client/tank_list.dart';
import 'package:test/test.dart';

void main() {
  group('Tank creation of class and name', () {
    test('Tank constructor', () {
      final Tank newTank = Tank('Test1');

      expect(newTank.name, equals('Test1'));
    });
  });

  group('Fetching List of logs', () {
    test('fetchList() with test class', () async {
      final getTankList = GetTankListForTest();
      final result = await getTankList.fetchList();

      expect(result, isList);
      expect(result.length, 3);
      expect(result[0].name, 'test1');
      expect(result[1].name, 'test2');
      expect(result[2].name, 'test3');
    });
    test('fetchList() from OAP html list', () async {
      final getTankList = GetTankListForApp();
      final result = await getTankList.fetchList();

      expect(result, isList);
      expect(result.length, 3);
      expect(result[0].name, '/logs/test1.log');
      expect(result[1].name, '/logs/test2.log');
      expect(result[2].name, '/logs/test2.log');
    });
  });
}
