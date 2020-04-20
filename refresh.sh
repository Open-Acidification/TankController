git pull
arduino-cli compile -b arduino:avr:mega /home/matma/Open-Acidification
arduino-cli upload /home/matma/Open-Acidification -b arduino:avr:mega -p /dev/ttyACM0
