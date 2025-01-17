import 'dart:async';
import 'package:flutter/material.dart';
import 'package:log_file_client/components/tank_thumbnail.dart';
import 'package:log_file_client/utils/http_client.dart';

class TankCard extends StatelessWidget {
  const TankCard({
    required this.log,
    required this.onTap,
    required this.httpClient,
    super.key,
  });

  final Log log;
  final void Function() onTap;
  final HttpClient httpClient;

  Future<TankSnapshot> getTankSnapshot() async {
    final snapshot = await httpClient.getTankSnapshot(log);
    return snapshot;
  }

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
            final double titleFontSize = cardWidth * 0.05;
            final double descriptionFontSize = cardWidth * 0.04;

            // ignore: discarded_futures
            final Future<TankSnapshot> tankSnapshot = getTankSnapshot();

            return Container(
              margin: EdgeInsets.all(cardWidth * 0.075),
              decoration: _cardBackground(),
              child: Column(
                children: [
                  _graphThumbnailBuilder(tankSnapshot, cardWidth),
                  _tankName(titleFontSize),
                  _tankInfoBuilder(
                    tankSnapshot,
                    cardWidth,
                    descriptionFontSize,
                  ),
                ],
              ),
            );
          },
        ),
      ),
    );
  }

  FutureBuilder<TankSnapshot> _graphThumbnailBuilder(
    Future<TankSnapshot> tankSnapshot,
    double cardWidth,
  ) {
    return FutureBuilder(
      future: tankSnapshot,
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return Center(child: CircularProgressIndicator());
        } else if (snapshot.hasError) {
          return Center(
            child: Text(
              'Error: ${snapshot.error}',
            ),
          );
        } else {
          return _graphThumbnail(cardWidth, snapshot);
        }
      },
    );
  }

  Container _graphThumbnail(
    double cardWidth,
    AsyncSnapshot<TankSnapshot> snapshot,
  ) {
    return Container(
      width: cardWidth,
      height: cardWidth * 0.6,
      margin: EdgeInsets.only(
        bottom: cardWidth * 0.05,
      ),
      decoration: BoxDecoration(
        borderRadius: BorderRadius.vertical(
          top: Radius.circular(20),
        ),
      ),
      child: ClipRRect(
        borderRadius: BorderRadius.vertical(
          top: Radius.circular(20),
        ),
        child: TankThumbnail(snapshot: snapshot.data!),
      ),
    );
  }

  Text _tankName(double titleFontSize) {
    return Text(
      log.name,
      style: TextStyle(
        fontSize: titleFontSize,
        fontWeight: FontWeight.bold,
        color: const Color(0xFF0C2D48),
      ),
    );
  }

  FutureBuilder<TankSnapshot> _tankInfoBuilder(
    Future<TankSnapshot> tankSnapshot,
    double cardWidth,
    double descriptionFontSize,
  ) {
    return FutureBuilder(
      future: tankSnapshot,
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return Center(
            child: Padding(
              padding: EdgeInsets.only(
                left: cardWidth * 0.2,
                right: cardWidth * 0.2,
                top: cardWidth * 0.05,
              ),
              child: LinearProgressIndicator(),
            ),
          );
        } else if (snapshot.hasError) {
          return Center(child: Text('Error: ${snapshot.error}'));
        } else {
          return _tankInfo(cardWidth, descriptionFontSize, snapshot);
        }
      },
    );
  }

  Padding _tankInfo(
    double cardWidth,
    double descriptionFontSize,
    AsyncSnapshot<TankSnapshot> snapshot,
  ) {
    return Padding(
      padding: EdgeInsets.only(top: cardWidth * 0.06),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        children: [
          _tankInfoTemp(descriptionFontSize, snapshot),
          _tankInfoPH(descriptionFontSize, snapshot),
        ],
      ),
    );
  }

  Row _tankInfoTemp(
    double descriptionFontSize,
    AsyncSnapshot<TankSnapshot> snapshot,
  ) {
    return Row(
      children: [
        Icon(
          Icons.thermostat,
          size: descriptionFontSize,
        ),
        Text(
          '${snapshot.data!.temperature}°C',
          style: TextStyle(
            fontSize: descriptionFontSize,
            color: const Color(0xFF6D6D6D),
          ),
        ),
      ],
    );
  }

  Row _tankInfoPH(
    double descriptionFontSize,
    AsyncSnapshot<TankSnapshot> snapshot,
  ) {
    return Row(
      children: [
        Icon(
          Icons.water_drop,
          size: descriptionFontSize,
        ),
        Text(
          'pH ${snapshot.data!.pH}',
          style: TextStyle(
            fontSize: descriptionFontSize,
            color: const Color(0xFF6D6D6D),
          ),
        ),
      ],
    );
  }

  BoxDecoration _cardBackground() {
    return BoxDecoration(
      borderRadius: BorderRadius.circular(24.0),
      border: Border.all(
        color: const Color(0xFFE6E6E6),
      ),
      color: const Color(0xFFFAFAF5),
    );
  }
}
