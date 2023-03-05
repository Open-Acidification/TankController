import 'package:flutter/material.dart';
import 'package:tank_manager/model/app_data.dart';
import 'package:tank_manager/model/tank.dart';
import 'package:tank_manager/model/tc_interface.dart';

class AppDrawer extends StatelessWidget {
  const AppDrawer({
    Key? key,
    required this.context,
  }) : super(key: key);

  final BuildContext context;

  @override
  Widget build(BuildContext context) {
    var appData = AppData.instance;
    final ipController = TextEditingController();
    final nameController = TextEditingController();
    List<Widget> tiles = <Widget>[];
    appData.readTankList();
    for (var tank in appData.tankList) {
      tiles.add(tile(tank));
    }
    return Drawer(
      backgroundColor: Colors.grey.shade600,
      child: ListView(
        padding: EdgeInsets.zero,
        children: [
          drawerHeader(context),
          ...tiles,
          field(nameController, 'Name', 'Tank 99'),
          field(ipController, 'IP', '000.000.000.000'),
          Align(
            alignment: Alignment.topRight,
            child: FloatingActionButton(
              onPressed: () {
                var newTank = Tank(nameController.text, ipController.text);
                appData.addTank(newTank);
              },
              tooltip: 'Add Tank',
              child: const Icon(Icons.add),
            ),
          ),
          Align (
            alignment: Alignment.topRight,
            child: FloatingActionButton(
              onPressed: () {
                appData.removeTank(appData.currentTank);
                appData.currentTank = Tank("", "");
              },
              tooltip: 'Remove Tank',
              child: const Icon(Icons.delete_sharp)
            ),
          ),
        ],
      ),
    );
  }

  Widget drawerHeader(BuildContext context) {
    return Container(
      color: Colors.blue,
      child: DrawerHeader(
        child: Image.asset(
          'lib/assets/oap.png',
        ),
      ),
    );
  }

  Widget field(var controller, var label, var hint) {
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

  Widget tile(var selected) {
    var appData = AppData.instance;
    var tcInterface = TcInterface.instance;
    return ListTile(
      title: Text(
        selected.name,
        style: const TextStyle(color: Colors.white),
      ),
      onTap: () {
        appData.currentTank = selected;
        tcInterface.get(appData.currentTank.ip, 'display').then((value) {
          appData.display = value;
        });
        Navigator.pop(context);
      },
    );
  }
}
