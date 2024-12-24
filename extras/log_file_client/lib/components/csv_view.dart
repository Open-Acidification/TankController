import 'dart:async';

import 'package:flutter/material.dart';
import 'package:log_file_client/utils/http_client.dart';

class CsvView extends StatelessWidget {
  CsvView({required this.csvPath, required this.httpClient, super.key});

  final String csvPath;
  final HttpClient httpClient;
  late final Future<List<LogDataLine>> logData = getLogData();

  Future<List<LogDataLine>> getLogData() async {
    final table = await httpClient.getLogData(csvPath);
    return table!;
  }

  @override
  Widget build(BuildContext context) {
    return FutureBuilder<List<LogDataLine>>(
      future: logData, // Assuming this fetches a List<LogDataLine>
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
                      flex: 2,
                      child: Text(
                        'time',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      child: Text(
                        'tankid',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      child: Text(
                        'temp',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      child: Text(
                        'temp setpoint',
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
                        'pH setpoint',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      child: Text(
                        'onTime',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      child: Text(
                        'Kp',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      child: Text(
                        'Ki',
                        style: TextStyle(fontStyle: FontStyle.italic),
                      ),
                    ),
                    Expanded(
                      child: Text(
                        'Kd',
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
                          Expanded(
                            flex: 2,
                            child: Text(
                              row.time.toString(),
                              style: const TextStyle(fontSize: 16),
                            ),
                          ),
                          Expanded(child: Text(row.tankid.toString())),
                          Expanded(child: Text(row.temp.toString())),
                          Expanded(child: Text(row.tempSetpoint.toString())),
                          Expanded(child: Text(row.pH.toString())),
                          Expanded(child: Text(row.pHSetpoint.toString())),
                          Expanded(child: Text(row.onTime.toString())),
                          Expanded(child: Text(row.kp.toString())),
                          Expanded(child: Text(row.ki.toString())),
                          Expanded(child: Text(row.kd.toString())),
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
