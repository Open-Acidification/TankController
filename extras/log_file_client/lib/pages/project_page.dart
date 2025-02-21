import 'dart:async';

import 'package:flutter/material.dart';
import 'package:log_file_client/components/page_header.dart';
import 'package:log_file_client/components/tank_card.dart';
import 'package:log_file_client/pages/graph_page.dart';
import 'package:log_file_client/utils/http_client.dart';

class ProjectPage extends StatefulWidget {
  const ProjectPage({
    required this.project,
    required this.httpClient,
    super.key,
  });

  final HttpClient httpClient;
  final Project project;

  @override
  State<ProjectPage> createState() => _ProjectPageState();
}

class _ProjectPageState extends State<ProjectPage> {
  Future<void> openTankGraph(Log log) async {
    await Navigator.of(context).push(
      PageRouteBuilder(
        pageBuilder: (context, animation1, animation2) => GraphPage(
          log: log,
          httpClient: widget.httpClient,
        ),
        transitionDuration: Duration.zero,
        reverseTransitionDuration: Duration.zero,
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final screenWidth = MediaQuery.of(context).size.width;
    final int gridCrossAxis;
    if (screenWidth > 1370) {
      gridCrossAxis = 3;
    } else if (screenWidth > 980) {
      gridCrossAxis = 2;
    } else {
      gridCrossAxis = 1;
    }

    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Tank Monitor'),
      ),
      body: Center(
        child: Column(
          children: [
            PageHeader(text: '${widget.project.name} Tanks'),
            _tankCards(gridCrossAxis),
          ],
        ),
      ),
    );
  }

  Expanded _tankCards(int gridCrossAxis) {
    return Expanded(
      child: GridView.builder(
        gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
          crossAxisCount: gridCrossAxis,
        ),
        itemCount: widget.project.logs.length,
        itemBuilder: (context, index) {
          return TankCard(
            log: widget.project.logs[index],
            httpClient: widget.httpClient,
            onTap: () => unawaited(openTankGraph(widget.project.logs[index])),
          );
        },
        padding: EdgeInsets.only(
          left: 100,
          right: 100,
          top: 16,
        ),
      ),
    );
  }
}
