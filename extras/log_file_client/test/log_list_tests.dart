import 'package:log_file_client/utils/log_list.dart';
import 'package:test/test.dart';

//  how to run the test
//  flutter test test/log_list_tests.dart

void main() {
  test('Log constructor', () {
    final Log newLog = Log('Test1', 'Test1.test');

    expect(newLog.name, equals('Test1'));
    expect(newLog.uri, equals('Test1.test'));
  });

  test('fetchList() with test class', () async {
    final getLogList = LogListReaderForTest();
    final result = await getLogList.fetchList();

    expect(result, isList);
    expect(result.length, 3);
    expect(result[0].name, 'test1');
    expect(result[1].name, 'test2');
    expect(result[2].name, 'test3');
    expect(result[0].uri, '/test1.test');
    expect(result[1].uri, '/test2.test');
    expect(result[2].uri, '/test3.test');
  });

  test('fetchList() from OAP html list', () async {
  // This test is only to pass the current structure of the HTML list. 
  // It will fail when the structure of the HTML list changes.
    final getLogList = LogListReaderForApp();
    final result = await getLogList.fetchList();

    expect(result, isList);
    expect(result.length, 4);

    expect(result[0].name, '20230121.log');
    expect(result[1].name, '20230120.csv');
    expect(result[2].name, '20230120.log');
    expect(result[3].name, 'index.html');

    expect(result[0].uri, '/logs/20230121.log');
    expect(result[1].uri, '/logs/20230120.csv');
    expect(result[2].uri, '/logs/20230120.log');
    expect(result[3].uri, '/logs/index.html');

  });
}
