import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:tank_manager/model/app_data.dart';
import 'package:tank_manager/view/home_page.dart';

Future<void> main() async {
  await AppData.instance().readTankList();
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

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
