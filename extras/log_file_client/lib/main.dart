import 'package:flutter/material.dart';
import 'package:log_file_client/pages/home_page.dart';
import 'package:log_file_client/utils/http_client.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key, this.httpClient});

  final HttpClient? httpClient;

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Tank Monitor',
      theme: ThemeData(
        scaffoldBackgroundColor: Colors.white,
        colorScheme: ColorScheme.fromSeed(
          seedColor: const Color.fromARGB(255, 21, 98, 231),
        ),
        useMaterial3: true,
      ),
      home: HomePage(httpClient: httpClient),
    );
  }
}
