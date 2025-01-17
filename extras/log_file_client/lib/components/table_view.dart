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
          return _table(logData);
        }
      },
    );
  }

  Column _table(List<LogDataLine> logData) {
    return Column(
      children: [
        _tableHeader(),
        _tableData(logData),
      ],
    );
  }

  Container _tableHeader() {
    return Container(
      padding: const EdgeInsets.all(5),
      decoration: const BoxDecoration(
        border: Border(bottom: BorderSide(color: Colors.grey)),
      ),
      child: Row(
        children: [
          _headerItem(item: 'Version'),
          _headerItem(item: 'Tank ID'),
          _headerItem(item: 'Time', flex: 2),
          _headerItem(item: 'Temp Target'),
          _headerItem(item: 'Temp Mean'),
          _headerItem(item: 'Temp Std Dev'),
          _headerItem(item: 'pH Target'),
          _headerItem(item: 'pH'),
          _headerItem(item: 'onTime'),
        ],
      ),
    );
  }

  Expanded _headerItem({required String item, int flex = 1}) {
    return Expanded(
      flex: flex,
      child: Text(
        item,
        style: TextStyle(fontStyle: FontStyle.italic),
      ),
    );
  }

  Expanded _tableData(List<LogDataLine> logData) {
    return Expanded(
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
            child: _dataRow(row),
          );
        },
      ),
    );
  }

  Row _dataRow(LogDataLine row) {
    return Row(
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
    );
  }
}
