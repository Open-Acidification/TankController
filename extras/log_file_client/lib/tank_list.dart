import 'package:flutter/material.dart';
import 'package:html/parser.dart';
import 'package:http/http.dart' as http;

class Tank {
  Tank(this.name);
  late String name;
}

abstract class TankListReader {
  Future<List<Tank>> fetchList();
  static TankListReader? _current;
  static TankListReader get current {
    _current ??= TankListReaderForApp();
    return _current!;
  }

  static set current(TankListReader? aTankListReader) =>
      _current = aTankListReader;
}

class TankListReaderForTest implements TankListReader {
  @override
  Future<List<Tank>> fetchList() async {
    return [Tank('test1'), Tank('test2'), Tank('test3')];
  }
}

class TankListReaderForApp implements TankListReader {
  @override
  Future<List<Tank>> fetchList() async {
    final url = Uri.https('oap.cs.wallawalla.edu', '/logs/index.html');
    final http.Response response = await http.get(url);
    debugPrint('Response status: ${response.statusCode}');
    if (response.statusCode != 200) {
      throw response.statusCode;
    }
    final htmlString = response.body;
    final document = parse(htmlString);
    final listItems = document
        .getElementsByTagName('li')
        .map((e) => e.children[0].innerHtml)
        .toList();
    debugPrint(listItems.toList().toString());
    final tankList = listItems.map((name) => Tank(name)).toList();
    return tankList;
  }
}
