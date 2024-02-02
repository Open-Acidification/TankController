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
    required this.context,
    Key? key,
  }) : super(key: key);

  final BuildContext context;

  bool canEditCurrentInfo(AppData appData) {
    print('appData.currentData = ${appData.currentData}');
    print(
        'appData.currentData.runtimeType = ${appData.currentData.runtimeType}');
    print('appData.currentData["Version"] = ${appData.currentData['Version']}');
    String v = appData.currentData['Version'].trim();
    final i = v.indexOf('-');
    if (i != -1) {
      v = v.substring(0, i);
    }
    final Version latestVersion = Version.parse(v);
    return latestVersion >= Version.parse('23.6.0');
  }

  bool canUploadFile(AppData appData) {
    String v = appData.currentData['Version'].trim();
    final i = v.indexOf('-');
    if (i != -1) {
      v = v.substring(0, i);
    }
    final Version latestVersion = Version.parse(v);
    return latestVersion >= Version.parse('99.9.9'); // not supported yet!
  }

  Future<void> showEditDialog(
    AppData appData,
    BuildContext context,
    String key,
    String value,
  ) async {
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
                  if (key == 'Target_pH_type')
                    DropdownButtonFormField<String>(
                      value: value,
                      items: const <DropdownMenuItem<String>>[
                        DropdownMenuItem(
                          value: '1',
                          child: Text('Flat'),
                        ),
                        DropdownMenuItem(
                          value: '2',
                          child: Text('Ramp'),
                        ),
                        DropdownMenuItem(
                          value: '3',
                          child: Text('Sine'),
                        ),
                      ],
                      onChanged: (String? newValue) {
                        TcInterface.instance()
                            .put(
                          '${appData.currentData["IPAddress"]}',
                          'data?$key=$newValue',
                        )
                            .then((value) {
                          appData.currentData = json.decode(value);
                        });
                        Navigator.pop(context);
                      },
                    )
                  else
                    TextFormField(
                      initialValue: value,
                      onFieldSubmitted: (val) {
                        TcInterface.instance()
                            .put(
                          '${appData.currentData["IPAddress"]}',
                          'data?$key=$val',
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

  Future<void> showPopupDialog(String titleString, String messageString) async {
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

  Future<void> sendArbitraryPathString(String arbitraryPath, String ip) async {
    final Uint8List bytes =
        const Base64Decoder().convert(arbitraryPath.split(',').last);
    if (bytes.length > 10000) {
      await showPopupDialog('File too large', 'Your file exceeds 10 KB.');
      throw UnsupportedError('File too large');
    }
    await postArbitraryPathAsFile(ip, bytes);
  }

  Future<String?> postArbitraryPathAsFile(String ip, Uint8List bytes) async {
    final uri = Uri.parse(ip);
    final request = http.MultipartRequest('POST', uri);
    request.files.add(
      http.MultipartFile.fromBytes(
        'file',
        bytes,
        contentType: MediaType('application', 'octet-stream'),
        filename: 'arbitraryPath.txt',
      ),
    );
    final res = await request.send();
    return res.reasonPhrase;
  }

  Future<void> selectFileToUpload(String ip) async {
    final html.FileUploadInputElement uploadInput =
        html.FileUploadInputElement();
    uploadInput.multiple = true;
    uploadInput.draggable = true;
    uploadInput.accept = '.txt,.csv';
    uploadInput.click();
    uploadInput.onChange.listen((e) {
      final files = uploadInput.files;
      final file = files![0];
      final dynamic reader = html.FileReader();
      reader.onLoadEnd.listen((e) {
        sendArbitraryPathString(reader.result as String, ip);
      });
      reader.readAsDataUrl(file);
    });
  }

  @override
  Widget build(BuildContext context) {
    print(
        'CurrentData.build found appData.currentData = ${context.read<AppData>().currentData}');
    return ColoredBox(
      color: Colors.white,
      child: Consumer<AppData>(
        builder: (context, appData, child) {
          final currentDataRows = <DataRow>[];
          final currentData = appData.currentData;
          // if (currentData.isEmpty) {
          //   return Container();
          // }
          final editableFields = currentData['EditableFields'];
          print('editableFields = $editableFields');
          currentData.remove('EditableFields');
          final canEdit = canEditCurrentInfo(appData);
          currentData.forEach(
            (key, value) => currentDataRows.add(
              DataRow(
                cells: <DataCell>[
                  DataCell(Text(key.toString())),
                  (!editableFields.contains(key.toString()) || !canEdit)
                      ? DataCell(Text(value.toString()))
                      : DataCell(
                          Text(value.toString()),
                          showEditIcon: true,
                          onTap: () async {
                            await showEditDialog(
                              appData,
                              context,
                              key,
                              value.toString(),
                            );
                          },
                        ),
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
                          : () async {
                              await showPopupDialog(
                                'Feature coming soon',
                                'Your tank controller is not at a version that supports file upload',
                              );
                            },
                      child: const Text('Upload file for arbitrary path'),
                    )
                  : OutlinedButton(
                      onPressed: () {},
                      child: const Text('Upload file for arbitrary path'),
                    ),
            ],
          );
        },
      ),
    );
  }
}
