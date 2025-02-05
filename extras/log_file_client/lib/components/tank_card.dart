import 'dart:async';
import 'package:flutter/material.dart';
import 'package:log_file_client/components/tank_thumbnail.dart';
import 'package:log_file_client/utils/http_client.dart';
import 'package:skeletonizer/skeletonizer.dart';

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

  Widget _graphThumbnailBuilder(
    Future<TankSnapshot> tankSnapshot,
    double cardWidth,
  ) {
    return FutureBuilder(
      future: tankSnapshot,
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return _skeletonLoaderGraph(cardWidth);
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

  Skeletonizer _skeletonLoaderGraph(double cardWidth) {
    return Skeletonizer(
      effect: ShimmerEffect(
        baseColor: Colors.grey[200]!,
        highlightColor: Colors.grey[100]!,
        duration: Duration(seconds: 2),
      ),
      child: _graphThumbnail(cardWidth, null),
    );
  }

  Container _graphThumbnail(
    double cardWidth,
    AsyncSnapshot<TankSnapshot>? snapshot,
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
        child: snapshot != null
            ? TankThumbnail(snapshot: snapshot.data!)
            : Container(
                decoration: BoxDecoration(
                  image: DecorationImage(
                    image: AssetImage(
                      './lib/assets/placeholder.png',
                    ),
                    fit: BoxFit.cover,
                  ),
                ),
              ),
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

  Widget _tankInfoBuilder(
    Future<TankSnapshot> tankSnapshot,
    double cardWidth,
    double descriptionFontSize,
  ) {
    return FutureBuilder(
      future: tankSnapshot,
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return _skeletonLoaderInfo(cardWidth, descriptionFontSize);
        } else if (snapshot.hasError) {
          return Center(child: Text('Error: ${snapshot.error}'));
        } else {
          return _tankInfo(cardWidth, descriptionFontSize, snapshot);
        }
      },
    );
  }

  Skeletonizer _skeletonLoaderInfo(
    double cardWidth,
    double descriptionFontSize,
  ) {
    return Skeletonizer(
      textBoneBorderRadius: TextBoneBorderRadius(BorderRadius.circular(4)),
      effect: ShimmerEffect(
        baseColor: Colors.grey[200]!,
        highlightColor: Colors.grey[100]!,
        duration: Duration(seconds: 2),
      ),
      child: _tankInfo(cardWidth, descriptionFontSize, null),
    );
  }

  Widget _tankInfo(
    double cardWidth,
    double descriptionFontSize,
    AsyncSnapshot<TankSnapshot>? snapshot,
  ) {
    return Padding(
      padding: EdgeInsets.only(top: cardWidth * 0.06),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        children: [
          _tankInfoLine('ph', descriptionFontSize, snapshot),
          _tankInfoLine('temp', descriptionFontSize, snapshot),
        ],
      ),
    );
  }

  Row _tankInfoLine(
    String type,
    double descriptionFontSize,
    AsyncSnapshot<TankSnapshot>? snapshot,
  ) {
    return Row(
      children: [
        Skeleton.keep(
          child: Icon(
            type == 'ph' ? Icons.water_drop : Icons.thermostat,
            color: type == 'ph' ? Colors.green : Colors.blue,
            size: descriptionFontSize,
          ),
        ),
        Text(
          type == 'ph'
              ? 'pH ${snapshot?.data!.pH ?? 'mock'}'
              : '${snapshot?.data!.temperature ?? 'mock'}°C',
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
