import 'package:flutter_test/flutter_test.dart';
import 'package:tank_manager/model/tc_interface.dart';

void main() {
  TcInterface.useMock();
  var tcInterface = TcInterface.instance;
  test('Post', () async {
    var post = await tcInterface.post('192.168.0.1', 'key?value=4');
    expect(
        post,
        'pH=7.352   7.218\n'
        'T=10.99 C 11.004');
  });

  test('Get Post', () async {
    var get = await tcInterface.get('192.168.0.1', '7');
    expect(
        get,
        'pH=7.352   7.218\n'
        'T=10.99 C 11.007');
  });

  test('Put', () async {
    var put = await tcInterface.put('192.168.0.1', 'PHSlope=22');
    expect(put, 'PHSlope=22');
  });

  test('Get Put', () async {
    var get = await tcInterface.get('192.168.0.1', 'PHSlope');
    expect(get, 'PHSlope=22');
  });
}
