import 'dart:async';

import 'package:flutter/material.dart';
import 'package:log_file_client/main.dart';
import 'package:log_file_client/tank_list.dart';

class TankGridView extends State<MyHomePage> {
  // TankGridView() {
  //   _tankList = TankListReader.current.fetchList();
  // }

  late Future<List<Tank>> _tankList;

  @override
  void initState() {
    print('initState');
    super.initState();
    unawaited(startReading());
  }

  Future<void> startReading() async {
    _tankList = TankListReader.current.fetchList();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Tank Monitor'),
      ),
      body: Center(
        child: FutureBuilder(
          future: _tankList,
          builder: (BuildContext context, AsyncSnapshot snapshot) {
            if (snapshot.connectionState == ConnectionState.waiting) {
              print('waiting');
              return const CircularProgressIndicator();
            } else if (snapshot.hasError) {
              print('error');
              return Text('Error: ${snapshot.error}');
            } else {
              print('loaded');
              return GridView.builder(
                key: const ValueKey('tank-grid-view'),
                gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
                  crossAxisCount: 3,
                ),
                itemCount: snapshot.data.length,
                itemBuilder: (BuildContext context, int index) {
                  print('building card $index ${snapshot.data[index].name}');
                  return Card(
                    key: ValueKey('tank-card-$index'),
                    child: Center(
                      child: Text(snapshot.data[index].name),
                    ),
                  );
                },
              );
            }
          },
        ),
      ),
    );
  }
}
