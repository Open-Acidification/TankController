import 'package:flutter/material.dart';
import 'package:log_file_client/utils/http_client.dart';
import 'package:syncfusion_flutter_charts/charts.dart';

class GraphView extends StatelessWidget {
  const GraphView({
    required this.filePath,
    required this.httpClient,
    super.key,
  });

  final String filePath;
  final HttpClient httpClient;

  Future<List<LogDataLine>> getLogData() async {
    final table = await httpClient.getLogData(filePath);
    return table;
  }

  @override
  Widget build(BuildContext context) {
    // ignore: discarded_futures
    final Future<List<LogDataLine>> logData = getLogData();

    return Scaffold(
      body: Center(
        child: FutureBuilder<List<LogDataLine>>(
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
              return Container(
                padding: const EdgeInsets.all(16.0),
                child: _graph(logData),
              );
            }
          },
        ),
      ),
    );
  }

  SfCartesianChart _graph(List<LogDataLine> logData) {
    final trackballBehavior = TrackballBehavior(
      enable: true,
      tooltipDisplayMode: TrackballDisplayMode.groupAllPoints,
      markerSettings: TrackballMarkerSettings(
        markerVisibility: TrackballVisibilityMode.visible,
      ),
      tooltipSettings: InteractiveTooltip(
        enable: true,
        format: 'series.name : point.y',
      ),
      activationMode: ActivationMode.singleTap,
    );

    return SfCartesianChart(
      title: ChartTitle(text: 'Tank ID: ${logData.first.tankid}'),
      backgroundColor: Colors.white,
      primaryXAxis: DateTimeAxis(
        title: AxisTitle(text: 'Time'),
        intervalType: DateTimeIntervalType.hours,
        interval: 1,
      ),
      primaryYAxis: NumericAxis(
        name: 'pHAxis',
        title: AxisTitle(text: 'pH Value'),
      ),
      axes: <ChartAxis>[
        NumericAxis(
          name: 'TemperatureAxis',
          title: AxisTitle(text: 'Temperature Value'),
          opposedPosition: true,
        ),
      ],
      legend: Legend(
        isVisible: true,
        position: LegendPosition.bottom,
      ),
      trackballBehavior: trackballBehavior,
      series: _chartSeries(logData),
    );
  }

  List<CartesianSeries> _chartSeries(List<LogDataLine> logData) {
    return <CartesianSeries>[
      LineSeries<LogDataLine, DateTime>(
        legendItemText: 'temp',
        name: 'temp',
        dataSource: logData,
        xValueMapper: (LogDataLine log, _) => log.time,
        yValueMapper: (LogDataLine log, _) => log.tempMean,
        color: Colors.blue,
        yAxisName: 'TemperatureAxis',
        animationDuration: 0,
      ),
      LineSeries<LogDataLine, DateTime>(
        legendItemText: 'temp setpoint',
        name: 'temp setpoint',
        dataSource: logData,
        xValueMapper: (LogDataLine log, _) => log.time,
        yValueMapper: (LogDataLine log, _) => log.tempTarget,
        dashArray: <double>[5, 5],
        color: Colors.blue,
        yAxisName: 'TemperatureAxis',
        animationDuration: 0,
      ),
      LineSeries<LogDataLine, DateTime>(
        legendItemText: 'pH',
        name: 'pH',
        dataSource: logData,
        xValueMapper: (LogDataLine log, _) => log.time,
        yValueMapper: (LogDataLine log, _) => log.phCurrent,
        color: Colors.green,
        yAxisName: 'pHAxis',
        animationDuration: 0,
      ),
      LineSeries<LogDataLine, DateTime>(
        legendItemText: 'pH setpoint',
        name: 'pH setpoint',
        dataSource: logData,
        xValueMapper: (LogDataLine log, _) => log.time,
        yValueMapper: (LogDataLine log, _) => log.phTarget,
        dashArray: <double>[5, 5],
        color: Colors.green,
        yAxisName: 'pHAxis',
        animationDuration: 0,
      ),
    ];
  }
}
