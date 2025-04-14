import 'package:flutter/material.dart';

class PageHeader extends StatelessWidget {
  const PageHeader({required this.text, super.key});
  final String text;

  @override
  Widget build(BuildContext context) {
    final screenWidth = MediaQuery.of(context).size.width;
    final double fontSize = screenWidth >= 500 ? 60 : 40;
    final double sideMargins = screenWidth >= 500 ? 135 : 40;
    final double topMargin = screenWidth >= 500 ? 40 : 30;

    return Container(
      margin: EdgeInsets.only(
        top: topMargin,
        left: sideMargins,
        right: sideMargins,
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
          fontSize: fontSize,
          letterSpacing: -2,
          color: const Color(0xFF0C2D48),
        ),
      ),
    );
  }
}
