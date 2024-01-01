import 'dart:async';
import 'package:flutter/material.dart';
import 'package:tank_manager/model/tc_interface.dart';
import 'package:tank_manager/model/app_data.dart';

class Keypad extends StatelessWidget {
  const Keypad({
    required this.context,
    Key? key,
  }) : super(key: key);

  final BuildContext context;

  @override
  Widget build(BuildContext context) {
    return Container(
      decoration: BoxDecoration(
        borderRadius: const BorderRadius.all(Radius.circular(20)),
        color: Colors.grey.shade800,
        border: Border.all(width: 3.0, color: Colors.white),
      ),
      height: 415,
      width: 415,
      child: GridView.count(
        primary: false,
        padding: const EdgeInsets.all(20),
        crossAxisSpacing: 10,
        mainAxisSpacing: 10,
        crossAxisCount: 4,
        children: buttons(context),
      ),
    );
  }

  List<Widget> buttons(BuildContext context) {
    return <Widget>[
      button(context, '1', Colors.blue),
      button(context, '2', Colors.blue),
      button(context, '3', Colors.blue),
      button(context, 'A', Colors.red),
      button(context, '4', Colors.blue),
      button(context, '5', Colors.blue),
      button(context, '6', Colors.blue),
      button(context, 'B', Colors.red),
      button(context, '7', Colors.blue),
      button(context, '8', Colors.blue),
      button(context, '9', Colors.blue),
      button(context, 'C', Colors.red),
      button(context, '*', Colors.red),
      button(context, '0', Colors.blue),
      button(context, '#', Colors.red),
      button(context, 'D', Colors.red),
    ];
  }

  Future<void> sendKeypress(String ip, String path) async {
    final String value = await TcInterface.instance().post(ip, path);
    AppData.instance().display = value;
  }

  Widget button(BuildContext context, String label, Color color) {
    final appData = AppData.instance();
    return Container(
      padding: const EdgeInsets.all(8),
      decoration: BoxDecoration(
        color: color,
        border: Border.all(width: 5, color: Colors.white),
        borderRadius: const BorderRadius.all(Radius.circular(20)),
      ),
      child: TextButton(
        style: TextButton.styleFrom(
          textStyle: const TextStyle(fontSize: 40),
          foregroundColor: Colors.white,
        ),
        onPressed: () {
          if (appData.currentTank.isNotEmpty()) {
            unawaited(sendKeypress(appData.currentTank.ip, 'key?value=$label'));
          }
        },
        child: Text(label),
      ),
    );
  }
}
