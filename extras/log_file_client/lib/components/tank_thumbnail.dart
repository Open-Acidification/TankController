import 'package:flutter/material.dart';
import 'package:log_file_client/utils/http_client.dart';
import 'package:syncfusion_flutter_charts/charts.dart';

class TankThumbnail extends StatelessWidget {
  TankThumbnail({
    required this.snapshot,
    double? tempDeviation,
    double? pHDeviation,
    DateTime? now,
    super.key,
  })  : now = now ?? DateTime.now(),
        tempDeviation = tempDeviation ?? 0.5,
        pHDeviation = pHDeviation ?? 0.5;

  final TankSnapshot snapshot;
  final DateTime now;
  final double tempDeviation;
  final double pHDeviation;

  @override
  Widget build(BuildContext context) {
    if (snapshot.latestData.isEmpty) {
      return Center(
        child: Text(
          'No data available within past 12 hrs',
          style: TextStyle(color: Colors.grey.shade600, fontSize: 16),
          textDirection: TextDirection.ltr,
        ),
      );
    } else {
      return Column(
        children: [
          _graph(_pHSeries(), 'pHAxis'),
          _graph(_tempSeries(), 'TemperatureAxis'),
        ],
      );
    }
  }

  Widget _graph(series, String axis) {
    final double setpoint =
        axis == 'pHAxis' ? snapshot.pHSetpoint! : snapshot.temperatureSetpoint!;
    final double deviation = axis == 'pHAxis' ? pHDeviation : tempDeviation;

    return Expanded(
      child: SfCartesianChart(
        backgroundColor: Colors.white,
        plotAreaBorderWidth: 0,
        primaryXAxis: DateTimeAxis(
          intervalType: DateTimeIntervalType.hours,
          interval: 6,
          labelStyle: TextStyle(color: Colors.grey.shade700),
          maximum: now,
          minimum: now.subtract(const Duration(hours: 12)),
        ),
        primaryYAxis: NumericAxis(
          name: axis,
          minimum: setpoint - deviation,
          maximum: setpoint + deviation,
          anchorRangeToVisiblePoints: false,
          labelStyle: TextStyle(color: Colors.grey.shade700),
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
