import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:tank_manager/model/app_data.dart';
import 'package:tank_manager/model/tc_interface.dart';

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
                  !showEdit(key.toString())
                      ? DataCell(Text(value.toString()))
                      : DataCell(
                          Text(value.toString()),
                          showEditIcon: true,
                          onTap: () async {
                            await showDialog(
                              context: context,
                              builder: (BuildContext context) {
                                return AlertDialog(
                                  title: Text('Edit ${key.toString()}'),
                                  content: Padding(
                                    padding: const EdgeInsets.all(8.0),
                                    child: Form(
                                      child: Column(
                                        mainAxisSize: MainAxisSize.min,
                                        children: <Widget>[
                                          TextFormField(
                                            initialValue: value.toString(),
                                            onFieldSubmitted: (val) {
                                              print(
                                                  '${appData.information["IPAddress"]} ${key.toString()}=$val');
                                              TcInterface.instance
                                                  .put(
                                                '${appData.information["IPAddress"]}',
                                                '${key.toString()}=$val',
                                              )
                                                  .then((value) {
                                                appData.information = value;
                                              });
                                              Navigator.pop(context);
                                            },
                                          ),

                                          const Text(
                                            '',
                                          ), //Placeholder for spacing until I find the correct way
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
                          },
                        )
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
              )
            ],
          );
        },
      ),
    );
  }
}
