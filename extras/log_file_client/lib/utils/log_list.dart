import 'package:html/parser.dart';
import 'package:http/http.dart' as http;

class Log {
  Log(this.name, this.uri);
  final String name;
  final String uri;
}

abstract class LogListReader {
  Future<List<Log>> fetchList();
  static LogListReader? _current;
  static LogListReader get current {
    _current ??= LogListReaderForApp();
    return _current!;
  }

  static set current(LogListReader? aTankListReader) =>
      _current = aTankListReader;
}

class LogListReaderForTest implements LogListReader {
  @override
  Future<List<Log>> fetchList() async {
    return [Log('test1', '/test1.test'), Log('test2', '/test2.test'), Log('test3','/test3.test')];
  }
}

class LogListReaderForApp implements LogListReader {
  @override
  Future<List<Log>> fetchList() async {
    final url = Uri.https('oap.cs.wallawalla.edu', '/logs/index.html');
    final http.Response response = await http.get(url);
    if (response.statusCode != 200) {
      throw response.reasonPhrase!;
    }
    final document = parse(response.body);
    final listItems = document
        .getElementsByTagName('li')
        .map((e) {
          final innerHtml = e.children[0].innerHtml;
          final name = innerHtml.substring(innerHtml.lastIndexOf('/') + 1);
          return Log(name, e.children[0].attributes['href']! );
        })
        .toList();
    return listItems;
  }
}