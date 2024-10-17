import 'package:flutter/services.dart';
import 'package:html/parser.dart';
import 'package:http/http.dart' as http;

class Log {
  Log(this.name, this.uri);
  final String name;
  final String uri;
}

abstract class LogListReader {
  Future<List<Log>> fetchList();

  List<Log> parseHTML(String html) {
    final document = parse(html);
    final listItems = document
      .getElementsByTagName('li')
      .map((e) {
        final innerHtml = e.children[0].innerHtml;
        final name = innerHtml.substring(innerHtml.lastIndexOf('/') + 1);
        if (e.children[0].attributes['href']!.endsWith('.csv')) {
          return [name, e.children[0].attributes['href']!];
        }
      })
      .where((item) => item != null)
      .toList();
    
    return listItems.map((e) => Log(e![0], e[1])).toList();
  }
}

class LogListReaderForTest extends LogListReader {
  final String testHTML = '<html><body><ul><li><a href="/test1.csv">/logs/test1</a></li><li><a href="/test2.csv"">/logs/test2</a></li><li><a href="/test3.csv">/logs/test3</a></li></ul></body></html>';

  @override
  Future<List<Log>> fetchList() async {
    return parseHTML(testHTML);
  }
}

class LogListReaderForAppWeb extends LogListReader {
  // Fetches data from the https website. Causes CORS issues.
  @override
  Future<List<Log>> fetchList() async {
    final url = Uri.https('oap.cs.wallawalla.edu', '/logs/index.html');
    final http.Response response = await http.get(url);
    if (response.statusCode != 200) {
      throw response.reasonPhrase!;
    }
    final logList = parseHTML(response.body);
    return logList;
  }
}

class LogListReaderForAppLocal extends LogListReader {
  // Fetches data from the local file system (logs/). To bypass CORS issue.
  @override
  Future<List<Log>> fetchList() async {
    final html = await rootBundle.loadString('logs/index.html');
    final logList = parseHTML(html);
    return logList;
  }
}
