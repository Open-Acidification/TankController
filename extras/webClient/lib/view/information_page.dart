import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';
import 'package:tank_manager/view/mock_html.dart'
    if (dart.library.html) 'dart:html' as html;
import 'package:http/http.dart' as http;
import 'package:http_parser/http_parser.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:tank_manager/model/app_data.dart';
import 'package:tank_manager/model/tc_interface.dart';
import 'package:version/version.dart';

class Information extends StatelessWidget {
  const Information({
    Key? key,
    required this.context,
  }) : super(key: key);

  final BuildContext context;

  bool showEdit(var valueString) {
    return valueString == 'PHSlope' ||
        valueString == 'Kp' ||
        valueString == 'Ki' ||
        valueString == 'Kd';
  }

  bool versionCheck(var versionValue) {
    Version latestVersion = Version.parse(versionValue);
    return latestVersion > Version.parse('23.3.0');
  }

  showEditDialog(var appData, BuildContext context, var key, var value) async {
    await showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: Text('Submit new ${key.toString()} value'),
          content: Padding(
            padding: const EdgeInsets.all(8.0),
            child: Form(
              child: Column(
                mainAxisSize: MainAxisSize.min,
                children: <Widget>[
                  TextFormField(
                    initialValue: value.toString(),
                    onFieldSubmitted: (val) {
                      TcInterface.instance
                          .put(
                        '${appData.information["IPAddress"]}',
                        'set?${key.toString()}=$val',
                      )
                          .then((value) {
                        appData.information = value;
                      });
                      Navigator.pop(context);
                    },
                  ),
                  const SizedBox(height: 20),
                  const Text(
                    'Press "Esc" to cancel, or "Enter" to submit',
                  ),
                ],
              ),
            ),
          ),
        );
      },
    );
  }

  showPopupDialog(String titleString, String messageString) async {
    await showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: Text(titleString),
          content: Padding(
            padding: const EdgeInsets.all(8.0),
            child: Form(
              child: Column(
                mainAxisSize: MainAxisSize.min,
                children: <Widget>[
                  Text(
                    messageString,
                  ),
                ],
              ),
            ),
          ),
        );
      },
    );
  }

  void handleResult(Object result, String ip) async {
    Uint8List bytesData =
        const Base64Decoder().convert(result.toString().split(',').last);
    if (bytesData.length > 10000) {
      throw UnsupportedError(
        showPopupDialog('File too large', 'Your file exceeds 10 KB.'),
      );
    }
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

  startWebFilePicker(String ip) async {
    html.FileUploadInputElement uploadInput = html.FileUploadInputElement();
    uploadInput.multiple = true;
    uploadInput.draggable = true;
    uploadInput.accept = '.rtf,.txt,.csv,';
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
                  (!showEdit(key.toString()) ||
                          !versionCheck(appData.information['Version']))
                      ? DataCell(Text(value.toString()))
                      : DataCell(
                          Text(value.toString()),
                          showEditIcon: true,
                          onTap: () async {
                            showEditDialog(appData, context, key, value);
                          },
                        )
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
              // This outer ternary is required to prevent the screen flashing an error
              // for a split second while appData.information loads to do the check.
              (appData.information['Version'] != null)
                  ? OutlinedButton(
                      onPressed: versionCheck(appData.information['Version'])
                          ? () {
                              unawaited(
                                startWebFilePicker(
                                  appData.currentTank.ip.toString(),
                                ),
                              );
                            }
                          : () {
                              showPopupDialog(
                                'Feature coming soon',
                                'Your tank controller is not at a version that supports file upload',
                              );
                            },
                      child: const Text('Upload file for arbitrary path'),
                    )
                  : OutlinedButton(
                      onPressed: () {},
                      child: const Text('Upload file for arbitrary path'),
                    )
            ],
          );
        },
      ),
    );
  }
}
