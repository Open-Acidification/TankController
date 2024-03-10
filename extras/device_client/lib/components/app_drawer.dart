import 'dart:async';

import 'package:device_client/model/app_data.dart';
import 'package:device_client/model/tank.dart';
import 'package:device_client/model/tc_interface.dart';
import 'package:device_client/model/version.dart';
import 'package:flutter/material.dart';

class AppDrawer extends StatelessWidget {
  const AppDrawer({
    required this.context,
    super.key,
  });

  final BuildContext context;

  Future<void> showAlertDialog(String message, BuildContext context) async {
    // set up the button
    final Widget okButton = TextButton(
      child: const Text('OK'),
      onPressed: () {
        Navigator.pop(context);
      },
    );

    // set up the AlertDialog
    final AlertDialog alert = AlertDialog(
      title: Text(message),
      content: const Text(
        'Error connecting to Tank Controller. This is likely due to an incorrect IP address.',
      ),
      actions: [
        okButton,
      ],
    );

    // show the dialog
    await showDialog(
      context: context,
      builder: (BuildContext context) {
        return alert;
      },
    );
  }

  Widget addTankButton(
    TextEditingController nameController,
    TextEditingController ipController,
    AppData appData,
  ) {
    return Align(
      alignment: Alignment.topRight,
      child: FloatingActionButton(
        onPressed: () async {
          final newTank = Tank(nameController.text, ipController.text);
          try {
            await appData.addTank(newTank);
          } catch (e) {
            if (context.mounted) {
              await showAlertDialog(e.runtimeType.toString(), context);
            }
          }
        },
        tooltip: 'Add Tank',
        child: const Icon(Icons.add),
      ),
    );
  }

  Widget removeTankButton(
    TextEditingController nameController,
    TextEditingController ipController,
    AppData appData,
  ) {
    return Align(
      alignment: Alignment.topRight,
      child: FloatingActionButton(
        onPressed: () async {
          await appData.removeTank(appData.currentTank);
          appData.clearTank();
        },
        tooltip: 'Remove Tank',
        child: const Icon(Icons.delete_sharp),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final appData = AppData.instance();
    final ipController = TextEditingController();
    final nameController = TextEditingController();
    final List<Widget> tiles = <Widget>[];
    for (final tank in appData.tankList) {
      tiles.add(tile(tank));
    }
    return Drawer(
      backgroundColor: Colors.grey.shade600,
      child: Column(
        children: <Widget>[
          Expanded(
            child: ListView(
              padding: EdgeInsets.zero,
              children: <Widget>[
                drawerHeader(context),
                ...tiles,
                field(nameController, 'Name', 'Tank 99'),
                field(ipController, 'IP', '000.000.000.000'),
                addTankButton(nameController, ipController, appData),
                removeTankButton(nameController, ipController, appData),
              ],
            ),
          ),
          const Align(
            alignment: FractionalOffset.bottomCenter,
            child: Text(gitVersion),
          ),
        ],
      ),
    );
  }

  Widget drawerHeader(BuildContext context) {
    return ColoredBox(
      color: Colors.blue,
      child: DrawerHeader(
        child: Image.asset(
          'lib/assets/oap.png',
        ),
      ),
    );
  }

  Widget field(TextEditingController controller, String label, String hint) {
    return TextField(
      controller: controller,
      style: const TextStyle(color: Colors.black),
      decoration: InputDecoration(
        border: const OutlineInputBorder(),
        labelText: label,
        fillColor: Colors.grey.shade100,
        filled: true,
        hintText: hint,
      ),
    );
  }

  Future<void> updateDisplay() async {
    final appData = AppData.instance();
    final tcInterface = TcInterface.instance();
    final String value =
        await tcInterface.get(appData.currentTank.ip, 'display');
    appData.display = value; // setter notifies listeners of change
  }

  Widget tile(Tank selected) {
    return ListTile(
      title: Text(
        selected.name,
        style: const TextStyle(color: Colors.white),
      ),
      onTap: () {
        AppData.instance().currentTank = selected;
        unawaited(updateDisplay());
        Navigator.pop(context);
      },
    );
  }
}
