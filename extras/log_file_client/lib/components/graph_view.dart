import 'package:flutter/material.dart';
import 'package:intl/intl.dart';
import 'package:log_file_client/components/chart_series_selector.dart';
import 'package:log_file_client/components/time_range_selector.dart';
import 'package:log_file_client/utils/http_client.dart';
import 'package:syncfusion_flutter_charts/charts.dart';

class GraphView extends StatefulWidget {
  const GraphView({
    required this.logData,
    this.now,
    super.key,
  });

  final List<LogDataLine> logData;
  final DateTime? now;

  @override
  State<GraphView> createState() => _GraphViewState();
}

class _GraphViewState extends State<GraphView> {
  bool _showPH = true;
  bool _showTemp = true;
  late final DateTime now;
  late final DateTimeRange avaliableTimeRange;
  late DateTimeRange displayedTimeRange;
  late List<int> _displayedTimeRangeIndices;
  late DateTimeIntervalType timeIntervalType;
  late double timeInterval;
  late DateFormat timeFormat;

  int scatterGranularity = 1;

  @override
  void initState() {
    super.initState();
    now = widget.now ?? DateTime.now();
    avaliableTimeRange = DateTimeRange(
      start: widget.logData.first.time,
      end: widget.logData.last.time,
    );
    toggleTimeRange(2, null);
    calculateTimeAxisFormat();
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

  void toggleTimeRange(int index, DateTimeRange? customRange) {
    // index: 0-5 are predefined ranges, 6 is max, 7 is custom
    final List<Duration> durations = [
      Duration(hours: 6),
      Duration(hours: 12),
      Duration(days: 1),
      Duration(days: 3),
      Duration(days: 7),
      Duration(days: 30),
    ];

    late DateTimeRange timeRange;
    if (index < 6) {
      timeRange = DateTimeRange(
        start: now.subtract(durations[index]),
        end: now,
      );
    } else if (index == 6) {
      timeRange = avaliableTimeRange;
    } else {
      timeRange = customRange!;
    }

    setState(() {
      displayedTimeRange = timeRange;
      _displayedTimeRangeIndices = calculateTimeRange(timeRange);
      calculateTimeAxisFormat();
      calculateGranularity();
    });
  }

  List<int> calculateTimeRange(DateTimeRange range) {
    int startIndex = 0;
    int endIndex = widget.logData.length;
    for (int i = widget.logData.length - 1; i >= 0; i--) {
      if (widget.logData[i].time.compareTo(range.start) < 0) {
        startIndex = i + 1;
        break;
      }
    }
    for (int i = widget.logData.length - 1; i >= 0; i--) {
      if (widget.logData[i].time.compareTo(range.end) > 0) {
        endIndex = i + 1;
        break;
      }
    }
    return [startIndex, endIndex];
  }

  void calculateTimeAxisFormat() {
    timeInterval = 1;

    if (displayedTimeRange.duration <= Duration(hours: 24)) {
      timeIntervalType = DateTimeIntervalType.hours;
      timeFormat = DateFormat('h a');
    } else if (displayedTimeRange.duration <= Duration(days: 3)) {
      timeIntervalType = DateTimeIntervalType.days;
      timeFormat = DateFormat('h a\nMMM d');
      timeInterval = 0.5;
    } else {
      timeIntervalType = DateTimeIntervalType.days;
      timeFormat = DateFormat('MMM d');
    }
  }

  void calculateGranularity() {
    final Duration duration = displayedTimeRange.duration;
    if (duration <= Duration(days: 5)) {
      scatterGranularity = 1;
    } else if (duration <= Duration(days: 20)) {
      scatterGranularity = 3;
    } else {
      scatterGranularity = 10;
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.start,
          children: [
            _topRow(widget.logData),
            _graph(
              widget.logData.sublist(
                _displayedTimeRangeIndices[0],
                _displayedTimeRangeIndices[1],
              ),
            ),
          ],
        ),
      ),
    );
  }

  Widget _topRow(List<LogDataLine> logData) {
    return Padding(
      padding: const EdgeInsets.only(left: 70, right: 70, top: 15, bottom: 10),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          Text(
            'Tank ID: ${logData.first.tankid}',
            style: TextStyle(
              fontWeight: FontWeight.bold,
              fontSize: 20,
            ),
          ),
          Row(
            children: [
              ChartSeriesSelector(onPressed: toggleSeriesView),
              const SizedBox(width: 10),
              TimeRangeSelector(
                onSelected: toggleTimeRange,
                avaliableTimeRange: avaliableTimeRange,
              ),
            ],
          ),
        ],
      ),
    );
  }

  Widget _graph(List<LogDataLine> logData) {
    final List<Map<String, dynamic>> trackballData = [];
    final trackballBehavior = TrackballBehavior(
      enable: true,
      tooltipDisplayMode: TrackballDisplayMode.groupAllPoints,
      activationMode: ActivationMode.singleTap,
      markerSettings: TrackballMarkerSettings(
        markerVisibility: TrackballVisibilityMode.visible,
      ),
      lineColor: Colors.grey.shade600,
      lineWidth: 1.5,
      lineDashArray: [2, 2],
      // builder: _trackballBuilder,
    );

    return Expanded(
      child: SfCartesianChart(
        backgroundColor: Colors.white,
        primaryXAxis: DateTimeAxis(
          title: AxisTitle(text: 'Time'),
          intervalType: timeIntervalType,
          interval: timeInterval,
          dateFormat: timeFormat,
          minimum: displayedTimeRange.start,
          maximum: displayedTimeRange.end,
        ),
        primaryYAxis: NumericAxis(
          name: 'pHAxis',
          title: AxisTitle(text: 'pH Value'),
          isVisible: _showPH,
        ),
        axes: <ChartAxis>[
          NumericAxis(
            name: 'TemperatureAxis',
            title: AxisTitle(text: 'Temperature Value'),
            opposedPosition: true,
            isVisible: _showTemp,
          ),
        ],
        trackballBehavior: trackballBehavior,
        onTrackballPositionChanging: (TrackballArgs details) {
          // Clear previously stored data on each update.
          trackballData.clear();

          // Store the details for the current trackball position.
          if (details.chartPointInfo.series != null &&
              details.chartPointInfo.chartPoint != null) {
            final String seriesName = details.chartPointInfo.series!.name;
            final DateTime date =
                details.chartPointInfo.chartPoint!.x as DateTime;
            final double yValue =
                details.chartPointInfo.chartPoint!.y as double;

            // Add this series' details to the trackballData list.
            trackballData.add({
              'seriesName': seriesName,
              'yValue': yValue,
              'date': date,
            });

            // Construct the tooltip dynamically from the stored data.
            String tooltipText = '';
            String seriesText = '';
            for (final data in trackballData) {
              seriesText += '${data['seriesName']} : ${data['yValue']}';
            }

            // Combine the series data into a compact format
            tooltipText += seriesText;

            // Update the trackball tooltip information
            details.chartPointInfo.label = tooltipText;
            details.chartPointInfo.header =
                DateFormat('MMM d hh:mm a').format(date);
          }
        },
        series: _chartSeries(logData),
      ),
    );
  }

  List<CartesianSeries> _chartSeries(List<LogDataLine> logData) {
    final MarkerSettings markerSettings = MarkerSettings(height: 2, width: 2);

    final subsampledLogData = _subsampleData(logData, scatterGranularity);

    return <CartesianSeries>[
      ScatterSeries<LogDataLine, DateTime>(
        legendItemText: 'pH',
        name: 'pH',
        dataSource: subsampledLogData,
        xValueMapper: (LogDataLine log, _) => log.time,
        yValueMapper: (LogDataLine log, _) => log.phCurrent,
        color: _showPH ? Colors.green : Colors.transparent,
        yAxisName: 'pHAxis',
        animationDuration: 0,
        markerSettings: markerSettings,
        enableTrackball: _showPH,
      ),
      ScatterSeries<LogDataLine, DateTime>(
        legendItemText: 'pH setpoint',
        name: 'pH setpoint',
        dataSource: subsampledLogData,
        xValueMapper: (LogDataLine log, _) => log.time,
        yValueMapper: (LogDataLine log, _) => log.phTarget,
        color: _showPH ? Colors.green.shade800 : Colors.transparent,
        yAxisName: 'pHAxis',
        animationDuration: 0,
        markerSettings: markerSettings,
        enableTrackball: _showPH,
      ),
      ScatterSeries<LogDataLine, DateTime>(
        legendItemText: 'temp',
        name: 'temp',
        dataSource: subsampledLogData,
        xValueMapper: (LogDataLine log, _) => log.time,
        yValueMapper: (LogDataLine log, _) => log.tempMean,
        color: _showTemp ? Colors.blue : Colors.transparent,
        yAxisName: 'TemperatureAxis',
        animationDuration: 0,
        markerSettings: markerSettings,
        enableTrackball: _showTemp,
      ),
      ScatterSeries<LogDataLine, DateTime>(
        legendItemText: 'temp setpoint',
        name: 'temp setpoint',
        dataSource: subsampledLogData,
        xValueMapper: (LogDataLine log, _) => log.time,
        yValueMapper: (LogDataLine log, _) => log.tempTarget,
        color: _showTemp ? Colors.blue.shade800 : Colors.transparent,
        yAxisName: 'TemperatureAxis',
        animationDuration: 0,
        markerSettings: markerSettings,
        enableTrackball: _showTemp,
      ),
    ];
  }

  List<LogDataLine> _subsampleData(List<LogDataLine> logData, int granularity) {
    final List<LogDataLine> subSampledData = [];

    for (int i = 0; i < logData.length; i += scatterGranularity) {
      subSampledData.add(logData[i]);
    }

    return subSampledData;
  }
}
