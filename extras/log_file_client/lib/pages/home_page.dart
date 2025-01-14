import 'dart:async';

import 'package:flutter/material.dart';
import 'package:log_file_client/components/project_card.dart';
import 'package:log_file_client/pages/project_page.dart';
import 'package:log_file_client/utils/http_client.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key, this.httpClient});

  final HttpClient? httpClient;

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  late final HttpClient httpClient;
  List<Project>? _projectList;
  bool _isLoading = true;

  @override
  void initState() {
    super.initState();
    httpClient = widget.httpClient ?? HttpClientProd();
    unawaited(_getProjectList());
  }

  // Fetches the list of projects available
  Future<void> _getProjectList() async {
    final result = await httpClient.getProjectList();
    setState(() {
      _projectList = result;
      _isLoading = false;
    });
  }

  Future<void> openProject(Project project) async {
    await Navigator.of(context).push(
      PageRouteBuilder(
        pageBuilder: (context, animation1, animation2) => ProjectPage(
          project: project,
          httpClient: httpClient,
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
                      'Projects',
                      style: TextStyle(
                        fontSize: screenWidth * 0.04,
                        letterSpacing: -2,
                        color: const Color(0xFF0C2D48),
                      ),
                    ),
                  ),
                  Expanded(
                    child: GridView.builder(
                      gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
                        crossAxisCount: gridCrossAxis,
                      ),
                      itemCount: _projectList?.length ?? 0,
                      itemBuilder: (context, index) {
                        return ProjectCard(
                          project: _projectList![index],
                          onTap: () =>
                              unawaited(openProject(_projectList![index])),
                        );
                      },
                      padding: EdgeInsets.only(
                        left: screenWidth * 0.067,
                        right: screenWidth * 0.067,
                        top: screenWidth * 0.011,
                      ),
                    ),
                  ),
                ],
              ),
      ),
    );
  }
}
