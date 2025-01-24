import 'package:flutter/material.dart';

class OptionSelector extends StatefulWidget {
  const OptionSelector({super.key});

  @override
  State<OptionSelector> createState() => _OptionSelectorState();
}

class _OptionSelectorState extends State<OptionSelector> {
  List<bool> isSelected = [true, true];

  @override
  Widget build(BuildContext context) {
    return ToggleButtons(
      borderRadius: BorderRadius.circular(10),
      borderColor: Colors.grey,
      selectedBorderColor: Colors.blue,
      selectedColor: Colors.white,
      fillColor: Colors.blue,
      splashColor: null,
      textStyle: const TextStyle(fontSize: 12, fontWeight: FontWeight.bold),
      constraints: const BoxConstraints(minHeight: 40, minWidth: 80),
      isSelected: isSelected,
      onPressed: (int index) {
        setState(() {
          if (index == 0 || index == 1) {
            isSelected[index] = !isSelected[index];
          }
        });
      },
      children: const [
        Text(
          'pH',
          style: TextStyle(color: Colors.green),
        ),
        Text('temp', style: TextStyle(color: Colors.blue)),
      ],
    );
  }
}
