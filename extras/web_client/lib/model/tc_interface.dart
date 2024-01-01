import 'package:http/http.dart' as http;

abstract class TcInterface {
  static TcInterface? _instance;

  static TcInterface instance() {
    _instance ??= TcRealInterface();
    return _instance!;
  }

  static void useMock() {
    _instance = TcMockInterface();
  }

  Future<String> get(String ip, String path, [int timeout = 5]);
  Future<String> post(String ip, String path, [int timeout = 5]);
  Future<String> put(String ip, String path);
}

class TcMockInterface extends TcInterface {
  @override
  Future<String> get(String ip, String path, [int timeout = 5]) async {
    if (ip == '127.0.0.1') {
      throw 'Invalid IP Address in TcMockInterface';
    }
    if (path == 'data') {
      return '{"IPAddress":"172.27.5.150","MAC":"90:A2:DA:0F:45:C0","FreeMemory":"3791 bytes","GoogleSheetInterval":10,"LogFile":"20220722.csv","PHSlope":"22","Kp":9000.4,"Ki":0.0,"Kd":0.0,"PID":"ON","TankID":3,"Uptime":"0d 0h 1m 7s","Version":"22.04.1"}';
    }
    if (path == 'rootdir') {
      return '20220217.log\t    62 KB\n20220217.csv\t  4321 KB\n20220218.csv\t  7616 KB\n20220218.log\t    69 KB\n20220219.csv\t  7668 KB\n20220219.log\t    70 KB\n20220220.csv\t  7668 KB\n20220220.log\t    70 KB\n20220221.csv\t  7668 KB\n20220221.log\t    67 KB\n20220222.csv\t  7668 KB\n20220222.log\t    66 KB\n20220223.csv\t  7668 KB\n20220223.log\t    75 KB\n20220224.csv\t  7668 KB\n20220224.log\t    69 KB\n20220225.csv\t  7668 KB\n20220225.log\t    70 KB\n20220226.csv\t  7668 KB\n20220226.log\t    72 KB\n20220227.csv\t  7668 KB\n20220227.log\t    71 KB\n20220228.csv\t  7668 KB\n20220228.log\t    67 KB\n20220301.csv\t  7748 KB\n20220301.log\t    66 KB\n20220302.csv\t  7461 KB\n20220302.log\t    63 KB\n20220303.log\t   108 KB\n20220303.csv\t  7250 KB\n20220304.csv\t  7637 KB\n20220304.log\t    68 KB\n20220305.csv\t  7668 KB\n20220305.log\t    70 KB\n20220306.csv\t  7668 KB\n20220306.log\t    70 KB\n20220307.csv\t  7668 KB\n20220307.log\t    68 KB\n20220308.csv\t  7668 KB\n20220308.log\t   106 KB\n20220309.csv\t  7660 KB\n20220309.log\t   118 KB\n20220310.csv\t  7655 KB\n20220310.log\t   114 KB\n20220311.csv\t  7582 KB\n20220311.log\t   107 KB\n20220312.csv\t  7623 KB\n20220312.log\t   102 KB\n20220313.csv\t  7655 KB\n20220313.log\t    99 KB\n20220314.csv\t  7579 KB\n20220314.log\t    90 KB\n20220315.csv\t  7624 KB\n20220315.log\t    95 KB\n20220316.csv\t  7655 KB\n20220316.log\t   101 KB\n20220317.csv\t  7655 KB\n20220317.log\t   112 KB\n20220318.csv\t   937 KB\n20220318.log\t    14 KB\n20220320.log\t     8 KB\n20220320.csv\t   784 KB\n20220321.csv\t  6537 KB\n20220321.log\t    60 KB\n20220322.csv\t  7573 KB\n20220322.log\t    66 KB\n20220323.csv\t  7584 KB\n20220323.log\t    64 KB\n20220324.csv\t  7666 KB\n20220324.log\t    64 KB\n20220325.csv\t  7668 KB\n20220325.log\t    64 KB\n20220326.csv\t  7668 KB\n20220326.log\t    64 KB\n20220327.csv\t  7668 KB\n20220327.log\t    64 KB\n20220328.csv\t  7668 KB\n20220328.log\t    64 KB\n20220329.csv\t  7668 KB\n20220329.log\t    66 KB\n20220330.csv\t  7668 KB\n20220330.log\t    64 KB\n20220331.csv\t  7668 KB\n20220331.log\t    64 KB\n20220401.csv\t  7668 KB\n20220401.log\t    64 KB\n20220402.csv\t  7668 KB\n20220402.log\t    64 KB\n20220403.csv\t  7714 KB\n20220403.log\t    64 KB\n20220404.csv\t  7752 KB\n20220404.log\t    64 KB\n20220405.csv\t  7752 KB\n20220405.log\t    64 KB\n20220406.csv\t  7752 KB\n20220406.log\t    64 KB\n20220407.csv\t  7752 KB\n20220407.log\t    64 KB\n20220408.csv\t  7752 KB\n20220408.log\t    64 KB\n20220409.csv\t  7752 KB\n20220409.log\t    64 KB\n20220410.csv\t  7752 KB\n20220410.log\t    64 KB\n20220411.csv\t  7757 KB\n20220411.log\t    64 KB\n20220412.csv\t  7837 KB\n20220412.log\t    66 KB\n20220413.csv\t  7836 KB\n20220413.log\t    66 KB\n20220414.csv\t  7828 KB\n20220414.log\t    65 KB\n20220415.csv\t  7708 KB\n20220415.log\t    55 KB\n20220416.csv\t  7583 KB\n20220416.log\t    37 KB\n20220417.csv\t  7664 KB\n20220417.log\t    37 KB\n20220418.csv\t  7668 KB\n20220418.log\t    37 KB\n20220419.csv\t  7668 KB\n20220419.log\t    37 KB\n20220420.csv\t  7668 KB\n20220420.log\t    37 KB\n20220421.csv\t  7668 KB\n20220421.log\t    38 KB\n20220422.csv\t  7668 KB\n20220422.log\t    37 KB\n20220423.csv\t  7668 KB\n20220423.log\t    37 KB\n20220424.csv\t  7672 KB\n20220424.log\t    37 KB\n20220425.csv\t  2997 KB\n20220425.log\t   423 KB\n20220426.csv\t  1647 KB\n20220426.log\t   526 KB\n20220427.csv\t  1253 KB\n20220427.log\t   564 KB\n20220428.log\t   580 KB\n20220428.csv\t  1133 KB\n20220429.log\t   589 KB\n20220429.csv\t   885 KB\n20220430.log\t   586 KB\n20220430.csv\t  1010 KB\n20220501.csv\t  1045 KB\n20220501.log\t   584 KB\n20220502.log\t   582 KB\n20220502.csv\t  1020 KB\n20220503.log\t   580 KB\n20220503.csv\t  1123 KB\n20220504.log\t   578 KB\n20220504.csv\t  1193 KB\n20220505.log\t   574 KB\n20220505.csv\t  1190 KB\n20220506.log\t    89 KB\n20220506.csv\t  7337 KB\n20220507.csv\t  7818 KB\n20220507.log\t    68 KB\n20220508.csv\t  7836 KB\n20220508.log\t    68 KB\n20220509.csv\t  7673 KB\n20220509.log\t   656 KB\n20220510.log\t   168 KB\n20220510.csv\t  7944 KB\n20220511.csv\t  8005 KB\n20220511.log\t    70 KB\n20220512.csv\t  7998 KB\n20220512.log\t    84 KB\n20220513.csv\t  8005 KB\n20220513.log\t    70 KB\n20220514.csv\t  8005 KB\n20220514.log\t    70 KB\n20220515.csv\t  8005 KB\n20220515.log\t    70 KB\n20220516.csv\t  8005 KB\n20220516.log\t    70 KB\n20220517.csv\t  8005 KB\n20220517.log\t    69 KB\n20220518.csv\t  8005 KB\n20220518.log\t    70 KB\n20220519.csv\t  8005 KB\n20220519.log\t    70 KB\n20220520.csv\t  8019 KB\n20220520.log\t    70 KB\n20220521.csv\t  8089 KB\n20220521.log\t    70 KB\n20220522.csv\t  8089 KB\n20220522.log\t    69 KB\n20220523.csv\t  8087 KB\n20220523.log\t    73 KB\n20220524.csv\t  8089 KB\n20220524.log\t    70 KB\n20220525.csv\t  8089 KB\n20220525.log\t    70 KB\n20220526.csv\t  8089 KB\n20220526.log\t    70 KB\n20220527.csv\t  8065 KB\n20220527.log\t   106 KB\n20220528.csv\t  8005 KB\n20220528.log\t    68 KB\n20220529.csv\t  8005 KB\n20220529.log\t    68 KB\n20220530.csv\t  8005 KB\n20220530.log\t    68 KB\n20220531.csv\t  8005 KB\n20220531.log\t    68 KB\n20220601.csv\t  8005 KB\n20220601.log\t    68 KB\n20220602.csv\t  7870 KB\n20220602.log\t    69 KB\n20220603.csv\t  7886 KB\n20220603.log\t    68 KB\n20220604.csv\t  7921 KB\n20220604.log\t    68 KB\n20220605.csv\t  7921 KB\n20220605.log\t    68 KB\n20220606.csv\t  7921 KB\n20220606.log\t    68 KB\n20220607.csv\t  7921 KB\n20220607.log\t    68 KB\n20220608.csv\t  7992 KB\n20220608.log\t    70 KB\n20220609.csv\t  8005 KB\n20220609.log\t    70 KB\n20220610.csv\t  8005 KB\n20220610.log\t    70 KB\n20220611.csv\t  8005 KB\n20220611.log\t    69 KB\n20220612.csv\t  7962 KB\n20220612.log\t    92 KB\n20220613.csv\t  7934 KB\n20220613.log\t    68 KB\n20220614.csv\t  8005 KB\n20220614.log\t    68 KB\n20220615.csv\t  8005 KB\n20220615.log\t    68 KB\n20220616.csv\t  8005 KB\n20220616.log\t    68 KB\n20220617.csv\t  8005 KB\n20220617.log\t    68 KB\n20220618.csv\t  8005 KB\n20220618.log\t    68 KB\n20220619.csv\t  8005 KB\n20220619.log\t    68 KB\n20220620.csv\t  8005 KB\n20220620.log\t    68 KB\n20220621.csv\t  8005 KB\n20220621.log\t    68 KB\n20220622.csv\t  8005 KB\n20220622.log\t    68 KB\n20220623.csv\t  8005 KB\n20220623.log\t    68 KB\n20220624.csv\t  8005 KB\n20220624.log\t    68 KB\n20220625.csv\t  8005 KB\n20220625.log\t    68 KB\n20220626.csv\t  8005 KB\n20220626.log\t    68 KB\n20220627.csv\t  8005 KB\n20220627.log\t    68 KB\n20220628.csv\t  8005 KB\n20220628.log\t    68 KB\n20220629.csv\t  8005 KB\n20220629.log\t    68 KB\n20220630.csv\t  8005 KB\n20220630.log\t    68 KB\n20220701.csv\t  8005 KB\n20220701.log\t    68 KB\n20220702.csv\t  7986 KB\n20220702.log\t    67 KB\n20220703.csv\t  7916 KB\n20220703.log\t    41 KB\n20220704.csv\t  7997 KB\n20220704.log\t    41 KB\n20220705.csv\t  8005 KB\n20220705.log\t    41 KB\n20220706.csv\t  8005 KB\n20220706.log\t    41 KB\n20220707.csv\t  8005 KB\n20220707.log\t    41 KB\n20220708.csv\t  8005 KB\n20220708.log\t    41 KB\n20220709.csv\t  8005 KB\n20220709.log\t    41 KB\n20220710.csv\t  8005 KB\n20220710.log\t    41 KB\n20220711.csv\t  8005 KB\n20220711.log\t    41 KB\n20220712.csv\t  8005 KB\n20220712.log\t    41 KB\n20220713.csv\t  7822 KB\n20220713.log\t    97 KB\n20220714.csv\t  7919 KB\n20220714.log\t    70 KB\n20220715.csv\t  8000 KB\n20220715.log\t    70 KB\n20220716.csv\t  8005 KB\n20220716.log\t    70 KB\n20220717.csv\t  8005 KB\n20220717.log\t    70 KB\n20220718.csv\t  8005 KB\n20220718.log\t    69 KB\n20220719.csv\t  8005 KB\n20220719.log\t    70 KB\n20220720.csv\t  8005 KB\n20220720.log\t    70 KB\n20220721.csv\t  7916 KB\n20220721.log\t    46 KB\n20220722.csv\t  7909 KB\n20220722.log\t   764 KB\n20220723.csv\t  7981 KB\n20220723.log\t    70 KB\n20220724.csv\t  8005 KB\n20220724.log\t    70 KB\n20220725.csv\t  8005 KB\n20220725.log\t    69 KB\n20220726.csv\t  8005 KB\n20220726.log\t    70 KB\n20220727.csv\t  8005 KB\n20220727.log\t    69 KB\n20220728.csv\t  8005 KB\n20220728.log\t    70 KB\n20220729.csv\t  8005 KB\n20220729.log\t    70 KB\n20220730.csv\t  8005 KB\n20220730.log\t    69 KB\n20220731.csv\t  8005 KB\n20220731.log\t    70 KB\n20220801.csv\t  8005 KB\n20220801.log\t    70 KB\n20220802.csv\t  7851 KB\n20220802.log\t    79 KB\n20220803.csv\t  7972 KB\n20220803.log\t    70 KB\n20220804.csv\t  7807 KB\n20220804.log\t   101 KB\n20220810.csv\t  7790 KB\n20220806.csv\t  7891 KB\n20220805.csv\t  7365 KB\n20220805.log\t   112 KB\n20220806.log\t    68 KB\n20220807.csv\t  7862 KB\n20220807.log\t    78 KB\n20220808.csv\t  7686 KB\n20220808.log\t    83 KB\n20220809.csv\t  7696 KB\n20220809.log\t   133 KB\n20220810.log\t    68 KB\n';
    }
    return 'pH=7.352   7.218\nT=10.99 C 11.00$path';
  }

  @override
  Future<String> post(String ip, String path, [int timeout = 5]) async {
    return 'pH=7.352   7.218\nT=10.99 C 11.00${path.substring(path.length - 1)}';
  }

  @override
  Future<String> put(String ip, String path) async {
    return '{"IPAddress":"172.27.5.150","MAC":"90:A2:DA:0F:45:C0","FreeMemory":"3791 bytes","GoogleSheetInterval":10,"LogFile":"20220722.csv","PHSlope":"22","Kp":9000.4,"Ki":0.0,"Kd":0.0,"PID":"ON","TankID":3,"Uptime":"0d 0h 1m 7s","Version":"22.04.1"}';
  }
}

class TcRealInterface extends TcInterface {
  @override
  Future<String> get(String ip, String path, [int timeout = 5]) async {
    final uri = 'http://$ip/api/1/$path';
    final future = http.get(Uri.parse(uri));
    final response = await future.timeout(Duration(seconds: timeout));
    if (response.statusCode != 200) {
      throw 'HTTP response not code 200';
    }
    final subString = response.body.toString().replaceAll('\r', '');
    return subString;
  }

  @override
  Future<String> post(String ip, String path, [int timeout = 5]) async {
    final uri = 'http://$ip/api/1/$path';
    final future = http.post(Uri.parse(uri));
    final response = await future.timeout(Duration(seconds: timeout));
    if (response.statusCode != 200) {
      throw 'HTTP response not code 200';
    }
    final subString = response.body.toString().replaceAll('\r', '');
    return subString;
  }

  @override
  Future<String> put(String ip, String path) async {
    final uri = 'http://$ip/api/1/$path';
    final future = http.put(Uri.parse(uri));
    final response = await future.timeout(const Duration(seconds: 5));
    if (response.statusCode != 200) {
      throw 'HTTP response not code 200';
    }
    final subString = response.body.toString().replaceAll('\r', '');
    return subString;
  }
}
