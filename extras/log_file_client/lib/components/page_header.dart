import 'package:flutter/material.dart';

class PageHeader extends StatelessWidget {
  const PageHeader({required this.text, super.key});
  final String text;

  @override
  Widget build(BuildContext context) {
    final screenWidth = MediaQuery.of(context).size.width;

    return Container(
      margin: EdgeInsets.only(
        top: 40,
        left: 135,
        right: 135,
      ),
      padding: EdgeInsets.only(bottom: 16),
      width: screenWidth,
      decoration: BoxDecoration(
        border: Border(
          bottom: BorderSide(
            color: const Color(0xFFE6E6E6),
            width: 1.5,
          ),
        ),
      ),
      child: Text(
        text,
        style: TextStyle(
          fontSize: 60,
          letterSpacing: -2,
          color: const Color(0xFF0C2D48),
        ),
      ),
    );
  }
}
