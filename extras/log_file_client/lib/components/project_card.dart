import 'package:flutter/material.dart';
import 'package:log_file_client/utils/http_client.dart';

class ProjectCard extends StatelessWidget {
  const ProjectCard({required this.project, required this.onTap, super.key});

  final Project project;
  final void Function() onTap;
  @override
  Widget build(BuildContext context) {
    return MouseRegion(
      cursor: SystemMouseCursors.click,
      child: GestureDetector(
        onTap: onTap,
        child: LayoutBuilder(
          builder: (BuildContext context, BoxConstraints constraints) {
            // Decide sizes of internal components based on card width
            final double cardWidth = constraints.maxWidth * 0.93;
            final double imageSize = cardWidth * 0.5;
            final double titleFontSize = cardWidth * 0.05;
            final double descriptionFontSize = cardWidth * 0.04;

            return Container(
              margin: EdgeInsets.all(cardWidth * 0.075),
              decoration: _cardBackground(),
              child: Column(
                children: [
                  _projectImage(imageSize, cardWidth),
                  _projectTitle(cardWidth, titleFontSize),
                  _projectDescription(descriptionFontSize),
                ],
              ),
            );
          },
        ),
      ),
    );
  }

  Container _projectImage(double imageSize, double cardWidth) {
    return Container(
      width: imageSize,
      height: imageSize,
      margin: EdgeInsets.only(top: cardWidth * 0.11, bottom: cardWidth * 0.075),
      decoration: BoxDecoration(
        borderRadius: BorderRadius.circular(cardWidth * 0.25),
        image: DecorationImage(
          image: AssetImage('./lib/assets/placeholder.png'),
          fit: BoxFit.cover,
        ),
      ),
    );
  }

  Padding _projectTitle(double cardWidth, double titleFontSize) {
    return Padding(
      padding: EdgeInsets.only(bottom: cardWidth * 0.02),
      child: Text(
        project.name,
        style: TextStyle(
          fontSize: titleFontSize,
          fontWeight: FontWeight.bold,
          color: const Color(0xFF0C2D48),
        ),
      ),
    );
  }

  Text _projectDescription(double descriptionFontSize) {
    return Text(
      'Project Description',
      style: TextStyle(
        fontSize: descriptionFontSize,
        color: const Color(0xFF6D6D6D),
      ),
    );
  }

  BoxDecoration _cardBackground() {
    return BoxDecoration(
      borderRadius: BorderRadius.circular(24.0),
      border: Border.all(color: const Color(0xFFE6E6E6)),
      color: const Color(0xFFFAFAF5),
    );
  }
}
