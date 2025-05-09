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
  late DateTimeRange selectedRange;

  @override
  void initState() {
    super.initState();
    selectedRange = widget.timeRange;
  }

  void onDatesSelected(DateTimeRange value) {
    setState(() {
      selectedRange = DateTimeRange(
        start: DateTime(
          value.start.year,
          value.start.month,
          value.start.day,
          selectedRange.start.hour,
          selectedRange.start.minute,
        ),
        end: DateTime(
          value.end.year,
          value.end.month,
          value.end.day,
          selectedRange.end.hour,
          selectedRange.end.minute,
        ),
      );
    });
  }

  void onTimeSelected(TimeOfDay time, RangeEndpointType type) {
    if (type == RangeEndpointType.start) {
      selectedRange = DateTimeRange(
        start: DateTime(
          selectedRange.start.year,
          selectedRange.start.month,
          selectedRange.start.day,
          time.hour,
          time.minute,
        ),
        end: selectedRange.end,
      );
    } else if (type == RangeEndpointType.end) {
      selectedRange = DateTimeRange(
        start: selectedRange.start,
        end: DateTime(
          selectedRange.end.year,
          selectedRange.end.month,
          selectedRange.end.day,
          time.hour,
          time.minute,
        ),
      );
    }
  }

  void onApplied() {
    // Ensure the time range is within the available range
    selectedRange = DateTimeRange(
      start: selectedRange.start.isAfter(widget.timeRange.start)
          ? selectedRange.start
          : widget.timeRange.start,
      end: selectedRange.end.isBefore(widget.timeRange.end)
          ? selectedRange.end
          : widget.timeRange.end,
    );

    widget.onApplied(selectedRange);
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
            child: TextButton(onPressed: onApplied, child: const Text('Apply')),
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
          ? DateFormat.yMMMd('en_US').format(selectedRange.start)
          : DateFormat.yMMMd('en_US').format(selectedRange.end),
      style: const TextStyle(fontSize: 16),
    );
  }

  Widget _calendarPicker() {
    return SizedBox(
      width: 300,
      height: 300,
      child: RangeDatePicker(
        centerLeadingDate: true,
        minDate: widget.timeRange.start,
        maxDate: widget.timeRange.end,
        selectedRange: selectedRange,
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
        ? TimeOfDay(
            hour: selectedRange.start.hour,
            minute: selectedRange.start.minute,
          )
        : TimeOfDay(
            hour: selectedRange.end.hour,
            minute: selectedRange.end.minute,
          );

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
