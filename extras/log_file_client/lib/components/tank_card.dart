import 'dart:async';
import 'package:flutter/material.dart';
import 'package:log_file_client/utils/http_client.dart';

class TankCard extends StatefulWidget {
  const TankCard({
    required this.log,
    required this.onTap,
    super.key,
    this.httpClient,
  });

  final Log log;
  final void Function() onTap;
  final HttpClient? httpClient;

  @override
  State<TankCard> createState() => _TankCardState();
}

class _TankCardState extends State<TankCard> {
  late final HttpClient httpClient;
  late final Future<TankSnapshot> _tankSnapshot = getTankSnapshot();

  @override
  void initState() {
    super.initState();
    httpClient = widget.httpClient ?? HttpClientProd();
  }

  Future<TankSnapshot> getTankSnapshot() async {
    final snapshot = await httpClient.getTankSnapshot(widget.log);
    return snapshot;
  }

  @override
  Widget build(BuildContext context) {
    return MouseRegion(
      cursor: SystemMouseCursors.click,
      child: GestureDetector(
        onTap: widget.onTap,
        child: LayoutBuilder(
          builder: (BuildContext context, BoxConstraints constraints) {
            // Access the available width and height of the widget
            final double cardWidth = constraints.maxWidth * 0.93;

            // Decide sizes of internal components based on card width
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
                  // Graph Thumbnail
                  Container(
                    width: cardWidth,
                    height: cardWidth * 0.6,
                    margin: EdgeInsets.only(
                      bottom: cardWidth * 0.05,
                    ),
                    decoration: BoxDecoration(
                      borderRadius:
                          BorderRadius.vertical(top: Radius.circular(20)),
                      image: DecorationImage(
                        image: AssetImage(
                          './lib/assets/placeholder.png',
                        ),
                        fit: BoxFit.cover,
                      ),
                    ),
                  ),

                  // Tank Name
                  Text(
                    widget.log.name,
                    style: TextStyle(
                      fontSize: titleFontSize,
                      fontWeight: FontWeight.bold,
                      color: const Color(0xFF0C2D48),
                    ),
                  ),

                  // Tank Info
                  FutureBuilder(
                    future: _tankSnapshot,
                    builder: (context, snapshot) {
                      if (snapshot.connectionState == ConnectionState.waiting) {
                        return Center(
                          child: Padding(
                            padding: EdgeInsets.only(
                                left: cardWidth * 0.2,
                                right: cardWidth * 0.2,
                                top: cardWidth * 0.05),
                            child: LinearProgressIndicator(),
                          ),
                        );
                      } else if (snapshot.hasError) {
                        return Center(child: Text('Error: ${snapshot.error}'));
                      } else {
                        final tankSnapshot = snapshot.data!;
                        return Padding(
                          padding: EdgeInsets.only(top: cardWidth * 0.06),
                          child: Row(
                            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                            children: [
                              Row(
                                children: [
                                  Icon(
                                    Icons.thermostat,
                                    size: descriptionFontSize,
                                  ),
                                  Text(
                                    '${tankSnapshot.temperature}°C',
                                    style: TextStyle(
                                      fontSize: descriptionFontSize,
                                      color: const Color(0xFF6D6D6D),
                                    ),
                                  ),
                                ],
                              ),
                              Row(
                                children: [
                                  Icon(
                                    Icons.water_drop,
                                    size: descriptionFontSize,
                                  ),
                                  Text(
                                    'pH ${tankSnapshot.pH}',
                                    style: TextStyle(
                                      fontSize: descriptionFontSize,
                                      color: const Color(0xFF6D6D6D),
                                    ),
                                  ),
                                ],
                              ),
                            ],
                          ),
                        );
                      }
                    },
                  ),
                ],
              ),
            );
          },
        ),
      ),
    );
  }
}
