import 'package:flutter/material.dart';
import 'package:log_file_client/utils/http_client.dart';
import 'package:syncfusion_flutter_charts/charts.dart';

class GraphView extends StatefulWidget {
  const GraphView({required this.csvPath, required this.httpClient, super.key});

  final String csvPath;
  final HttpClient httpClient;

  @override
  State<GraphView> createState() => _GraphViewState();
}

class _GraphViewState extends State<GraphView> {
  late TrackballBehavior _trackballBehavior;
  late final Future<List<LogDataLine>> logData = getLogData();

  Future<List<LogDataLine>> getLogData() async {
    final table = await widget.httpClient.getLogData(widget.csvPath);
    return table!;
  }

  @override
  void initState() {
    _trackballBehavior = TrackballBehavior(
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
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
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
                child: SfCartesianChart(
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
                  trackballBehavior: _trackballBehavior,
                  series: <CartesianSeries>[
                    LineSeries<LogDataLine, DateTime>(
                      legendItemText: 'temp',
                      name: 'temp',
                      dataSource: logData,
                      xValueMapper: (LogDataLine log, _) => log.time,
                      yValueMapper: (LogDataLine log, _) => log.temp,
                      color: Colors.blue,
                      yAxisName: 'TemperatureAxis',
                    ),
                    LineSeries<LogDataLine, DateTime>(
                      legendItemText: 'temp setpoint',
                      name: 'temp setpoint',
                      dataSource: logData,
                      xValueMapper: (LogDataLine log, _) => log.time,
                      yValueMapper: (LogDataLine log, _) => log.tempSetpoint,
                      dashArray: <double>[5, 5],
                      color: Colors.blue,
                      yAxisName: 'TemperatureAxis',
                    ),
                    LineSeries<LogDataLine, DateTime>(
                      legendItemText: 'pH',
                      name: 'pH',
                      dataSource: logData,
                      xValueMapper: (LogDataLine log, _) => log.time,
                      yValueMapper: (LogDataLine log, _) => log.pH,
                      color: Colors.green,
                      yAxisName: 'pHAxis',
                    ),
                    LineSeries<LogDataLine, DateTime>(
                      legendItemText: 'pH setpoint',
                      name: 'pH setpoint',
                      dataSource: logData,
                      xValueMapper: (LogDataLine log, _) => log.time,
                      yValueMapper: (LogDataLine log, _) => log.pHSetpoint,
                      dashArray: <double>[5, 5],
                      color: Colors.green,
                      yAxisName: 'pHAxis',
                    ),
                  ],
                ),
              );
            }
          },
        ),
      ),
    );
  }
}
