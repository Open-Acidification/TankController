import 'dart:async';

import 'package:flutter/material.dart';
import 'package:log_file_client/utils/http_client.dart';

class TableView extends StatelessWidget {
  TableView({required this.filePath, required this.httpClient, super.key});

  final String filePath;
  final HttpClient httpClient;
  late final Future<List<LogDataLine>> logData = getLogData();

  Future<List<LogDataLine>> getLogData() async {
    final table = await httpClient.getLogData(filePath);
    return table;
  }

  @override
  Widget build(BuildContext context) {
    return FutureBuilder<List<LogDataLine>>(
      future: logData,
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return const Center(child: CircularProgressIndicator());
        } else if (snapshot.hasError) {
          return Center(child: Text('Error: ${snapshot.error}'));
        } else if (!snapshot.hasData || snapshot.data!.isEmpty) {
          return const Center(child: Text('No data found'));
        } else {
          final logData = snapshot.data!;
          return Column(
            children: [
              // Headers
              Container(
                padding: const EdgeInsets.all(5),
                decoration: const BoxDecoration(
                  border: Border(bottom: BorderSide(color: Colors.grey)),
                ),
                child: Row(
                  children: const [
                    Expanded(
                      child: Text(
                        'Version',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      child: Text(
                        'Tank ID',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      flex: 2,
                      child: Text(
                        'Time',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      child: Text(
                        'Temp Target',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      child: Text(
                        'Temp Mean',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      child: Text(
                        'Temp Standard Deviation',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      child: Text(
                        'pH Target',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      child: Text(
                        'pH',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      child: Text(
                        'onTime',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                  ],
                ),
              ),

              // Data
              Expanded(
                child: ListView.builder(
                  // Cutoff at 5000 lines to avoid long wait times
                  itemCount: logData.length > 5000 ? 5000 : logData.length,
                  itemBuilder: (context, index) {
                    final row = logData[index];
                    return Container(
                      padding: const EdgeInsets.all(5),
                      decoration: BoxDecoration(
                        border: Border(
                          bottom: BorderSide(color: Colors.grey.shade400),
                        ),
                      ),
                      child: Row(
                        children: [
                          Expanded(child: Text(row.version.toString())),
                          Expanded(child: Text(row.tankid.toString())),
                          Expanded(
                            flex: 2,
                            child: Text(
                              row.time.toString(),
                              style: const TextStyle(fontSize: 16),
                            ),
                          ),
                          Expanded(child: Text(row.tempTarget.toString())),
                          Expanded(child: Text(row.tempMean.toString())),
                          Expanded(child: Text(row.tempStdDev.toString())),
                          Expanded(child: Text(row.phTarget.toString())),
                          Expanded(child: Text(row.phCurrent.toString())),
                          Expanded(child: Text(row.onTime.toString())),
                        ],
                      ),
                    );
                  },
                ),
              ),
            ],
          );
        }
      },
    );
  }
}
