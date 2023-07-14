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

class CurrentData extends StatelessWidget {
  const CurrentData({
    Key? key,
    required this.context,
  }) : super(key: key);

  final BuildContext context;

  bool showEdit(var valueString) {
    return valueString == 'Kp' || valueString == 'Ki' || valueString == 'Kd';
  }

  bool canEditCurrentInfo(AppData appData) {
    String v = appData.currentData['Version'].trim();
    Version latestVersion = Version.parse(v);
    return latestVersion >= Version.parse('23.6.0');
  }

  bool canUploadFile(AppData appData) {
    String v = appData.currentData['Version'].trim();
    Version latestVersion = Version.parse(v);
    return latestVersion >= Version.parse('99.9.9'); // not supported yet!
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
                        '${appData.currentData["IPAddress"]}',
                        'data?${key.toString()}=$val',
                      )
                          .then((value) {
                        appData.currentData = json.decode(value);
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

  void sendArbitraryPathString(String arbitraryPath, String ip) async {
    Uint8List bytes =
        const Base64Decoder().convert(arbitraryPath.split(',').last);
    if (bytes.length > 10000) {
      throw UnsupportedError(
        showPopupDialog('File too large', 'Your file exceeds 10 KB.'),
      );
    }
    await postArbitraryPathAsFile(ip, bytes);
  }

  Future<String?> postArbitraryPathAsFile(String ip, Uint8List bytes) async {
    var uri = Uri.parse(ip);
    var request = http.MultipartRequest('POST', uri);
    request.files.add(
      http.MultipartFile.fromBytes(
        'file',
        bytes,
        contentType: MediaType('application', 'octet-stream'),
        filename: 'arbitraryPath.txt',
      ),
    );
    var res = await request.send();
    return res.reasonPhrase;
  }

  selectFileToUpload(String ip) async {
    html.FileUploadInputElement uploadInput = html.FileUploadInputElement();
    uploadInput.multiple = true;
    uploadInput.draggable = true;
    uploadInput.accept = '.txt,.csv';
    uploadInput.click();
    uploadInput.onChange.listen((e) {
      final files = uploadInput.files;
      final file = files![0];
      dynamic reader = html.FileReader();
      reader.onLoadEnd.listen((e) {
        sendArbitraryPathString(reader.result as String, ip);
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
          var currentDataRows = <DataRow>[];
          appData.currentData.forEach(
            (key, value) => currentDataRows.add(
              DataRow(
                cells: <DataCell>[
                  DataCell(Text(key.toString())),
                  (!showEdit(key.toString()) || !canEditCurrentInfo(appData))
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
                      rows: currentDataRows,
                    ),
                  ],
                ),
              ),
              // This outer ternary is required to prevent the screen flashing an error
              // for a split second while appData.currentData loads to do the check.
              (appData.currentData['Version'] != null)
                  ? OutlinedButton(
                      onPressed: canUploadFile(appData)
                          ? () {
                              unawaited(
                                selectFileToUpload(
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
