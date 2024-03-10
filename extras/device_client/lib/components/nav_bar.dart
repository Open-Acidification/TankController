import 'dart:async';

import 'package:device_client/model/app_data.dart';
import 'package:flutter/material.dart';

class NavBar extends StatelessWidget {
  const NavBar({
    required this.context,
    super.key,
  });

  final BuildContext context;

  final items = const <BottomNavigationBarItem>[
    BottomNavigationBarItem(
      icon: Icon(Icons.apps),
      label: 'Keypad',
    ),
    BottomNavigationBarItem(
      icon: Icon(Icons.event_note_outlined),
      label: 'Current Data',
    ),
    BottomNavigationBarItem(
      icon: Icon(Icons.folder),
      label: 'Files',
    ),
    // BottomNavigationBarItem(
    //   icon: Icon(Icons.bar_chart_rounded),
    //   label: 'Graphs',
    // ),
  ];

  @override
  Widget build(BuildContext context) {
    final appData = AppData.instance();
    final int currentIndex = appData.currentIndex;
    return BottomNavigationBar(
      currentIndex: currentIndex,
      onTap: onTabTapped,
      backgroundColor: Colors.grey.shade800,
      selectedItemColor: Colors.blue,
      unselectedItemColor: Colors.white,
      items: items,
    );
  }

  void onTabTapped(int index) {
    final appData = AppData.instance();
    appData.currentIndex = index;
    if (items[index].label == 'Keypad') {
      unawaited(appData.refreshDisplay()); // this will notify listeners
    }
    if (items[index].label == 'Current Data') {
      unawaited(appData.refreshCurrentData()); // this will notify listeners
    }
    if (items[index].label == 'Files') {
      unawaited(appData.refreshFiles());
    }
  }
}
