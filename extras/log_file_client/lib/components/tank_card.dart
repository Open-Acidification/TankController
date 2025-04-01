import 'dart:async';
import 'package:flutter/material.dart';
import 'package:log_file_client/components/tank_thumbnail.dart';
import 'package:log_file_client/utils/http_client.dart';
import 'package:skeletonizer/skeletonizer.dart';

enum TankInfoType { pH, temp }

enum TankInfoMode { current, range }

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
            final double cardWidth = constraints.maxWidth;
            final double titleFontSize = 20;
            final double tankInfoFontSize = 16;
            final double tankInfoHeaderFontSize = 14;

            // ignore: discarded_futures
            final Future<TankSnapshot> tankSnapshot = getTankSnapshot();

            return Container(
              margin: EdgeInsets.all(30),
              decoration: _cardBackground(),
              child: Column(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  _graphThumbnailBuilder(tankSnapshot, cardWidth),
                  _tankName(titleFontSize),
                  _tankInfoBuilder(
                    tankSnapshot,
                    tankInfoFontSize,
                    tankInfoHeaderFontSize,
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

  Widget _graphThumbnail(
    double cardWidth,
    AsyncSnapshot<TankSnapshot>? snapshot,
  ) {
    return SizedBox(
      height: cardWidth * 0.53,
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
    double tankInfoFontSize,
    double tankInfoHeaderFontSize,
  ) {
    return FutureBuilder(
      future: tankSnapshot,
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return _skeletonLoaderInfo(
            tankInfoFontSize,
            tankInfoHeaderFontSize,
          );
        } else if (snapshot.hasError) {
          return Center(child: Text('Error: ${snapshot.error}'));
        } else {
          return _tankInfo(
            tankInfoFontSize,
            tankInfoHeaderFontSize,
            snapshot,
          );
        }
      },
    );
  }

  Skeletonizer _skeletonLoaderInfo(
    double tankInfoFontSize,
    double tankInfoHeaderFontSize,
  ) {
    return Skeletonizer(
      textBoneBorderRadius: TextBoneBorderRadius(BorderRadius.circular(4)),
      effect: ShimmerEffect(
        baseColor: Colors.grey[200]!,
        highlightColor: Colors.grey[100]!,
        duration: Duration(seconds: 2),
      ),
      child: _tankInfo(tankInfoFontSize, tankInfoHeaderFontSize, null),
    );
  }

  Widget _tankInfo(
    double tankInfoFontSize,
    double tankInfoHeaderFontSize,
    AsyncSnapshot<TankSnapshot>? snapshot,
  ) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 15),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        children: [
          _tankInfoBlock(
            TankInfoMode.current,
            tankInfoFontSize,
            tankInfoHeaderFontSize,
            snapshot,
          ),
          _tankInfoBlock(
            TankInfoMode.range,
            tankInfoFontSize,
            tankInfoHeaderFontSize,
            snapshot,
          ),
        ],
      ),
    );
  }

  Widget _tankInfoBlock(
    TankInfoMode mode,
    double tankInfoFontSize,
    double tankInfoHeaderFontSize,
    AsyncSnapshot<TankSnapshot>? snapshot,
  ) {
    return Column(
      children: [
        _tankInfoHeader(mode, tankInfoHeaderFontSize),
        _tankInfoLine(TankInfoType.pH, mode, tankInfoFontSize, snapshot),
        _tankInfoLine(TankInfoType.temp, mode, tankInfoFontSize, snapshot),
      ],
    );
  }

  Widget _tankInfoHeader(TankInfoMode mode, double tankInfoHeaderFontSize) {
    return Text(
      mode == TankInfoMode.current ? 'Current' : '12hr Range',
      style: TextStyle(
        fontSize: tankInfoHeaderFontSize,
        fontWeight: FontWeight.normal,
        color: Colors.grey.shade500,
      ),
    );
  }

  Widget _tankInfoLine(
    TankInfoType type,
    TankInfoMode mode,
    double tankInfoFontSize,
    AsyncSnapshot<TankSnapshot>? snapshot,
  ) {
    final IconData iconType =
        type == TankInfoType.pH ? Icons.water_drop : Icons.thermostat;
    final Color iconColor =
        type == TankInfoType.pH ? Colors.green : Colors.blue;

    String textData = 'mock';
    if (mode == TankInfoMode.current) {
      if (type == TankInfoType.pH) {
        textData = 'pH ${snapshot?.data!.pH ?? 'mock'}';
      } else {
        textData = '${snapshot?.data!.temperature ?? 'mock'}°C';
      }
    } else if (mode == TankInfoMode.range) {
      if (type == TankInfoType.pH) {
        final min = snapshot?.data!.minPH ?? 'm.k';
        final max = snapshot?.data!.maxPH ?? 'm.k';
        textData = 'pH $min - $max';
      } else {
        final min = snapshot?.data!.minTemp ?? 'mk';
        final max = snapshot?.data!.maxTemp ?? 'mk';
        textData = '$min - $max°C';
      }
    }

    return Row(
      children: [
        Skeleton.keep(
          child: Icon(
            iconType,
            color: iconColor,
            size: tankInfoFontSize,
          ),
        ),
        SizedBox(width: 5),
        Text(
          textData,
          style: TextStyle(
            fontSize: tankInfoFontSize,
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
