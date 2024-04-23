import 'package:log_file_client/tank_list.dart';
import 'package:test/test.dart';

//  how to run the test
//  flutter test test/tank_list_tests.dart

void main() {
  test('Tank constructor', () {
    final Tank newTank = Tank('Test1');

    expect(newTank.name, equals('Test1'));
  });

  test('fetchList() with test class', () async {
    final getTankList = TankListReaderForTest();
    final result = await getTankList.fetchList();

    expect(result, isList);
    expect(result.length, 3);
    expect(result[0].name, 'test1');
    expect(result[1].name, 'test2');
    expect(result[2].name, 'test3');
  });
  test('fetchList() from OAP html list', () async {
    final getTankList = TankListReaderForApp();
    final result = await getTankList.fetchList();

    expect(result, isList);
    expect(result.length, 3);
    expect(result[0].name, '/logs/test1.log');
    expect(result[1].name, '/logs/test2.log');
    expect(result[2].name, '/logs/test3.log');
  });
}
