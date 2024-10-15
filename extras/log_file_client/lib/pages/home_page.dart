import 'package:flutter/material.dart';
import 'package:log_file_client/components/app_drawer.dart';
import 'package:log_file_client/components/csv_view.dart';
import 'package:log_file_client/utils/log_list.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key, this.logListReader});

  final LogListReader? logListReader;

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  late final LogListReader logListReader;
  List<Log>? _logList;
  bool _isLoading = true;
  int? _openedLogIndex;

  @override
  void initState() {
    super.initState();
    logListReader = widget.logListReader ?? LogListReaderForAppLocal();
    _getLogList();
  }

  // Fetches the list of csv files avaliable
  _getLogList() async {
    final result = await logListReader.fetchList();
    setState(() {
      _logList = result;
      _isLoading = false;
    });
  }

  void openLogFile (Log log) {
    setState(() {
      _openedLogIndex = _logList!.indexOf(log); // Open the selected log file
      Navigator.of(context).pop(); // Close the drawer
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Tank Monitor'),
      ),
      drawer: _isLoading 
        ? Drawer() // null drawer
        : AppDrawer(
          logList: _logList!,
          openLogFile: openLogFile,
        ),
      body: Center(
        child: _isLoading 
          ? const CircularProgressIndicator() 
          : _openedLogIndex != null ? CsvView(csvPath: _logList![_openedLogIndex!].uri) : null,
      ),
    );
  }
}
