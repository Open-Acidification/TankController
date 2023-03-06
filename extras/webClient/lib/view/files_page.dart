import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:tank_manager/model/app_data.dart';

class Files extends StatelessWidget {
  const Files({
    Key? key,
    required this.context,
  }) : super(key: key);

  final BuildContext context;

  @override
  Widget build(BuildContext context) {
    return Container(
      color: Colors.white,
      child: Consumer<AppData>(
        builder: (context, appData, child) {
          List<DataRow> fileRows = <DataRow>[];
          appData.files.forEach(
            (fileName, fileSize) => fileRows.add(
              DataRow(
                cells: <DataCell>[
                  DataCell(Text(fileName.toString())),
                  DataCell(Container(
                    alignment: const Alignment(1.0, 0.0),
                    child: Text(fileSize.toString().trim()),
                  )),
                ],
              ),
            ),
          );
          fileRows.sort(
            (a, b) => a.cells[0].child
                .toString()
                .compareTo(b.cells[0].child.toString()),
          );
          return ListView(children: <Widget>[
            DataTable(
              headingRowHeight: 0,
              columns: const <DataColumn>[
                DataColumn(label: Text('File Name')),
                DataColumn(label: Text('File Size')),
              ],
              rows: fileRows,
            )
          ]);
        },
      ),
    );
  }
}
