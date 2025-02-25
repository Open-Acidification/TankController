import 'package:flutter/material.dart';
import 'package:log_file_client/components/chart_series_selector.dart';
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
        child: Column(
          mainAxisAlignment: MainAxisAlignment.end,
          children: [
            _graphBuilder(logData),
            _chartSeriesSelector(),
          ],
        ),
      ),
    );
  }

  FutureBuilder _graphBuilder(
    Future<List<LogDataLine>> logData,
  ) {
    return FutureBuilder<List<LogDataLine>>(
      future: logData,
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return Padding(
            padding: const EdgeInsets.only(bottom: 300),
            child: CircularProgressIndicator(),
          );
        } else if (snapshot.hasError) {
          return Center(child: Text('Error: ${snapshot.error}'));
        } else if (!snapshot.hasData || snapshot.data!.isEmpty) {
          return const Center(child: Text('No data found'));
        } else {
          final logData = snapshot.data!;
          return _graph(logData);
        }
      },
    );
  }

  Widget _chartSeriesSelector() {
    return Center(
      child: Padding(
        padding: const EdgeInsets.only(bottom: 8.0),
        child: ChartSeriesSelector(
          onPressed: toggleSeriesView,
        ),
      ),
    );
  }

  Widget _graph(List<LogDataLine> logData) {
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
      lineColor: Colors.grey.shade600,
      lineWidth: 1.5,
      lineDashArray: [2, 2],
    );

    return Expanded(
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
            isVisible: _showTemp && _showPH,
          ),
        ],
        trackballBehavior: trackballBehavior,
        series: _chartSeries(logData),
      ),
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
        color: Colors.blue.shade800,
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
        color: Colors.green.shade800,
        yAxisName: 'pHAxis',
        animationDuration: 0,
        initialIsVisible: _showPH,
      ),
    ];
  }
}
