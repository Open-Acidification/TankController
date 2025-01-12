import 'package:flutter/material.dart';
import 'package:log_file_client/components/graph_view.dart';
import 'package:log_file_client/utils/http_client.dart';

class GraphPage extends StatefulWidget {
  const GraphPage({required this.log, super.key, this.httpClient});

  final HttpClient? httpClient;
  final Log log;

  @override
  State<GraphPage> createState() => _GraphPageState();
}

class _GraphPageState extends State<GraphPage> {
  late final HttpClient httpClient;

  @override
  void initState() {
    super.initState();
    httpClient = widget.httpClient ?? HttpClientProd();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Tank Monitor'),
      ),
      body: Center(
        child: GraphView(
                    filePath: '/logs${widget.log.uri}',
                    httpClient: httpClient,
                  ),
      ),
    );
  }
}
