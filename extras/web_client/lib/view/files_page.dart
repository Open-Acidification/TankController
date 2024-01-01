import 'dart:async';

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:tank_manager/model/app_data.dart';
import 'package:url_launcher/url_launcher.dart';

class Files extends StatelessWidget {
  const Files({
    required this.context,
    Key? key,
  }) : super(key: key);

  final BuildContext context;

  @override
  Widget build(BuildContext context) {
    return ColoredBox(
      color: Colors.white,
      child: Consumer<AppData>(
        builder: (context, appData, child) {
          final List<DataRow> fileRows = <DataRow>[];
          appData.files.forEach(
            (fileName, fileSize) => fileRows.add(
              DataRow(
                cells: <DataCell>[
                  DataCell(
                    GestureDetector(
                      onTap: () {
                        final String ip = appData.currentTank.ip;
                        final String file = fileName.toString();
                        final String loc = 'http://$ip/$file';
                        unawaited(launchUrl(Uri.parse(loc)));
                      },
                      child: Text(
                        fileName.toString(),
                      ),
                    ),
                  ),
                  DataCell(
                    Container(
                      alignment: const Alignment(1.0, 0.0),
                      child: Text(fileSize.toString().trim()),
                    ),
                  ),
                ],
              ),
            ),
          );
          fileRows.sort(
            (a, b) => a.cells[0].child
                .toString()
                .compareTo(b.cells[0].child.toString()),
          );
          return ListView(
            children: <Widget>[
              DataTable(
                headingRowHeight: 0,
                columns: const <DataColumn>[
                  DataColumn(label: Text('File Name')),
                  DataColumn(label: Text('File Size')),
                ],
                rows: fileRows,
              ),
            ],
          );
        },
      ),
    );
  }
}
