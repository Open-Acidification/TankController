import 'package:flutter/material.dart';

class ChartSeriesSelector extends StatefulWidget {
  const ChartSeriesSelector({required this.onPressed, super.key});
  final void Function(int) onPressed;

  @override
  State<ChartSeriesSelector> createState() => _ChartSeriesSelectorState();
}

class _ChartSeriesSelectorState extends State<ChartSeriesSelector> {
  final List<bool> _buttonState = [true, true];

  void toggleButton(int index) {
    setState(() {
      _buttonState[index] = !_buttonState[index];
      widget.onPressed(index);
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
      child: SizedBox(
        child: Row(
          mainAxisAlignment: MainAxisAlignment.center,
          mainAxisSize: MainAxisSize.min,
          children: [
            _chartSeriesOption('pH', Colors.green, 0),
            SizedBox(width: 4),
            _chartSeriesOption('temp', Colors.blue, 1),
          ],
        ),
      ),
    );
  }

   Widget _chartSeriesOption(String text, Color color, int index) {
    final active = _buttonState[index];
    return MouseRegion(
      cursor: SystemMouseCursors.click,
      child: GestureDetector(
        onTap: () => toggleButton(index),
        child: Container(
          padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 4),
          decoration: BoxDecoration(
            color: active ? color : Colors.grey[100],
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
            style: TextStyle(color: active ? Colors.white : color),
          ),
        ),
      ),
    );
  }
}
