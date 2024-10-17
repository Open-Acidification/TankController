import 'package:flutter_test/flutter_test.dart';
import 'package:log_file_client/utils/log_list.dart';
import 'package:test/test.dart' as test_package;

//  how to run the test
//  flutter test test/log_list_test.dart

void main() {
  TestWidgetsFlutterBinding.ensureInitialized();

  test_package.test('Log constructor', () {
    final Log newLog = Log('Test1', 'Test1.test');

    expect(newLog.name, equals('Test1'));
    expect(newLog.uri, equals('Test1.test'));
  });

  test_package.test('fetchList() with test class', () async {
    final getLogList = LogListReaderForTest();
    final result = await getLogList.fetchList();

    expect(result, isList);
    expect(result.length, 3);
    expect(result[0].name, 'test1');
    expect(result[1].name, 'test2');
    expect(result[2].name, 'test3');
    expect(result[0].uri, '/test1.csv');
    expect(result[1].uri, '/test2.csv');
    expect(result[2].uri, '/test3.csv');
  });

  // THIS TEST IS COMMENTED OUT BECAUSE test suite uses TestWidgetsFlutterBinding so https requests don't work
  // But also this class isn't currently being used due to CORS issues

  // test_package.test('fetchList() from OAP html list', () async {
  // // This test is only to pass the current structure of the HTML list. 
  // // It will fail when the structure of the HTML list changes.
  //   final getLogList = LogListReaderForAppWeb();
  //   final result = await getLogList.fetchList();

  //   expect(result, isList);
  //   expect(result.length, 1);

  //   expect(result[0].name, '20230120.csv');
  //   expect(result[0].uri, '/logs/20230120.csv');

  // });

  test_package.test('fetchList() from local html list', () async {
  // This test is only to pass the current structure of the HTML list. 
  // It will fail when the structure of the HTML list changes.
    final getLogList = LogListReaderForAppLocal();
    final result = await getLogList.fetchList();

    expect(result, isList);
    expect(result.length, 1);

    expect(result[0].name, '20230120.csv');
    expect(result[0].uri, '/logs/20230120.csv');

  });
}
