import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';
import 'dart:html' as html;
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:tank_manager/model/app_data.dart';

class Information extends StatelessWidget {
  const Information({
    Key? key,
    required this.context,
  }) : super(key: key);

  final BuildContext context;
  List<int> _selectedFile;
  Uint8List _bytesData;

  // void _handleResult(Object result) {
  //   setState(())
  // }

  // startWebFilePicker() async {
  //   html.FileUploadInputElement uploadInput = html.FileUploadInputElement();
  //   uploadInput.multiple = true;
  //   uploadInput.draggable = true;
  //   uploadInput.click();
  //   uploadInput.onChange.listen((e) { 
  //     final files = uploadInput.files;
  //     final file = files[0];
  //     final reader = new html.FileReader();
  //   });
  // }

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
          return Column(
            children: [
              Expanded(
                child: ListView(
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
                  ],
                ),
              ),
              OutlinedButton(
                onPressed: () {
                  startWebFilePicker();
                },
                child: const Text('Add File'),
              )
            ],
          );
        },
      ),
    );
  }
}
