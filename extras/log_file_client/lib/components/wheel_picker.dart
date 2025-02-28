import 'package:flutter/material.dart';
import 'package:wheel_picker/wheel_picker.dart';

enum WheelType { hours, minutes, amPm }

class MyWheelPicker extends StatefulWidget {
  const MyWheelPicker({
    required this.selectedTime,
    required this.onTimeSelected,
    super.key,
  });

  final TimeOfDay selectedTime;
  final void Function(TimeOfDay) onTimeSelected;

  @override
  State<MyWheelPicker> createState() => _MyWheelPickerState();
}

class _MyWheelPickerState extends State<MyWheelPicker> {
  late TimeOfDay selectedTime;
  late String selectedAmPm = selectedTime.hour < 12 ? 'AM' : 'PM';

  @override
  void initState() {
    super.initState();
    selectedTime = widget.selectedTime;
  }

  void onIndexChanged(int index, WheelType type) {
    switch (type) {
      case WheelType.hours:
        selectedTime = TimeOfDay(
          hour: (selectedAmPm == 'PM') ? index + 12 : index,
          minute: selectedTime.minute,
        );
        break;
      case WheelType.minutes:
        selectedTime = TimeOfDay(
          hour: selectedTime.hour,
          minute: index,
        );
        break;
      case WheelType.amPm:
        selectedAmPm = ['AM', 'PM'][index];
        selectedTime = TimeOfDay(
          hour: selectedTime.hour,
          minute: selectedTime.minute,
        );
        break;
    }
    widget.onTimeSelected(selectedTime);
  }

  late final _hoursWheelController = WheelPickerController(
    itemCount: 12,
    initialIndex: selectedTime.hour % 12,
  );
  late final _minutesWheelController = WheelPickerController(
    itemCount: 60,
    initialIndex: selectedTime.minute,
    mounts: [_hoursWheelController],
  );

  @override
  Widget build(BuildContext context) {
    const textStyle = TextStyle(
      fontSize: 16,
      fontWeight: FontWeight.normal,
      height: 1.5,
      color: Colors.black,
      decoration: TextDecoration.none,
    );
    final wheelStyle = WheelPickerStyle(
      itemExtent: textStyle.fontSize! * textStyle.height!, // Text height
      squeeze: 1.25,
      diameterRatio: .8,
      surroundingOpacity: .25,
    );

    Widget itemBuilder(BuildContext context, int index) {
      return Text('$index'.padLeft(2, '0'), style: textStyle);
    }

    final hourWheel = Expanded(
      child: WheelPicker(
        builder: itemBuilder,
        controller: _hoursWheelController,
        looping: false,
        style: wheelStyle,
        selectedIndexColor: Colors.black,
        onIndexChanged: (index, interactionType) {
          onIndexChanged(index, WheelType.hours);
        },
      ),
    );

    final minuteWheel = Expanded(
      child: WheelPicker(
        builder: itemBuilder,
        controller: _minutesWheelController,
        looping: true,
        style: wheelStyle,
        selectedIndexColor: Colors.black,
        onIndexChanged: (index, interactionType) {
          onIndexChanged(index, WheelType.minutes);
        },
      ),
    );

    final amPmWheel = Expanded(
      child: WheelPicker(
        itemCount: 2,
        builder: (context, index) {
          return Text(['AM', 'PM'][index], style: textStyle);
        },
        initialIndex: (selectedTime.period == DayPeriod.am) ? 0 : 1,
        onIndexChanged: (index, interactionType) {
          onIndexChanged(index, WheelType.amPm);
        },
        looping: false,
        style: wheelStyle.copyWith(
          shiftAnimationStyle: const WheelShiftAnimationStyle(
            duration: Duration(seconds: 1),
            curve: Curves.bounceOut,
          ),
        ),
      ),
    );

    return Center(
      child: SizedBox(
        width: 125,
        height: 150,
        child: Stack(
          fit: StackFit.expand,
          children: [
            _centerBar(context),
            Padding(
              padding: const EdgeInsets.symmetric(horizontal: 15.0),
              child: Row(
                children: [
                  hourWheel,
                  const Text(':', style: textStyle),
                  minuteWheel,
                  amPmWheel,
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }

  @override
  void dispose() {
    _hoursWheelController.dispose();
    _minutesWheelController.dispose();
    super.dispose();
  }

  Widget _centerBar(BuildContext context) {
    return Center(
      child: Container(
        height: 30,
        decoration: BoxDecoration(
          color: const Color.fromARGB(255, 116, 126, 202).withAlpha(26),
          borderRadius: BorderRadius.circular(8.0),
        ),
      ),
    );
  }
}
