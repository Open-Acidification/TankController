import 'package:device_client/model/app_data.dart';
import 'package:device_client/view/home_page.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

Future<void> main() async {
  await AppData.instance().readTankList();
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider<AppData>.value(
      value: AppData.instance(),
      child: MaterialApp(
        debugShowCheckedModeBanner: false,
        title: 'Tank Manager',
        theme: ThemeData(
          primarySwatch: Colors.blue,
          scaffoldBackgroundColor: Colors.grey.shade500,
        ),
        home: const MyHomePage(title: 'Tank Manager'),
      ),
    );
  }
}
