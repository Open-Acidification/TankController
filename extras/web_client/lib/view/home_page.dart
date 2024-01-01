import 'package:flutter/material.dart';
import 'package:tank_manager/components/app_drawer.dart';
import 'package:tank_manager/components/display.dart';
import 'package:tank_manager/components/nav_bar.dart';
import 'package:provider/provider.dart';
import 'package:tank_manager/model/app_data.dart';
import 'package:tank_manager/components/keypad.dart';
import 'package:tank_manager/view/current_data_page.dart';
import 'package:tank_manager/view/files_page.dart';

class MyHomePage extends StatefulWidget {
  const MyHomePage({required this.title, Key? key}) : super(key: key);

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  Widget appBody(BuildContext context) {
    return Center(
      child: Column(
        children: <Widget>[
          Display(context: context),
          Flexible(
            child: Keypad(context: context),
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final List<Widget> children = [
      appBody(context),
      CurrentData(context: context),
      Files(context: context),
    ];
    return Consumer<AppData>(
      builder: (context, appData, child) {
        return Scaffold(
          appBar: AppBar(
            title: Text(
              '${widget.title}: ${appData.currentTank.name}',
            ),
          ),
          drawer: AppDrawer(context: context),
          body: children[appData.currentIndex],
          bottomNavigationBar: NavBar(context: context),
        );
      },
    );
  }
}
