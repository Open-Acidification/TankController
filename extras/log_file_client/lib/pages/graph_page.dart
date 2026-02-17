import 'package:flutter/material.dart';
import 'package:log_file_client/components/graph_view.dart';
import 'package:log_file_client/utils/http_client.dart';

class GraphPage extends StatelessWidget {
  const GraphPage({
    required this.log,
    required this.httpClient,
    this.now,
    super.key,
  });

  final DateTime? now;
  final HttpClient httpClient;
  final Log log;

  Future<List<LogDataLine>> getLogData() async {
    final table = await httpClient.getLogData('/logs/${log.uri}');

    return table;
  }

  @override
  Widget build(BuildContext context) {
    // ignore: discarded_futures
    final Future<List<LogDataLine>> logData = getLogData();

    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Tank Monitor'),
      ),
      body: Center(
        child: FutureBuilder(
          future: logData,
          builder: (context, snapshot) {
            if (snapshot.connectionState == ConnectionState.waiting) {
              return Padding(
                padding: const EdgeInsets.only(bottom: 300),
                child: CircularProgressIndicator(),
              );
            } else if (snapshot.hasError) {
              return Center(child: Text('Error: ${snapshot.error}'));
            } else if (!snapshot.hasData || snapshot.data!.isEmpty) {
              return const Center(child: Text('No data found'));
            } else {
              final logData = snapshot.data!;
              return GraphView(logData: logData, now: now);
            }
          },
        ),
      ),
    );
  }
}
