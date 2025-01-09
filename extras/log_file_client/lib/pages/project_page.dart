import 'dart:async';

import 'package:flutter/material.dart';
// import 'package:log_file_client/components/app_drawer.dart';
// import 'package:log_file_client/components/graph_view.dart';
import 'package:log_file_client/utils/http_client.dart';

class ProjectPage extends StatefulWidget {
  const ProjectPage({required this.project, super.key, this.httpClient});

  final HttpClient? httpClient;
  final Project project;

  @override
  State<ProjectPage> createState() => _ProjectPageState();
}

class _ProjectPageState extends State<ProjectPage> {
  late final HttpClient httpClient;
  List<Log>? _logList;
  bool _isLoading = true;

  @override
  void initState() {
    super.initState();
    httpClient = widget.httpClient ?? HttpClientProd();
    unawaited(_getLogList());
  }

  // Fetches the list of log files available
  Future<void> _getLogList() async {
    final result = await httpClient.getLogList();
    setState(() {
      _logList = result;
      _isLoading = false;
    });
  }

  @override
  Widget build(BuildContext context) {
    final screenWidth = MediaQuery.of(context).size.width;
    final gridCrossAxis = screenWidth > 750 ? 3 : 2;

    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Tank Monitor'),
      ),
      // drawer: _isLoading
      //     ? Drawer() // null drawer
      //     : AppDrawer(
      //         logList: _logList!,
      //         openLogFile: openLogFile,
      //       ),
      body: Center(
        child: _isLoading
            ? const CircularProgressIndicator()
            : Column(
                children: [
                  Container(
                    margin: EdgeInsets.only(
                      top: screenWidth * 0.025,
                      left: screenWidth * 0.0875,
                      right: screenWidth * 0.0875,
                    ),
                    padding: EdgeInsets.only(bottom: screenWidth * 0.01),
                    width: screenWidth,
                    decoration: BoxDecoration(
                      border: Border(
                        bottom: BorderSide(
                          color: const Color(0xFFE6E6E6),
                          width: 1.5,
                        ),
                      ),
                    ),
                    child: Text(
                      '${widget.project.name} Tanks',
                      style: TextStyle(
                        fontSize: screenWidth * 0.04,
                        letterSpacing: -2,
                        color: const Color(0xFF0C2D48),
                      ),
                    ),
                  ),
                  // TODO: Implement the tank cards
                ],
              ),
      ),
    );
  }
}
