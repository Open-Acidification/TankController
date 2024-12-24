import 'dart:async';

import 'package:flutter/material.dart';
import 'package:log_file_client/components/app_drawer.dart';
import 'package:log_file_client/components/graph_view.dart';
import 'package:log_file_client/utils/http_client.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key, this.httpClient});

  final HttpClient? httpClient;

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  late final HttpClient httpClient;
  List<Log>? _logList;
  bool _isLoading = true;
  int _openedLogIndex = -1;

  @override
  void initState() {
    super.initState();
    httpClient = widget.httpClient ?? HttpClientProd();
    unawaited(_getLogList());
  }

  // Fetches the list of csv files available
  Future<void> _getLogList() async {
    final result = await httpClient.getLogList();
    setState(() {
      _logList = result;
      _isLoading = false;
    });
  }

  void openLogFile(Log log) {
    setState(() {
      _openedLogIndex = _logList!.indexOf(log); // Open the selected log file
      Navigator.of(context).pop(); // Close the drawer
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Tank Monitor'),
      ),
      drawer: _isLoading
          ? Drawer() // null drawer
          : AppDrawer(
              logList: _logList!,
              openLogFile: openLogFile,
            ),
      body: Center(
        child: _isLoading
            ? const CircularProgressIndicator()
            : _openedLogIndex >= 0
                ? GraphView(
                    csvPath: _logList![_openedLogIndex].uri,
                    httpClient: httpClient,
                  )
                : null,
      ),
    );
  }
}
