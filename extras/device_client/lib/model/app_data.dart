import 'dart:async';
import 'dart:convert';

import 'package:device_client/model/tank.dart';
import 'package:device_client/model/tc_interface.dart';
import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

class AppData with ChangeNotifier {
  AppData() {
    _currentTank = _emptyTank;
  }
  static AppData? _instance;

  static AppData instance() {
    _instance ??= AppData();
    return _instance!;
  }

  final _emptyTank = Tank('', '');
  late Tank _currentTank;
  var _display = '';
  Map<String, dynamic> _currentData = <String, dynamic>{};
  Map<String, dynamic> _files = <String, dynamic>{};
  List<Tank> _tankList = [];
  int _currentIndex = 0;

  Future<void> readTankList() async {
    if (tankList.isNotEmpty) {
      return;
    }
    WidgetsFlutterBinding.ensureInitialized();
    final SharedPreferences prefs = await SharedPreferences.getInstance();
    if (prefs.containsKey('obj1')) {
      final String obj1 = prefs.getString('obj1')!;
      final x = jsonDecode(obj1).map((obj1) => Tank.fromJson(obj1));
      _tankList = List<Tank>.from(x);
    }
  }

  Future<void> writeTankList(List<Tank> tankList) async {
    final SharedPreferences prefs = await SharedPreferences.getInstance();
    await prefs.setString('obj1', jsonEncode(tankList));
  }

  Future<void> refreshDisplay() async {
    if (currentTank.isNotEmpty()) {
      final tcInterface = TcInterface.instance();
      display = await tcInterface.get(currentTank.ip, 'display');
    } else {
      display = '';
    }
    notifyListeners();
  }

  Future<void> refreshCurrentData() async {
    if (currentTank.isEmpty()) {
      currentData = jsonDecode('{"Error: Choose tank from menu":""}');
    } else {
      final tcInterface = TcInterface.instance();
      final value = await tcInterface.get(currentTank.ip, 'data');
      try {
        currentData = jsonDecode(value);
      } catch (e) {
        currentData = jsonDecode('{"Error: $e":""}');
      }
    }
  }

  Future<void> refreshFiles() async {
    if (currentTank.isEmpty()) {
      _files = jsonDecode('{"Error: Choose tank from menu":""}');
      return;
    }
    final tcInterface = TcInterface.instance();
    var value = '';
    // wait 15 seconds (the default of 5 seconds could be too little)
    try {
      value = await tcInterface.get(currentTank.ip, 'rootdir', 15);
      while (value.substring(value.length - 1) == '\n') {
        value = value.substring(0, value.length - 1);
      }
      value = value.replaceAll('\n', '", "');
      value = value.replaceAll('\t', '":"');
      value = '{"$value"}';
      _files = jsonDecode(value);
    } catch (e) {
      _files = jsonDecode('{"Error: $e":""}');
    }
    notifyListeners();
  }

  Future<void> addTank(Tank tank) async {
    // make a call to the device to see if it exists
    // ignore result
    await TcInterface.instance().get(tank.ip, 'data');
    _tankList.add(tank);
    unawaited(refreshDisplay());
    notifyListeners();
    unawaited(writeTankList(tankList));
  }

  Future<void> removeTank(Tank tank) async {
    _tankList.remove(tank);
    if (_currentTank == tank) {
      clearTank();
      unawaited(refreshDisplay());
    }
    notifyListeners();
    unawaited(writeTankList(tankList));
  }

  void clearTank() {
    _currentTank = _emptyTank;
    notifyListeners();
  }

  set currentIndex(int index) {
    _currentIndex = index;
    notifyListeners();
  }

  set tankList(List<Tank> tankList) {
    _tankList = tankList;
    notifyListeners();
  }

  set display(String text) {
    _display = text;
    notifyListeners();
  }

  set currentData(Map<String, dynamic> data) {
    _currentData = data;
    notifyListeners();
  }

  set currentTank(Tank newTank) {
    _currentTank = newTank;
    notifyListeners();
  }

  Map<String, dynamic> get currentData => _currentData;
  Map<String, dynamic> get files => _files;
  int get currentIndex => _currentIndex;
  List<Tank> get tankList => _tankList;
  Tank get currentTank => _currentTank;
  String get display => _display;
}
