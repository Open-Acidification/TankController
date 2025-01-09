import 'package:flutter/material.dart';

class ProjectCard extends StatelessWidget {
  const ProjectCard({super.key});

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(
        builder: (BuildContext context, BoxConstraints constraints) {
      // Access the available width and height of the widget
      final double cardWidth = constraints.maxWidth * 0.93;

      // Decide sizes of internal components based on card width
      final double imageSize = cardWidth * 0.5;
      final double titleFontSize = cardWidth * 0.05;
      final double descriptionFontSize = cardWidth * 0.04;

      return Container(
        margin: EdgeInsets.all(cardWidth * 0.075),
        decoration: BoxDecoration(
          borderRadius: BorderRadius.circular(24.0),
          border: Border.all(
            color: const Color(0xFFE6E6E6),
          ),
          color: const Color(0xFFFAFAF5),
        ),
        child: Column(
          children: [
            Container(
              width: imageSize,
              height: imageSize,
              margin: EdgeInsets.only(
                  top: cardWidth * 0.11, bottom: cardWidth * 0.075),
              decoration: BoxDecoration(
                borderRadius: BorderRadius.circular(cardWidth * 0.25),
                image: DecorationImage(
                  image: AssetImage(
                    './lib/assets/placeholder.png',
                  ),
                  fit: BoxFit.cover,
                ),
              ),
            ),
            Padding(
              padding: EdgeInsets.only(bottom: cardWidth * 0.02),
              child: Text(
                'Project Name',
                style: TextStyle(
                  fontSize: titleFontSize,
                  fontWeight: FontWeight.bold,
                  color: const Color(0xFF0C2D48),
                ),
              ),
            ),
            Text(
              'Project Description',
              style: TextStyle(
                fontSize: descriptionFontSize,
                color: const Color(0xFF6D6D6D),
              ),
            ),
          ],
        ),
      );
    });
  }
}
