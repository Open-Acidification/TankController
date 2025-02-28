import 'package:date_picker_plus/date_picker_plus.dart';
import 'package:flutter/material.dart';
import 'package:intl/intl.dart';
import 'package:log_file_client/components/wheel_picker.dart';

enum RangeEndpointType { start, end }

class CustomTimeRangePicker extends StatefulWidget {
  const CustomTimeRangePicker({
    required this.timeRange,
    required this.onApplied,
    super.key,
  });

  final DateTimeRange timeRange;
  final void Function(DateTimeRange) onApplied;

  @override
  State<CustomTimeRangePicker> createState() => _CustomTimeRangePickerState();
}

class _CustomTimeRangePickerState extends State<CustomTimeRangePicker> {
  DateTime? minTime;
  DateTime? maxTime;

  @override
  void initState() {
    super.initState();
    minTime = widget.timeRange.start;
    maxTime = widget.timeRange.end;
  }

  void onDatesSelected(DateTimeRange value) {
    minTime = DateTime(
      value.start.year,
      value.start.month,
      value.start.day,
      minTime!.hour,
      minTime!.minute,
    );
    maxTime = DateTime(
      value.end.year,
      value.end.month,
      value.end.day,
      maxTime!.hour,
      maxTime!.minute,
    );
  }

  void onTimeSelected(TimeOfDay time, RangeEndpointType type) {
    if (type == RangeEndpointType.start) {
      minTime = DateTime(
        minTime!.year,
        minTime!.month,
        minTime!.day,
        time.hour,
        time.minute,
      );
    } else if (type == RangeEndpointType.end) {
      maxTime = DateTime(
        maxTime!.year,
        maxTime!.month,
        maxTime!.day,
        time.hour,
        time.minute,
      );
    }
  }

  void onApplied() {
    widget.onApplied(DateTimeRange(start: minTime!, end: maxTime!));
  }

  @override
  Widget build(BuildContext context) {
    return IntrinsicHeight(
      child: Column(
        children: [
          Row(
            mainAxisSize: MainAxisSize.min,
            children: [
              _calendarPicker(),
              const SizedBox(width: 28),
              _rangeDisplay(),
              const SizedBox(width: 28),
            ],
          ),
          Padding(
            padding: const EdgeInsets.only(bottom: 8),
            child: TextButton(
              onPressed: onApplied,
              child: const Text('Apply'),
            ),
          ),
        ],
      ),
    );
  }

  Widget _rangeDisplay() {
    return Column(
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        const SizedBox(height: 16),
        _rangeDisplayItem(RangeEndpointType.start),
        const SizedBox(height: 32),
        _rangeDisplayItem(RangeEndpointType.end),
      ],
    );
  }

  Widget _rangeDisplayItem(RangeEndpointType type) {
    return Column(
      mainAxisAlignment: MainAxisAlignment.center,
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          type == RangeEndpointType.start ? 'From' : 'To',
          style: const TextStyle(
            fontSize: 12,
            fontWeight: FontWeight.bold,
            color: Colors.grey,
          ),
        ),
        const SizedBox(height: 4),
        _dateDisplay(type),
        const SizedBox(height: 4),
        _timePicker(type),
      ],
    );
  }

  Widget _dateDisplay(RangeEndpointType type) {
    return Text(
      type == RangeEndpointType.start
          ? DateFormat.yMMMd('en_US').format(minTime!)
          : DateFormat.yMMMd('en_US').format(maxTime!),
      style: const TextStyle(fontSize: 16),
    );
  }

  Widget _calendarPicker() {
    return SizedBox(
      width: 300,
      height: 300,
      child: RangeDatePicker(
        centerLeadingDate: true,
        minDate: minTime!,
        maxDate: maxTime!,
        onRangeSelected: onDatesSelected,
        splashColor: Colors.transparent,
        daysOfTheWeekTextStyle: TextStyle(
          fontSize: 12,
          fontWeight: FontWeight.bold,
          color: Colors.grey.shade400,
        ),
        enabledCellsTextStyle: const CellTextStyle(),
        disabledCellsTextStyle: const CellTextStyle(color: Colors.grey),
        selectedCellsTextStyle: const CellTextStyle(),
        singleSelectedCellTextStyle: const CellTextStyle(color: Colors.white),
        currentDateTextStyle: const CellTextStyle(),
      ),
    );
  }

  Widget _timePicker(RangeEndpointType type) {
    final TimeOfDay selectedTime = type == RangeEndpointType.start
        ? TimeOfDay(hour: minTime!.hour, minute: minTime!.minute)
        : TimeOfDay(hour: maxTime!.hour, minute: maxTime!.minute);

    return SizedBox(
      height: 100,
      child: MyWheelPicker(
        selectedTime: selectedTime,
        onTimeSelected: (time) => onTimeSelected(time, type),
      ),
    );
  }
}

class CellTextStyle extends TextStyle {
  const CellTextStyle({super.color, fontSize = 16});
}
