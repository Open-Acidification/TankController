import 'dart:async';

import 'package:flutter/material.dart';
import 'package:log_file_client/components/advanced_options_dropdown.dart';
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

  final _tempDeviationController = TextEditingController(text: '0.5');
  final _pHDeviationController = TextEditingController(text: '0.5');
  double _tempDeviation = 0.5;
  double _pHDeviation = 0.5;

  @override
  void initState() {
    super.initState();
    _tempDeviationController.addListener(_onDeviationChanged);
    _pHDeviationController.addListener(_onDeviationChanged);
  }

  void _onDeviationChanged() {
    setState(() {
      _tempDeviation = double.tryParse(_tempDeviationController.text) ?? 0.5;
      _pHDeviation = double.tryParse(_pHDeviationController.text) ?? 0.5;
    });
  }

  @override
  void dispose() {
    _tempDeviationController.removeListener(_onDeviationChanged);
    _pHDeviationController.removeListener(_onDeviationChanged);
    _tempDeviationController.dispose();
    _pHDeviationController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final screenWidth = MediaQuery.of(context).size.width;
    final int gridCrossAxis = screenWidth > 1024
        ? 3
        : screenWidth > 500
            ? 2
            : 1;
    final double sideMargins = screenWidth > 500 ? 100 : 10;

    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Tank Monitor'),
        actions: [
          AdvancedOptionsDropdown(
            tempController: _tempDeviationController,
            phController: _pHDeviationController,
          ),
        ],
      ),
      body: Center(
        child: Column(
          children: [
            PageHeader(text: '${widget.project.name} Tanks'),
            _tankCards(gridCrossAxis, sideMargins),
          ],
        ),
      ),
    );
  }

  Expanded _tankCards(int gridCrossAxis, double sideMargins) {
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
            tempDeviation: _tempDeviation,
            pHDeviation: _pHDeviation,
            onTap: () => unawaited(openTankGraph(widget.project.logs[index])),
          );
        },
        padding: EdgeInsets.only(
          left: sideMargins,
          right: sideMargins,
          top: 16,
        ),
      ),
    );
  }
}
