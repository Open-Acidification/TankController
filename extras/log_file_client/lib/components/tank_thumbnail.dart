import 'package:flutter/material.dart';
import 'package:log_file_client/utils/http_client.dart';
import 'package:syncfusion_flutter_charts/charts.dart';

class TankThumbnail extends StatelessWidget {
  const TankThumbnail({required this.snapshot, super.key});

  final TankSnapshot snapshot;

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        _graph(_pHSeries(), 'pHAxis'),
        _graph(_tempSeries(), 'TemperatureAxis'),
      ],
    );
  }

  Widget _graph(series, String axis) {
    return Expanded(
      child: SfCartesianChart(
        backgroundColor: Colors.white,
        plotAreaBorderWidth: 0,
        primaryXAxis: DateTimeAxis(
          intervalType: DateTimeIntervalType.hours,
          interval: 1,
          // isVisible: !(axis == 'pHAxis'),
        ),
        primaryYAxis: NumericAxis(
          name: axis,
          // title: axis == 'pHAxis'
          //     ? const AxisTitle(text: 'pH')
          //     : const AxisTitle(text: 'temp'),
          minimum: axis == 'pHAxis'
              ? snapshot.pHSetpoint! - 0.5
              : snapshot.temperatureSetpoint! - 0.5,
          maximum: axis == 'pHAxis'
              ? snapshot.pHSetpoint! + 0.5
              : snapshot.temperatureSetpoint! + 0.5,
          anchorRangeToVisiblePoints: false,
        ),
        series: series,
      ),
    );
  }

  List<CartesianSeries> _pHSeries() {
    return <CartesianSeries>[
      LineSeries<LogDataLine?, DateTime>(
        legendItemText: 'pH',
        name: 'pH',
        dataSource: snapshot.latestData,
        xValueMapper: (LogDataLine? log, _) => log?.time,
        yValueMapper: (LogDataLine? log, _) => log?.phCurrent,
        color: Colors.green,
        width: 1,
        yAxisName: 'pHAxis',
        animationDuration: 0,
      ),
      LineSeries<LogDataLine?, DateTime>(
        legendItemText: 'pH setpoint',
        name: 'pH setpoint',
        dataSource: snapshot.latestData,
        xValueMapper: (LogDataLine? log, _) => log?.time,
        yValueMapper: (LogDataLine? log, _) => log?.phTarget,
        color: Colors.green.shade800,
        width: 1,
        yAxisName: 'pHAxis',
        animationDuration: 0,
      ),
    ];
  }

  List<CartesianSeries> _tempSeries() {
    return <CartesianSeries>[
      LineSeries<LogDataLine?, DateTime>(
        legendItemText: 'temp',
        name: 'temp',
        dataSource: snapshot.latestData,
        xValueMapper: (LogDataLine? log, _) => log?.time,
        yValueMapper: (LogDataLine? log, _) => log?.tempMean,
        color: Colors.blue,
        width: 1,
        yAxisName: 'TemperatureAxis',
        animationDuration: 0,
      ),
      LineSeries<LogDataLine?, DateTime>(
        legendItemText: 'temp setpoint',
        name: 'temp setpoint',
        dataSource: snapshot.latestData,
        xValueMapper: (LogDataLine? log, _) => log?.time,
        yValueMapper: (LogDataLine? log, _) => log?.tempTarget,
        color: Colors.blue.shade800,
        width: 1,
        yAxisName: 'TemperatureAxis',
        animationDuration: 0,
      ),
    ];
  }
}
