import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:popover/popover.dart';

class AdvancedOptionsDropdown extends StatelessWidget {
  const AdvancedOptionsDropdown({
    required this.tempController,
    required this.phController,
    super.key,
  });

  final TextEditingController tempController;
  final TextEditingController phController;

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.only(right: 16.0),
      child: Builder(
        builder: (buttonContext) {
          return MouseRegion(
            cursor: SystemMouseCursors.click,
            child: GestureDetector(
              onTap: () async {
                await showPopover(
                  context: buttonContext,
                  bodyBuilder: _popoverBuilder,
                  direction: PopoverDirection.bottom,
                  width: 160,
                  arrowHeight: 0,
                  backgroundColor: Theme.of(context).cardColor,
                  radius: 8,
                );
              },
              child: Row(
                mainAxisSize: MainAxisSize.min,
                children: [
                  Text(
                    'Advanced Options',
                    style: Theme.of(context).textTheme.titleMedium,
                  ),
                  const SizedBox(width: 4),
                  const Icon(Icons.arrow_drop_down),
                ],
              ),
            ),
          );
        },
      ),
    );
  }

  Widget _popoverBuilder(context) => Padding(
        padding: const EdgeInsets.all(12),
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            _decimalTextField(tempController, 'Temperature Deviation'),
            const SizedBox(height: 8),
            _decimalTextField(phController, 'pH Deviation'),
          ],
        ),
      );

  TextFormField _decimalTextField(
    TextEditingController controller,
    String label,
  ) {
    return TextFormField(
      controller: controller,
      keyboardType: const TextInputType.numberWithOptions(decimal: true),
      decoration: InputDecoration(labelText: label),
      inputFormatters: [
        FilteringTextInputFormatter.allow(RegExp(r'^\d*\.?\d*')),
      ],
    );
  }
}
