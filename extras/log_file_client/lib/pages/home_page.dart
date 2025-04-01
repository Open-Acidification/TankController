import 'dart:async';

import 'package:flutter/material.dart';
import 'package:log_file_client/components/page_header.dart';
import 'package:log_file_client/components/project_card.dart';
import 'package:log_file_client/pages/project_page.dart';
import 'package:log_file_client/utils/http_client.dart';
import 'package:skeletonizer/skeletonizer.dart';
// import 'package:web/web.dart' as html;
// TODO: Figure out how to link to website without having interference with Flutter widget tests

class HomePage extends StatefulWidget {
  const HomePage({super.key, this.httpClient});

  final HttpClient? httpClient;

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  late final HttpClient httpClient;
  List<Project>? _projectList;
  final List<Project> _mockProjectList = [
    Project('mock', []),
    Project('mock', []),
    Project('mock', []),
  ];
  bool _isLoading = true;

  @override
  void initState() {
    super.initState();
    httpClient = widget.httpClient ?? HttpClientProd();
    unawaited(_getProjectList());
  }

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
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Tank Monitor'),
        // actions: [
        //   _tankManagerRedirect(),
        // ],
      ),
      body: Center(
        child: Column(
          children: [
            PageHeader(text: 'Projects'),
            _projectCards(),
          ],
        ),
      ),
    );
  }

  // Padding _tankManagerRedirect() {
  //   return Padding(
  //     padding: const EdgeInsets.only(right: 8.0),
  //     child: TextButton(
  //       onPressed: () {
  //         const tankManagerUrl = 'http://oap.cs.wallawalla.edu/';
  //         html.window.open(tankManagerUrl, '_self');
  //       },
  //       child: Row(
  //         mainAxisSize: MainAxisSize.min,
  //         children: const [
  //           Text(
  //             'Go to Tank Manager',
  //             style: TextStyle(fontWeight: FontWeight.bold),
  //           ),
  //           SizedBox(width: 8),
  //           Icon(Icons.swap_horiz),
  //         ],
  //       ),
  //     ),
  //   );
  // }

  Widget _projectCards() {
    final screenWidth = MediaQuery.of(context).size.width;
    final int gridCrossAxis = screenWidth > 800 ? 3 : screenWidth > 500 ? 2 : 1;
    final double sideMargins = screenWidth > 500 ? 100 : 40;

    return _isLoading
        ? _skeletonLoader(gridCrossAxis, sideMargins)
        : Expanded(
            child: GridView.builder(
              gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
                crossAxisCount: gridCrossAxis,
              ),
              itemCount: _projectList?.length ?? 0,
              itemBuilder: (context, index) {
                return ProjectCard(
                  project: _projectList![index],
                  onTap: () => unawaited(openProject(_projectList![index])),
                );
              },
              padding: _gridViewPadding(sideMargins),
            ),
          );
  }

  Widget _skeletonLoader(int gridCrossAxis, double sideMargins) {
    return Expanded(
      child: Skeletonizer(
        effect: ShimmerEffect(
          baseColor: Colors.grey[300]!,
          highlightColor: Colors.grey[100]!,
          duration: Duration(seconds: 2),
        ),
        child: GridView.builder(
          gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
            crossAxisCount: gridCrossAxis,
          ),
          itemCount: _mockProjectList.length,
          itemBuilder: (context, index) {
            return ProjectCard(
              project: _mockProjectList[index],
              onTap: () {},
            );
          },
          padding: _gridViewPadding(sideMargins),
        ),
      ),
    );
  }

  EdgeInsets _gridViewPadding(sideMargins) {
    return EdgeInsets.only(
      left: sideMargins,
      right: sideMargins,
      top: 16,
    );
  }
}
