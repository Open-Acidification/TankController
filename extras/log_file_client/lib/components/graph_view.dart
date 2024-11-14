import 'package:flutter/material.dart';
import 'package:syncfusion_flutter_charts/charts.dart';

class GraphView extends StatefulWidget {
  const GraphView({super.key});

  @override
  State<GraphView> createState() => _GraphViewState();
}

class _GraphViewState extends State<GraphView> {
  late TrackballBehavior _trackballBehavior;
  final List<LogData> chartData = [
    LogData(
      DateTime.parse('2023-01-20 16:18:00'),
      99,
      0.00,
      10.00,
      0.000,
      8.645,
      6,
      700.0,
      100.0,
      0.0,
    ),
    LogData(
      DateTime.parse('2023-01-20 16:19:00'),
      99,
      1.23,
      10.00,
      7.123,
      8.645,
      8,
      710.0,
      110.0,
      1.0,
    ),
    LogData(
      DateTime.parse('2023-01-20 16:20:00'),
      99,
      2.34,
      10.00,
      6.789,
      8.645,
      9,
      720.0,
      120.0,
      2.0,
    ),
    LogData(
      DateTime.parse('2023-01-20 16:21:00'),
      99,
      3.45,
      10.00,
      5.456,
      8.645,
      10,
      730.0,
      130.0,
      3.0,
    ),
    LogData(
      DateTime.parse('2023-01-20 16:22:00'),
      99,
      4.56,
      10.00,
      4.123,
      8.645,
      11,
      740.0,
      140.0,
      4.0,
    ),
  ];

  @override
  void initState() {
    _trackballBehavior = TrackballBehavior(
      enable: true,
      tooltipDisplayMode: TrackballDisplayMode.nearestPoint,
      markerSettings: TrackballMarkerSettings(
          markerVisibility: TrackballVisibilityMode.visible),
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
        child: Container(
          padding: const EdgeInsets.all(16.0),
          child: SfCartesianChart(
            title: ChartTitle(text: 'Tank ID: ${chartData.first.tankid}'),
            primaryXAxis: DateTimeAxis(
              title: AxisTitle(text: 'Time'),
              intervalType: DateTimeIntervalType.minutes,
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
              LineSeries<LogData, DateTime>(
                legendItemText: 'temp',
                name: 'temp',
                dataSource: chartData,
                xValueMapper: (LogData log, _) => log.time,
                yValueMapper: (LogData log, _) => log.temp,
                color: Colors.blue,
                yAxisName: 'TemperatureAxis',
              ),
              LineSeries<LogData, DateTime>(
                legendItemText: 'temp setpoint',
                name: 'temp setpoint',
                dataSource: chartData,
                xValueMapper: (LogData log, _) => log.time,
                yValueMapper: (LogData log, _) => log.tempSetpoint,
                dashArray: <double>[5, 5],
                color: Colors.blue,
                yAxisName: 'TemperatureAxis',
              ),
              LineSeries<LogData, DateTime>(
                legendItemText: 'pH',
                name: 'pH',
                dataSource: chartData,
                xValueMapper: (LogData log, _) => log.time,
                yValueMapper: (LogData log, _) => log.pH,
                color: Colors.green,
                yAxisName: 'pHAxis',
              ),
              LineSeries<LogData, DateTime>(
                legendItemText: 'pH setpoint',
                name: 'pH setpoint',
                dataSource: chartData,
                xValueMapper: (LogData log, _) => log.time,
                yValueMapper: (LogData log, _) => log.pHSetpoint,
                dashArray: <double>[5, 5],
                color: Colors.green,
                yAxisName: 'pHAxis',
              ),
            ],
          ),
        ),
      ),
    );
  }
}

class LogData {
  LogData(
    this.time,
    this.tankid,
    this.temp,
    this.tempSetpoint,
    this.pH,
    this.pHSetpoint,
    this.onTime,
    this.kp,
    this.ki,
    this.kd,
  );
  final DateTime time;
  final int tankid;
  final double temp;
  final double tempSetpoint;
  final double pH;
  final double pHSetpoint;
  final int onTime;
  final double kp;
  final double ki;
  final double kd;
}
