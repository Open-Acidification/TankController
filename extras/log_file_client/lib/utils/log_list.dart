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
}

class LogListReaderForTest implements LogListReader {
  @override
  Future<List<Log>> fetchList() async {
    return [Log('test1', '/test1.test'), Log('test2', '/test2.test'), Log('test3','/test3.test')];
  }
}

class LogListReaderForAppWeb implements LogListReader {
  // Fetches data from the https website. Causes CORS issues.
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
        if (name.endsWith('.csv')) {
          return [name, e.children[0].attributes['href']!];
        }
      })
      .where((item) => item != null)
      .toList();
    
    return listItems.map((e) => Log(e![0], e[1])).toList();
  }
}

class LogListReaderForAppLocal implements LogListReader {
  // Fetches data from the local file system (logs/). To bypass CORS issue.
  @override
  Future<List<Log>> fetchList() async {
    final html = await rootBundle.loadString('logs/index.html');
    final document = parse(html);
    final listItems = document
      .getElementsByTagName('li')
      .map((e) {
        final innerHtml = e.children[0].innerHtml;
        final name = innerHtml.substring(innerHtml.lastIndexOf('/') + 1);
        if (name.endsWith('.csv')) {
          return [name, e.children[0].attributes['href']!];
        }
      })
      .where((item) => item != null)
      .toList();
    
    return listItems.map((e) => Log(e![0], e[1])).toList();
  }
}
