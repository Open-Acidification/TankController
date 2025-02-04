import 'package:flutter/material.dart';

class ToggleButton extends StatefulWidget {
  const ToggleButton({required this.onPressed, super.key});
  final void Function(int index) onPressed;

  @override
  State<ToggleButton> createState() => _ToggleButtonState();
}

class _ToggleButtonState extends State<ToggleButton> {
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
      decoration: BoxDecoration(
        borderRadius: BorderRadius.circular(10),
        color: Colors.grey[500],
      ),
      padding: const EdgeInsets.all(0.5),
      child: SizedBox(
        width: 148,
        child: Row(
          mainAxisAlignment: MainAxisAlignment.center,
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
              ? BorderRadius.only(topLeft: Radius.circular(10), bottomLeft: Radius.circular(10))
              : BorderRadius.only(topRight: Radius.circular(10), bottomRight: Radius.circular(10)),
        ),
      ),
    );
  }

  Widget _buttonText(String text, Color color, int index) {
    return SizedBox(
      width: 50,
      height: 40,
      child: Center(
        child: Text(
          text,
          style: TextStyle(color: buttonState[index] ? Colors.white : color),
        ),
      ),
    );
  }
}
