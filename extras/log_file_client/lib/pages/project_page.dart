import 'dart:async';

import 'package:flutter/material.dart';
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
    final gridCrossAxis = screenWidth > 750 ? 3 : 2;

    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Tank Monitor'),
      ),
      body: Center(
        child: Column(
          children: [
            _pageHeader(screenWidth),
            _tankCards(gridCrossAxis, screenWidth),
          ],
        ),
      ),
    );
  }

  Container _pageHeader(double screenWidth) {
    return Container(
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
    );
  }

  Expanded _tankCards(int gridCrossAxis, double screenWidth) {
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
          left: screenWidth * 0.067,
          right: screenWidth * 0.067,
          top: screenWidth * 0.011,
        ),
      ),
    );
  }
}
