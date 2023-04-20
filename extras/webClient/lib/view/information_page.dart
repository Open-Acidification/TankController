import 'dart:convert';
import 'dart:typed_data';
import 'dart:html';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:tank_manager/model/app_data.dart';

class Information extends StatelessWidget {
  const Information({
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
          var informationRows = <DataRow>[];
          appData.information.forEach(
            (key, value) => informationRows.add(
              DataRow(
                cells: <DataCell>[
                  DataCell(Text(key.toString())),
                  DataCell(Text(value.toString()))
                ],
              ),
            ),
          );
          return ListView(
            children: <Widget>[
              DataTable(
                headingRowHeight: 0,
                columns: const <DataColumn>[
                  DataColumn(
                    label: Text('Key'),
                  ),
                  DataColumn(
                    label: Text('Value'),
                  ),
                ],
                rows: informationRows,
              ),
              // Container(
              //   child: TextButton,
              // )
            ],
          );
        },
      ),
    );
  }
}
