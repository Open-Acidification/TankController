import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:tank_manager/model/tank.dart';
import 'package:tank_manager/model/tc_interface.dart';
import 'dart:async';

class AppData with ChangeNotifier {
  static AppData? _instance;

  static get instance {
    _instance ??= AppData();
    return _instance;
  }

  AppData() {
    _currentTank = _emptyTank;
  }

  final _emptyTank = Tank('', '');
  dynamic _currentTank;
  var _display = '';
  Map<String, dynamic> _information = <String, dynamic>{};
  Map<String, dynamic> _files = <String, dynamic>{};
  List<Tank> _tankList = [];
  int _currentIndex = 0;

  Future<void> readTankList() async {
    if (tankList.isNotEmpty) return;
    WidgetsFlutterBinding.ensureInitialized();
    SharedPreferences prefs = await SharedPreferences.getInstance();
    if (prefs.containsKey('obj1')) {
      String obj1 = prefs.getString('obj1')!;
      var x = jsonDecode(obj1).map((obj1) => Tank.fromJson(obj1));
      _tankList = List<Tank>.from(x);
    }
  }

  Future<void> writeTankList(tankList) async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    prefs.setString('obj1', jsonEncode(tankList));
  }

  Future<void> refreshDisplay() async {
    if (currentTank.isNotEmpty()) {
      var tcInterface = TcInterface.instance;
      display = await tcInterface.get(currentTank.ip, 'display');
    } else {
      display = '';
    }
    notifyListeners();
  }

  Future<void> refreshInformation() async {
    if (currentTank.isEmpty()) {
      _information = jsonDecode("{\"Error: Choose tank from menu\":\"\"}");
    } else {
      var tcInterface = TcInterface.instance;
      var value = await tcInterface.get(currentTank.ip, 'current');
      _information = jsonDecode(value);
    }
    notifyListeners();
  }

  Future<void> refreshFiles() async {
    if (currentTank.isEmpty()) {
      _files = jsonDecode("{\"Error: Choose tank from menu\":\"\"}");
    } else {
      var tcInterface = TcInterface.instance;
      var value = await tcInterface.get(currentTank.ip, 'rootdir');
      while (value.substring(value.length - 1) == "\n") {
        value = value.substring(0, value.length - 1);
      }
      value = value.replaceAll("\n", '", "');
      value = value.replaceAll("\t", '":"');
      value = '{"$value"}';
      _files = jsonDecode(value);
    }
    notifyListeners();
  }

  Future<void> addTank(tank) async {
    // make a call to the device to see if it exists
    // ignore result
    await TcInterface.instance.get(tank.ip, 'current');
    _tankList.add(tank);
    refreshDisplay();
    notifyListeners();
    writeTankList(tankList);
  }

  void removeTank(tank) {
    _tankList.remove(tank);
    if (_currentTank == tank) {
      clearTank();
      refreshDisplay();
    }
    notifyListeners();
    writeTankList(tankList);
  }

  void clearTank() {
    _currentTank = _emptyTank;
    notifyListeners();
  }

  set currentIndex(index) {
    _currentIndex = index;
    notifyListeners();
  }

  set tankList(tankList) {
    _tankList = tankList;
    notifyListeners();
  }

  set display(text) {
    _display = text;
    notifyListeners();
  }

  set currentTank(newTank) {
    _currentTank = newTank;
    notifyListeners();
  }

  Map<String, dynamic> get information => _information;
  Map<String, dynamic> get files => _files;
  int get currentIndex => _currentIndex;
  List<Tank> get tankList => _tankList;
  dynamic get currentTank => _currentTank;
  String get display => _display;
}
