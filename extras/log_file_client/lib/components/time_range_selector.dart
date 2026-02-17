import 'dart:async';

import 'package:flutter/material.dart';
import 'package:log_file_client/components/custom_time_range_picker.dart';
import 'package:popover/popover.dart';

class TimeRangeSelector extends StatefulWidget {
  const TimeRangeSelector({
    required this.onSelected,
    required this.avaliableTimeRange,
    super.key,
  });
  final void Function(int, DateTimeRange?) onSelected;
  final DateTimeRange avaliableTimeRange;

  @override
  State<TimeRangeSelector> createState() => _TimeRangeSelectorState();
}

class _TimeRangeSelectorState extends State<TimeRangeSelector> {
  final List<bool> _isSelected = [
    false,
    false,
    true,
    false,
    false,
    false,
    false,
    false,
  ];

  Future<void> onPressed(int index, BuildContext context) async {
    if (index < 7) {
      setState(() {
        _isSelected.fillRange(0, _isSelected.length, false);
        _isSelected[index] = true;
        widget.onSelected(index, null);
      });
    }
    if (index == 7) {
      late DateTimeRange timeRange;
      await showPopover(
        context: context,
        bodyBuilder: (context) => CustomTimeRangePicker(
          timeRange: widget.avaliableTimeRange,
          onApplied: (value) {
            timeRange = value;
            Navigator.of(context).pop();
          },
        ),
      );
      setState(() {
        _isSelected.fillRange(0, _isSelected.length, false);
        _isSelected[index] = true;
        widget.onSelected(index, timeRange);
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      padding: const EdgeInsets.all(4),
      decoration: BoxDecoration(
        color: Colors.grey[100],
        borderRadius: BorderRadius.circular(8),
      ),
      child: IntrinsicHeight(
        child: Row(
          mainAxisAlignment: MainAxisAlignment.center,
          mainAxisSize: MainAxisSize.min,
          children: [
            _timeRangeOption('6H', 0),
            _timeRangeOption('12H', 1),
            _timeRangeOption('24H', 2),
            _timeRangeOption('3D', 3),
            _timeRangeOption('1W', 4),
            _timeRangeOption('1M', 5),
            _timeRangeOption('Max', 6),
            _verticalDivider(),
            _timeRangeOption('Custom', 7),
          ],
        ),
      ),
    );
  }

  Padding _verticalDivider() {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 4),
      child: VerticalDivider(
        color: Colors.grey[300],
        width: 1,
        indent: 4,
        endIndent: 4,
      ),
    );
  }

  Widget _timeRangeOption(String text, int index) {
    final active = _isSelected[index];
    return MouseRegion(
      cursor: SystemMouseCursors.click,
      child: Builder(
        builder: (context) {
          return GestureDetector(
            onTap: () => unawaited(onPressed(index, context)),
            child: Container(
              padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 4),
              decoration: BoxDecoration(
                color: active ? Colors.white : Colors.grey[100],
                borderRadius: BorderRadius.circular(8),
                boxShadow: active
                    ? [
                        BoxShadow(
                          color: Colors.grey[300]!,
                          blurRadius: 4,
                          offset: const Offset(0, 2),
                        ),
                      ]
                    : null,
              ),
              child: Text(
                text,
                style: TextStyle(
                  color: active ? Colors.black : Colors.grey[600],
                ),
              ),
            ),
          );
        },
      ),
    );
  }
}
