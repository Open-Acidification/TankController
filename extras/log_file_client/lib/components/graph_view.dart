import 'dart:math';

import 'package:flutter/material.dart';
import 'package:intl/intl.dart';
import 'package:log_file_client/components/chart_series_selector.dart';
import 'package:log_file_client/components/time_range_selector.dart';
import 'package:log_file_client/utils/http_client.dart';
import 'package:syncfusion_flutter_charts/charts.dart';

class GraphView extends StatefulWidget {
  const GraphView({
    required this.logData,
    super.key,
  });

  final List<LogDataLine> logData;

  @override
  State<GraphView> createState() => _GraphViewState();
}

class _GraphViewState extends State<GraphView> {
  bool _showPH = true;
  bool _showTemp = true;
  late final DateTimeRange avaliableTimeRange;
  late List<int> _displayedTimeRange = [
    max(widget.logData.length - 1400, 0),
    widget.logData.length,
  ]; // 1 day default
  late DateTimeIntervalType timeIntervalType;
  late double timeInterval;
  late DateFormat timeFormat;

  @override
  void initState() {
    super.initState();
    avaliableTimeRange = DateTimeRange(
      start: widget.logData.first.time,
      end: widget.logData.last.time,
    );
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
    final ranges = [
      360, // 6 hours
      720, // 12 hours
      1440, // 1 day
      4320, // 3 days
      10080, // 7 days
      43200, // 30 days
      9999999, // Max
    ];

    setState(() {
      if (index < 7) {
        _displayedTimeRange = [
          max(widget.logData.length - ranges[index], 0),
          widget.logData.length,
        ];
      } else if (index == 7) {
        final int endOffset =
            avaliableTimeRange.end.difference(customRange!.end).inMinutes;
        final int endIndex = widget.logData.length - endOffset;
        final int startIndex = endIndex - customRange.duration.inMinutes;
        _displayedTimeRange = [startIndex, endIndex];
      }
      calculateTimeAxisFormat();
    });
  }

  void calculateTimeAxisFormat() {
    timeInterval = 1;

    if (_displayedTimeRange[1] - _displayedTimeRange[0] <= 1440) {
      timeIntervalType = DateTimeIntervalType.hours;
      timeFormat = DateFormat('h a');
    } else if (_displayedTimeRange[1] - _displayedTimeRange[0] <= 4320) {
      timeIntervalType = DateTimeIntervalType.days;
      timeFormat = DateFormat('h a\nMMM d');
      timeInterval = 0.5;
    } else {
      timeIntervalType = DateTimeIntervalType.days;
      timeFormat = DateFormat('MMM d');
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
                _displayedTimeRange[0],
                _displayedTimeRange[1],
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
        backgroundColor: Colors.white,
        primaryXAxis: DateTimeAxis(
          title: AxisTitle(text: 'Time'),
          intervalType: timeIntervalType,
          interval: timeInterval,
          dateFormat: timeFormat,
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
        series: _chartSeries(logData),
      ),
    );
  }

  List<CartesianSeries> _chartSeries(List<LogDataLine> logData) {
    final MarkerSettings markerSettings = MarkerSettings(height: 2, width: 2);

    return <CartesianSeries>[
      ScatterSeries<LogDataLine, DateTime>(
        legendItemText: 'pH',
        name: 'pH',
        dataSource: logData,
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
        dataSource: logData,
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
        dataSource: logData,
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
        dataSource: logData,
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
}
