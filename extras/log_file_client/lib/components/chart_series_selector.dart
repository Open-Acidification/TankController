import 'package:flutter/material.dart';

class ChartSeriesSelector extends StatefulWidget {
  const ChartSeriesSelector({required this.onPressed, super.key});
  final void Function(int index) onPressed;

  @override
  State<ChartSeriesSelector> createState() => _ChartSeriesSelectorState();
}

class _ChartSeriesSelectorState extends State<ChartSeriesSelector> {
  List<bool> buttonState = [true, true];

  void toggleButton(int index) {
    setState(() {
      buttonState[index] = !buttonState[index];
      widget.onPressed(index);
    });
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      padding: const EdgeInsets.all(4),
      decoration: BoxDecoration(
        borderRadius: BorderRadius.circular(8),
        // color: Colors.grey[500],
      ),
      child: SizedBox(
        child: Row(
          mainAxisAlignment: MainAxisAlignment.center,
          mainAxisSize: MainAxisSize.min,
          children: [
            _halfButton('pH', Colors.green, 0),
            _halfButton('temp', Colors.blue, 1),
          ],
        ),
      ),
    );
  }

  TextButton _halfButton(String text, Color color, int index) {
    return TextButton(
      key: Key(text),
      onPressed: () {
        toggleButton(index);
      },
      style: _buttonStyle(color, index),
      child: _buttonText(text, color, index),
    );
  }

  ButtonStyle _buttonStyle(Color color, int index) {
    return ButtonStyle(
      backgroundColor: buttonState[index]
          ? WidgetStateProperty.all<Color>(color)
          : WidgetStateProperty.all<Color>(Colors.white),
      shape: WidgetStateProperty.all<OutlinedBorder>(
        RoundedRectangleBorder(
          borderRadius: index == 0
              ? BorderRadius.only(
                  topLeft: Radius.circular(8),
                  bottomLeft: Radius.circular(8),
                )
              : BorderRadius.only(
                  topRight: Radius.circular(8),
                  bottomRight: Radius.circular(8),
                ),
        ),
      ),
    );
  }

  Widget _buttonText(String text, Color color, int index) {
    return SizedBox(
      width: 40,
      // height: 40,
      child: Center(
        child: Text(
          text,
          style: TextStyle(color: buttonState[index] ? Colors.white : color),
        ),
      ),
    );
  }
}
