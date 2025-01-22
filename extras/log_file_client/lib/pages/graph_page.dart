import 'package:flutter/material.dart';
import 'package:log_file_client/components/graph_view.dart';
import 'package:log_file_client/utils/http_client.dart';

class GraphPage extends StatelessWidget {
  const GraphPage({required this.log, required this.httpClient, super.key});

  final HttpClient httpClient;
  final Log log;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Tank Monitor'),
      ),
      body: Center(
        child: GraphView(
          filePath: '/logs/${log.uri}',
          httpClient: httpClient,
        ),
      ),
    );
  }
}
