import 'package:device_client/model/tc_interface.dart';
import 'package:flutter_test/flutter_test.dart';

void main() {
  TcInterface.useMock();
  final tcInterface = TcInterface.instance();
  test('Post', () async {
    final post = await tcInterface.post('192.168.0.1', 'key?value=4');
    expect(
      post,
      'pH=7.352   7.218\n'
      'T=10.99 C 11.004',
    );
  });

  test('Get Post', () async {
    final get = await tcInterface.get('192.168.0.1', '7');
    expect(
      get,
      'pH=7.352   7.218\n'
      'T=10.99 C 11.007',
    );
  });

  test('Put', () async {
    final put = await tcInterface.put('192.168.0.1', 'data?PHSlope=22');
    expect(
      put,
      '{"IPAddress":"172.27.5.150","MAC":"90:A2:DA:0F:45:C0","FreeMemory":"3791 bytes","GoogleSheetInterval":10,"LogFile":"20220722.csv","PHSlope":"22","Kp":9000.4,"Ki":0.0,"Kd":0.0,"PID":"ON","TankID":3,"Uptime":"0d 0h 1m 7s","Version":"22.04.1"}',
    );
  });

  test('Get Put', () async {
    final get = await tcInterface.get('192.168.0.1', 'data');
    expect(get, '''
{
      "pH": "8.0",
      "Temperature": "20.0",
      "pH_FunctionType":"FLAT",
      "Target_pH": "7.0",
      "pH_RampHours":"0.0",
      "pH_SinePeriodHours":"0.0",
      "pH_SineAmplitude":"0.0",
      "Therm_FunctionType":"FLAT",
      "TargetTemperature":"21.75",
      "Therm_RampHours":"0.0",
      "Therm_SinePeriodHours":"0.0",
      "Therm_SineAmplitude":"0.0",
      "IPAddress": "172.27.5.150",
      "MAC": "90:A2:DA:0F:45:C0",
      "FreeMemory": "3791 bytes",
      "GoogleSheetInterval": 10,
      "LogFile": "20220722.csv",
      "PHSlope": "22",
      "Kp": 9000.4,
      "Ki": 0.0,
      "Kd": 0.0,
      "PID": "ON",
      "TankID": 3,
      "Uptime": "0d 0h 1m 7s",
      "HeatOrChill": "HEAT",
      "Version": "22.04.1",
      "EditableFields": [
            "Target_pH",
            "TargetTemperature",
            "GoogleSheetInterval",
            "Kp",
            "Ki",
            "Kd",
            "pH_RampHours",
            "Therm_RampHours",
            "TankID",
            "HeatOrChill",
            "PID"
      ]
}
''');
  });
}
