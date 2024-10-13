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
  List? csvTable;
  
  Future<List> getCsvTable() async {
    final reader = CsvReaderForApp(widget.csvPath);
    final table = await reader.csvTable();
    return table;
  }

  @override
  void initState() {
    super.initState();
    unawaited(getCsvTable().then((value) {
      setState(() {
        csvTable = value;
      });
    }));
  }

  @override
  Widget build(BuildContext context) {
    if (csvTable == null) {
      return const Center(child: CircularProgressIndicator());
    } else {
      return DataTable(
        
        // Headers
        columns: csvTable![0].map<DataColumn>((header) {
          return DataColumn(
            label: Text(
              header.toString(),
              style: const TextStyle(fontStyle: FontStyle.italic),
            ),
          );
        }).toList(),

        // Data
        rows: csvTable!.sublist(1, 6).map<DataRow>((row) {
          return DataRow(
            cells: row.map<DataCell>((cell) {
              return DataCell(Text(cell.toString()));
            }).toList(),
          );
        }).toList(),
      );
    }
  }
}