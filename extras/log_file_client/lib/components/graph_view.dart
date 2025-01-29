import 'package:flutter/material.dart';
import 'package:log_file_client/components/toggle_button.dart';
import 'package:log_file_client/utils/http_client.dart';
import 'package:syncfusion_flutter_charts/charts.dart';

class GraphView extends StatefulWidget {
  const GraphView({
    required this.filePath,
    required this.httpClient,
    super.key,
  });

  final String filePath;
  final HttpClient httpClient;

  @override
  State<GraphView> createState() => _GraphViewState();
}

class _GraphViewState extends State<GraphView> {
  bool _showPH = true;
  bool _showTemp = true;

  Future<List<LogDataLine>> getLogData() async {
    final table = await widget.httpClient.getLogData(widget.filePath);
    return table;
  }

  void toggleSeriesView(int index) {
    setState(() {
      if (index == 0) {
        _showPH = !_showPH;
      } else {
        _showTemp = !_showTemp;
      }
    });
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
              return Column(
                children: [
                  Expanded(
                    child: _graph(logData),
                  ),
                  Center(
                    child: Padding(
                      padding: const EdgeInsets.only(bottom: 8.0),
                      child: ToggleButton(
                        onPressed: toggleSeriesView,
                      ),
                    ),
                  ),
                ],
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
      // primaryYAxis: _showPH
      //     ? NumericAxis(
      //         name: 'pHAxis',
      //         title: AxisTitle(text: 'pH Value'),
      //       )
      //     : NumericAxis(
      //         name: 'TemperatureAxis',
      //         title: AxisTitle(text: 'Temperature Value'),
      //         opposedPosition: true,
      //       ),
      axes: <ChartAxis>[
        // NumericAxis(
        //   name: 'pHAxis',
        //   title: AxisTitle(text: 'pH Value'),
        //   isVisible: _showPH && _showTemp,
        // ),
        NumericAxis(
          name: 'TemperatureAxis',
          title: AxisTitle(text: 'Temperature Value'),
          opposedPosition: true,
          isVisible: _showTemp && _showPH,
        ),
      ],
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
        initialIsVisible: _showTemp,
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
        initialIsVisible: _showTemp,
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
        initialIsVisible: _showPH,
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
        initialIsVisible: _showPH,
      ),
    ];
  }
}
