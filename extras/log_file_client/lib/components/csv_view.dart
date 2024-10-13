import 'dart:async';

import 'package:flutter/material.dart';
import 'package:log_file_client/utils/csv_reader.dart';

class CsvView extends StatefulWidget {
  const CsvView({super.key, required this.csvPath});
  final String csvPath;

  @override
  State<CsvView> createState() => _CsvViewState();
}

class _CsvViewState extends State<CsvView> {
  Future<List> getCsvTable() async {
    final reader = CsvReaderForApp(widget.csvPath);
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
          return SingleChildScrollView(
            scrollDirection: Axis.horizontal,
            child: SingleChildScrollView(
              scrollDirection: Axis.vertical,
              child: DataTable(
                // Headers
                columns: csvTable[0].map<DataColumn>((header) {
                  return DataColumn(
                    label: Text(
                      header.toString(),
                      style: const TextStyle(fontStyle: FontStyle.italic),
                    ),
                  );
                }).toList(),

                // Data
                rows: csvTable.sublist(1, 1000).map<DataRow>((row) {
                  return DataRow(
                    cells: row.map<DataCell>((cell) {
                      return DataCell(Text(cell.toString()));
                    }).toList(),
                  );
                }).toList(),
              ),
            ),
          );
        }
      },
    );
  }
}
