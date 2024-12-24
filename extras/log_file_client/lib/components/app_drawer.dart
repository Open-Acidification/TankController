import 'package:flutter/material.dart';
import 'package:log_file_client/utils/http_client.dart';

class AppDrawer extends StatelessWidget {
  const AppDrawer({
    required this.logList,
    required this.openLogFile,
    super.key,
  });
  final List<Log> logList;
  final void Function(Log) openLogFile;

  @override
  Widget build(BuildContext context) {
    return Drawer(
      backgroundColor: Theme.of(context).colorScheme.inversePrimary,
      child: Column(
        children: [
          // Header
          DrawerHeader(
            child: Image.asset('lib/assets/oap.png'),
          ),

          // CSV files that can be opened
          Expanded(
            child: ListView.builder(
              itemCount: logList.length,
              itemBuilder: (context, index) {
                final log = logList[index];
                return ListTile(
                  title: Text(log.name),
                  onTap: () {
                    openLogFile(log);
                  },
                );
              },
            ),
          ),
        ],
      ),
    );
  }
}
