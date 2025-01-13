import 'package:flutter/material.dart';
import 'package:log_file_client/utils/http_client.dart';
import 'package:syncfusion_flutter_charts/charts.dart';

class TankThumbnail extends StatelessWidget {
  const TankThumbnail({required this.snapshot, super.key});

  final TankSnapshot snapshot;

  @override
  Widget build(BuildContext context) {
    return SfCartesianChart(
      backgroundColor: Colors.white,
      plotAreaBorderWidth: 0,
      primaryXAxis: DateTimeAxis(
        intervalType: DateTimeIntervalType.hours,
        interval: 1,
      ),
      primaryYAxis: NumericAxis(
        name: 'pHAxis',
        // isVisible: false,
        minimum: 6,
        maximum: 8,
        anchorRangeToVisiblePoints: false,
      ),
      axes: <ChartAxis>[
        NumericAxis(
          name: 'TemperatureAxis',
          opposedPosition: true,
          // isVisible: false,
          minimum: 10,
          maximum: 30,
          anchorRangeToVisiblePoints: false,
        ),
      ],
      series: <CartesianSeries>[
        LineSeries<LogDataLine?, DateTime>(
          legendItemText: 'temp',
          name: 'temp',
          dataSource: snapshot.latestData,
          xValueMapper: (LogDataLine? log, _) => log?.time,
          yValueMapper: (LogDataLine? log, _) => log?.tempMean,
          color: Colors.blue,
          yAxisName: 'TemperatureAxis',
          animationDuration: 0,
        ),
        LineSeries<LogDataLine?, DateTime>(
          legendItemText: 'pH',
          name: 'pH',
          dataSource: snapshot.latestData,
          xValueMapper: (LogDataLine? log, _) => log?.time,
          yValueMapper: (LogDataLine? log, _) => log?.phCurrent,
          color: Colors.green,
          yAxisName: 'pHAxis',
          animationDuration: 0,
        ),
      ],
    );
  }
}
