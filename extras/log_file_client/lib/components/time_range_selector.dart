import 'package:flutter/material.dart';

class TimeRangeSelector extends StatefulWidget {
  const TimeRangeSelector({required this.onSelected, super.key});
  final Function(int) onSelected;

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

  void onPressed(int index) {
    setState(() {
      _isSelected.fillRange(0, _isSelected.length, false);
      _isSelected[index] = true;
      widget.onSelected(index);
    });
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
      child: GestureDetector(
        onTap: () => onPressed(index),
        child: Container(
          padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 4),
          decoration: BoxDecoration(
            color: active ? Colors.white : Colors.grey[100],
            borderRadius: BorderRadius.circular(8),
          ),
          child: Text(
            text,
            style: TextStyle(color: active ? Colors.black : Colors.grey[600]),
          ),
        ),
      ),
    );
  }
}
