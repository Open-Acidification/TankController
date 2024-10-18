import 'dart:async';

import 'package:flutter/material.dart';
import 'package:log_file_client/utils/csv_reader.dart';

class CsvView extends StatelessWidget {
  const CsvView({required this.csvPath, super.key});
  final String csvPath;

  Future<List<List<dynamic>>> getCsvTable() async {
    final reader = CsvReaderForAppLocal(csvPath);
    final table = await reader.csvTable();
    return table;
  }

  @override
  Widget build(BuildContext context) {
    return FutureBuilder<List>(
      future: getCsvTable(),
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
                  itemCount: csvTable.sublist(1, 5000).length,
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
                            child: Text(cell.toString()),
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
