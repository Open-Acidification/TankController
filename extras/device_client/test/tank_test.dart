import 'dart:convert';

import 'package:device_client/model/tank.dart';
import 'package:flutter_test/flutter_test.dart';

void main() {
  test('Tank set name/ip', () {
    final tank = Tank('tank_test', '192.168.0.1');

    expect(tank.name, 'tank_test');
    expect(tank.ip, '192.168.0.1');
  });

  test('Tank equivalence', () {
    final tank1 = Tank('tank_test', '192.168.0.1');
    final tank2 = Tank('tank_test', '192.168.0.1');
    final tank3 = Tank('tank', '192.168.0.1');

    expect(tank1, tank2);
    expect(tank1, isNot(tank3));
  });

  test('Tank encode/decode', () {
    dynamic tank = Tank('tank_test', '192.168.0.1');
    final String jsonTank = jsonEncode(tank);
    expect(jsonTank, '{"name":"tank_test","ip":"192.168.0.1"}');

    tank = jsonDecode(jsonTank);
    expect(tank, {'name': 'tank_test', 'ip': '192.168.0.1'});
  });
}
