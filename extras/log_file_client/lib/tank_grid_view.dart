import 'package:flutter/material.dart';
import 'package:log_file_client/main.dart';
import 'package:log_file_client/tank_list.dart';

class TankGridView extends State<MyHomePage> {
  final getTankList = GetTankListForApp();

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Tank Monitor'),
      ),
      body: Center(
        child: FutureBuilder(
          future: getTankList.fetchList(),
          builder: (BuildContext context, AsyncSnapshot snapshot) {
            if (snapshot.connectionState == ConnectionState.waiting) {
              return const CircularProgressIndicator();
            } else if (snapshot.hasError) {
              return Text('Error: ${snapshot.error}');
            } else {
              return GridView.builder(
                gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
                  crossAxisCount: 3,
                ),
                itemCount: snapshot.data.length,
                itemBuilder: (BuildContext context, int index) {
                  return Card(
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
