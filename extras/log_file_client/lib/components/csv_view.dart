import 'dart:async';

import 'package:flutter/material.dart';
import 'package:log_file_client/utils/csv_reader.dart';

class CsvView extends StatelessWidget {
  CsvView({required this.csvPath, super.key});
  final String csvPath;
  late final Future<List<List<dynamic>>> csvTable = getCsvTable();

  Future<List<List<dynamic>>> getCsvTable() async {
    // final reader = CsvReaderForAppLocal(csvPath);
    final reader = CsvReaderForAppLocal('csv_test.csv');
    final table = await reader.csvTable();
    return table;
  }

  @override
  Widget build(BuildContext context) {
    return FutureBuilder<List>(
      future: csvTable,
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return const Center(child: CircularProgressIndicator());
        } else if (snapshot.hasError) {
          return Center(child: Text('Error: ${snapshot.error}'));
        } else if (!snapshot.hasData || snapshot.data!.isEmpty) {
          return const Center(child: Text('No data found'));
        } else {
          final csvTable = snapshot.data!;
          return Column(
            children: [
              // Headers
              Container(
                padding: EdgeInsets.all(5),
                decoration: const BoxDecoration(
                  border: Border(bottom: BorderSide(color: Colors.grey)),
                ),
                child: Row(
                  children: csvTable[0].asMap().entries.map<Widget>((entry) {
                    final int idx = entry.key;
                    final header = entry.value;
                    return Expanded(
                      flex: idx == 0
                          ? 2
                          : 1, // Allow more space for the first column for the datetimes
                      child: Text(
                        header.toString(),
                        style: const TextStyle(fontStyle: FontStyle.italic),
                      ),
                    );
                  }).toList(),
                ),
              ),

              // Data
              Expanded(
                child: ListView.builder(
                  // Cutoff at 5000 lines to avoid long wait times
                  itemCount: csvTable.length > 5000
                      ? csvTable.sublist(1, 5000).length
                      : csvTable.length - 1,
                  itemBuilder: (context, index) {
                    return Container(
                      padding: EdgeInsets.all(5),
                      decoration: BoxDecoration(
                        border: Border(
                          bottom: BorderSide(color: Colors.grey.shade400),
                        ),
                      ),
                      child: Row(
                        children: csvTable[index + 1]
                            .asMap()
                            .entries
                            .map<Widget>((entry) {
                          final int idx = entry.key;
                          final cell = entry.value;
                          return Expanded(
                            flex: idx == 0
                                ? 2
                                : 1, // Allow more space for the first column
                            child: Text(
                              cell is num
                                ? (idx >= 2 && idx <= 5
                                  ? cell.toStringAsFixed(3)
                                  : cell.toStringAsFixed(0))
                                : cell.toString(),
                            ),
                          );
                        }).toList(),
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
