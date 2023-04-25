import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';
import 'dart:html' as html;
import 'package:http/http.dart' as http;
import 'package:http_parser/http_parser.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:tank_manager/model/app_data.dart';

class Information extends StatelessWidget {
  const Information({
    Key? key,
    required this.context,
  }) : super(key: key);

  final BuildContext context;

  void handleResult(Object result, String ip) async {
    Uint8List bytesData =
        const Base64Decoder().convert(result.toString().split(',').last);
    List<int> selectedFile = bytesData;
    await makeRequest(ip, selectedFile);
  }

  Future<String?> makeRequest(String ip, List<int> selectedFile) async {
    var url = Uri.parse(ip);
    var request = http.MultipartRequest('POST', url);
    request.files.add(
      http.MultipartFile.fromBytes(
        'file',
        selectedFile,
        contentType: MediaType('application', 'octet-stream'),
        filename: 'file_up',
      ),
    );
    var res = await request.send();
    return res.reasonPhrase;
    }
    //Can I copy Dialog box frame from Shortcuts?
  }

  startWebFilePicker(String ip) async {
    html.FileUploadInputElement uploadInput = html.FileUploadInputElement();
    uploadInput.multiple = true;
    uploadInput.draggable = true;
    uploadInput.click();
    uploadInput.onChange.listen((e) {
      final files = uploadInput.files;
      final file = files![0];
      dynamic reader = html.FileReader();
      reader.onLoadEnd.listen((e) {
        handleResult(reader.result, ip);
      });
      reader.readAsDataUrl(file);
    });
  }

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
                  startWebFilePicker(appData.currentTank.ip.toString());
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
