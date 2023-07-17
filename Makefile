LIBRARIES=/Users/jfoster/Documents/Arduino/libraries
ARDUINO_CI=$(LIBRARIES)/TankController/vendor/bundle/ruby/3.2.0/bundler/gems/arduino_ci-bc9c7e5b3bc3/cpp
SRC=$(LIBRARIES)/TankController/src
TEST=$(LIBRARIES)/TankController/test
BIN=$(LIBRARIES)/TankController/.arduino_ci

FLAGS=-std=c++0x \
  -Wno-deprecated-declarations \
  -DARDUINO=100 \
  -g \
  -O1 \
  -fno-omit-frame-pointer \
  -fno-optimize-sibling-calls \
  -fsanitize=address \
  -Wno-unknown-attributes \
  -Wno-address-of-packed-member \
  -D__AVR__ \
  -D__AVR_ATmega2560__ \
  -DARDUINO_ARCH_AVR \
  -DARDUINO_AVR_MEGA2560

INCLUDE=-I$(ARDUINO_CI)/arduino \
  -I$(ARDUINO_CI)/unittest \
  -I$(LIBRARIES)/TankController/src \
  -I$(LIBRARIES)/TankController/src/Devices \
  -I$(LIBRARIES)/TankController/src/UIState \
  -I$(LIBRARIES)/Adafruit_BusIO/src \
  -I$(LIBRARIES)/Adafruit_MAX31865_library/src \
  -I$(LIBRARIES)/PID/src \
  -I$(LIBRARIES)/Ethernet/src \
  -I$(LIBRARIES)/Ethernet/src/utility \
  -I$(LIBRARIES)/LiquidCrystal/src \
  -I$(LIBRARIES)/RTClib/src \
  -I$(LIBRARIES)/Keypad/src \
  -I$(LIBRARIES)/SdFat/src \
  -I$(LIBRARIES)/SdFat/src/ArduinoCI \
  -I$(LIBRARIES)/SdFat/src/DigitalIO \
  -I$(LIBRARIES)/SdFat/src/DigitalIO/boards \
  -I$(LIBRARIES)/SdFat/src/ExFatLib \
  -I$(LIBRARIES)/SdFat/src/FatLib \
  -I$(LIBRARIES)/SdFat/src/FsLib \
  -I$(LIBRARIES)/SdFat/src/SdCard \
  -I$(LIBRARIES)/SdFat/src/SpiDriver \
  -I$(LIBRARIES)/SdFat/src/common \
  -I$(LIBRARIES)/SdFat/src/iostream

.PHONY : all
all : $(BIN)/BlinkTest.cpp.bin 


LIBS_SO=$(BIN)/libBusIO.so $(BIN)/libMAX31865.so $(BIN)/libPID.so $(BIN)/libEthernet.so \
	$(BIN)/libLiquidCrystal.so $(BIN)/libRTClib.so $(BIN)/libKeypad.so $(BIN)/libSdFat.so \
	$(BIN)/libarduino.so
LIBS=-lBusIO -lMAX31865 -lPID -lEthernet -lLiquidCrystal -lRTClib -lKeypad -lSdFat -larduino
GPP_TEST=g++ $(FLAGS) -L$(LIBRARIES)/TankController/.arduino_ci $(INCLUDE)

$(BIN)/BlinkTest.cpp.bin: $(LIBS_SO) $(TEST)/BlinkTest.cpp
	$(GPP_TEST) -o $(BIN)/BlinkTest.cpp.bin $(TEST)/BlinkTest.cpp $(LIBS)

OBJECTS=$(BIN)/Arduino.o $(BIN)/Godmode.o $(BIN)/stdlib.o $(BIN)/ArduinoUnitTests.o \
  $(BIN)/TC_util.o $(BIN)/TankController.o $(BIN)/DataLogger_TC.o $(BIN)/DateTime_TC.o \
  $(BIN)/EEPROM_TC.o $(BIN)/EthernetServer_TC.o $(BIN)/Ethernet_TC.o $(BIN)/JSONBuilder.o \
  $(BIN)/Keypad_TC.o $(BIN)/LiquidCrystal_TC.o $(BIN)/PHControl.o $(BIN)/PHProbe.o \
  $(BIN)/PID_TC.o $(BIN)/PushingBox.o $(BIN)/SD_TC.o $(BIN)/Serial_TC.o $(BIN)/TempProbe_TC.o \
  $(BIN)/TemperatureControl.o $(BIN)/CalibrationManagement.o $(BIN)/EnablePID.o $(BIN)/MainMenu.o \
  $(BIN)/NumberCollectorState.o $(BIN)/PHCalibrationHigh.o $(BIN)/PHCalibrationLow.o \
  $(BIN)/PHCalibrationMid.o $(BIN)/SeeDeviceAddress.o $(BIN)/SeeDeviceUptime.o $(BIN)/SeeFreeMemory.o \
  $(BIN)/SeeGoogleMins.o $(BIN)/SeeLogFile.o $(BIN)/SeePHSlope.o $(BIN)/SeePIDConstants.o \
  $(BIN)/SeePh.o $(BIN)/SeeTankID.o $(BIN)/SeeTempCalOffset.o $(BIN)/SeeVersion.o \
  $(BIN)/SetChillOrHeat.o $(BIN)/SetGoogleSheetInterval.o $(BIN)/SetKD.o $(BIN)/SetKI.o \
  $(BIN)/SetKP.o $(BIN)/SetPHCalibClear.o $(BIN)/SetPHSetPoint.o $(BIN)/SetPHWithSine.o \
  $(BIN)/SetTankID.o $(BIN)/SetTempCalibClear.o $(BIN)/SetTempSetPoint.o $(BIN)/SetTempWithSine.o \
  $(BIN)/SetTime.o $(BIN)/TemperatureCalibration.o $(BIN)/UIState.o $(BIN)/Wait.o
$(BIN)/libarduino.so: $(OBJECTS)
	g++ $(FLAGS) -shared -fPIC -Wl,-undefined,dynamic_lookup -L$(LIBRARIES)/TankController/.arduino_ci \
	$(INCLUDE) -o $(BIN)/libarduino.so $(OBJECTS)

$(BIN)/Arduino.o: $(ARDUINO_CI)/arduino/Arduino.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Arduino.o $(ARDUINO_CI)/arduino/Arduino.cpp

$(BIN)/Godmode.o: $(ARDUINO_CI)/arduino/Godmode.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Godmode.o $(ARDUINO_CI)/arduino/Godmode.cpp

$(BIN)/stdlib.o: $(ARDUINO_CI)/arduino/stdlib.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/stdlib.o $(ARDUINO_CI)/arduino/stdlib.cpp

$(BIN)/ArduinoUnitTests.o: $(ARDUINO_CI)/unittest/ArduinoUnitTests.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/ArduinoUnitTests.o $(ARDUINO_CI)/unittest/ArduinoUnitTests.cpp

$(BIN)/TC_util.o: $(SRC)/TC_util.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/TC_util.o $(SRC)/TC_util.cpp

$(BIN)/TankController.o: $(SRC)/TankController.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/TankController.o $(SRC)/TankController.cpp

$(BIN)/DataLogger_TC.o: $(SRC)/Devices/DataLogger_TC.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/DataLogger_TC.o $(SRC)/Devices/DataLogger_TC.cpp

$(BIN)/DateTime_TC.o: $(SRC)/Devices/DateTime_TC.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/DateTime_TC.o $(SRC)/Devices/DateTime_TC.cpp

$(BIN)/EEPROM_TC.o: $(SRC)/Devices/EEPROM_TC.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/EEPROM_TC.o $(SRC)/Devices/EEPROM_TC.cpp

$(BIN)/EthernetServer_TC.o: $(SRC)/Devices/EthernetServer_TC.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/EthernetServer_TC.o $(SRC)/Devices/EthernetServer_TC.cpp

$(BIN)/Ethernet_TC.o: $(SRC)/Devices/Ethernet_TC.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Ethernet_TC.o $(SRC)/Devices/Ethernet_TC.cpp

$(BIN)/JSONBuilder.o: $(SRC)/Devices/JSONBuilder.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/JSONBuilder.o $(SRC)/Devices/JSONBuilder.cpp

$(BIN)/Keypad_TC.o: $(SRC)/Devices/Keypad_TC.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Keypad_TC.o $(SRC)/Devices/Keypad_TC.cpp

$(BIN)/LiquidCrystal_TC.o: $(SRC)/Devices/LiquidCrystal_TC.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/LiquidCrystal_TC.o $(SRC)/Devices/LiquidCrystal_TC.cpp

$(BIN)/PHControl.o: $(SRC)/Devices/PHControl.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PHControl.o $(SRC)/Devices/PHControl.cpp

$(BIN)/PHProbe.o: $(SRC)/Devices/PHProbe.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PHProbe.o $(SRC)/Devices/PHProbe.cpp

$(BIN)/PID_TC.o: $(SRC)/Devices/PID_TC.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PID_TC.o $(SRC)/Devices/PID_TC.cpp

$(BIN)/PushingBox.o: $(SRC)/Devices/PushingBox.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PushingBox.o $(SRC)/Devices/PushingBox.cpp

$(BIN)/SD_TC.o: $(SRC)/Devices/SD_TC.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SD_TC.o $(SRC)/Devices/SD_TC.cpp

$(BIN)/Serial_TC.o: $(SRC)/Devices/Serial_TC.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Serial_TC.o $(SRC)/Devices/Serial_TC.cpp

$(BIN)/TempProbe_TC.o: $(SRC)/Devices/TempProbe_TC.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/TempProbe_TC.o $(SRC)/Devices/TempProbe_TC.cpp

$(BIN)/TemperatureControl.o: $(SRC)/Devices/TemperatureControl.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/TemperatureControl.o $(SRC)/Devices/TemperatureControl.cpp

$(BIN)/CalibrationManagement.o: $(SRC)/UIState/CalibrationManagement.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/CalibrationManagement.o $(SRC)/UIState/CalibrationManagement.cpp

$(BIN)/EnablePID.o: $(SRC)/UIState/EnablePID.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/EnablePID.o $(SRC)/UIState/EnablePID.cpp

$(BIN)/MainMenu.o: $(SRC)/UIState/MainMenu.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/MainMenu.o $(SRC)/UIState/MainMenu.cpp

$(BIN)/NumberCollectorState.o: $(SRC)/UIState/NumberCollectorState.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/NumberCollectorState.o $(SRC)/UIState/NumberCollectorState.cpp

$(BIN)/PHCalibrationHigh.o: $(SRC)/UIState/PHCalibrationHigh.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PHCalibrationHigh.o $(SRC)/UIState/PHCalibrationHigh.cpp

$(BIN)/PHCalibrationLow.o: $(SRC)/UIState/PHCalibrationLow.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PHCalibrationLow.o $(SRC)/UIState/PHCalibrationLow.cpp

$(BIN)/PHCalibrationMid.o: $(SRC)/UIState/PHCalibrationMid.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PHCalibrationMid.o $(SRC)/UIState/PHCalibrationMid.cpp

$(BIN)/SeeDeviceAddress.o: $(SRC)/UIState/SeeDeviceAddress.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeDeviceAddress.o $(SRC)/UIState/SeeDeviceAddress.cpp

$(BIN)/SeeDeviceUptime.o: $(SRC)/UIState/SeeDeviceUptime.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeDeviceUptime.o $(SRC)/UIState/SeeDeviceUptime.cpp

$(BIN)/SeeFreeMemory.o: $(SRC)/UIState/SeeFreeMemory.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeFreeMemory.o $(SRC)/UIState/SeeFreeMemory.cpp

$(BIN)/SeeGoogleMins.o: $(SRC)/UIState/SeeGoogleMins.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeGoogleMins.o $(SRC)/UIState/SeeGoogleMins.cpp

$(BIN)/SeeLogFile.o: $(SRC)/UIState/SeeLogFile.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeLogFile.o $(SRC)/UIState/SeeLogFile.cpp

$(BIN)/SeePHSlope.o: $(SRC)/UIState/SeePHSlope.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeePHSlope.o $(SRC)/UIState/SeePHSlope.cpp

$(BIN)/SeePIDConstants.o: $(SRC)/UIState/SeePIDConstants.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeePIDConstants.o $(SRC)/UIState/SeePIDConstants.cpp

$(BIN)/SeePh.o: $(SRC)/UIState/SeePh.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeePh.o $(SRC)/UIState/SeePh.cpp

$(BIN)/SeeTankID.o: $(SRC)/UIState/SeeTankID.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeTankID.o $(SRC)/UIState/SeeTankID.cpp

$(BIN)/SeeTempCalOffset.o: $(SRC)/UIState/SeeTempCalOffset.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeTempCalOffset.o $(SRC)/UIState/SeeTempCalOffset.cpp

$(BIN)/SeeVersion.o: $(SRC)/UIState/SeeVersion.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeVersion.o $(SRC)/UIState/SeeVersion.cpp

$(BIN)/SetChillOrHeat.o: $(SRC)/UIState/SetChillOrHeat.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetChillOrHeat.o $(SRC)/UIState/SetChillOrHeat.cpp

$(BIN)/SetGoogleSheetInterval.o: $(SRC)/UIState/SetGoogleSheetInterval.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetGoogleSheetInterval.o $(SRC)/UIState/SetGoogleSheetInterval.cpp

$(BIN)/SetKD.o: $(SRC)/UIState/SetKD.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetKD.o $(SRC)/UIState/SetKD.cpp

$(BIN)/SetKI.o: $(SRC)/UIState/SetKI.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetKI.o $(SRC)/UIState/SetKI.cpp

$(BIN)/SetKP.o: $(SRC)/UIState/SetKP.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetKP.o $(SRC)/UIState/SetKP.cpp

$(BIN)/SetPHCalibClear.o: $(SRC)/UIState/SetPHCalibClear.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetPHCalibClear.o $(SRC)/UIState/SetPHCalibClear.cpp

$(BIN)/SetPHSetPoint.o: $(SRC)/UIState/SetPHSetPoint.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetPHSetPoint.o $(SRC)/UIState/SetPHSetPoint.cpp

$(BIN)/SetPHWithSine.o: $(SRC)/UIState/SetPHWithSine.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetPHWithSine.o $(SRC)/UIState/SetPHWithSine.cpp

$(BIN)/SetTankID.o: $(SRC)/UIState/SetTankID.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetTankID.o $(SRC)/UIState/SetTankID.cpp

$(BIN)/SetTempCalibClear.o: $(SRC)/UIState/SetTempCalibClear.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetTempCalibClear.o $(SRC)/UIState/SetTempCalibClear.cpp

$(BIN)/SetTempSetPoint.o: $(SRC)/UIState/SetTempSetPoint.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetTempSetPoint.o $(SRC)/UIState/SetTempSetPoint.cpp

$(BIN)/SetTempWithSine.o: $(SRC)/UIState/SetTempWithSine.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetTempWithSine.o $(SRC)/UIState/SetTempWithSine.cpp

$(BIN)/SetTime.o: $(SRC)/UIState/SetTime.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetTime.o $(SRC)/UIState/SetTime.cpp

$(BIN)/TemperatureCalibration.o: $(SRC)/UIState/TemperatureCalibration.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/TemperatureCalibration.o $(SRC)/UIState/TemperatureCalibration.cpp

$(BIN)/UIState.o: $(SRC)/UIState/UIState.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/UIState.o $(SRC)/UIState/UIState.cpp

$(BIN)/Wait.o: $(SRC)/UIState/Wait.cpp
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Wait.o $(SRC)/UIState/Wait.cpp

BUSIO=$(LIBRARIES)/Adafruit_BusIO/src/Adafruit_BusIO_Register.cpp \
  $(LIBRARIES)/Adafruit_BusIO/src/Adafruit_I2CDevice.cpp \
  $(LIBRARIES)/Adafruit_BusIO/src/Adafruit_SPIDevice.cpp
$(BIN)/libBusIO.so: $(BUSIO)
	g++ -$(FLAGS) -shared -fPIC -Wl,-undefined,dynamic_lookup \
	$(INCLUDE) -o $(BIN)/libBusIO.so $(BUSIO)

MAX31865=$(LIBRARIES)/Adafruit_MAX31865_library/src/Adafruit_MAX31865.cpp \
  $(LIBRARIES)/Adafruit_MAX31865_library/src/Adafruit_MAX31865_CI.cpp
$(BIN)/libMAX31865.so: $(MAX31865)
	g++ -$(FLAGS) -shared -fPIC -Wl,-undefined,dynamic_lookup \
	$(INCLUDE) -o $(BIN)/libMAX31865.so $(MAX31865)

$(BIN)/libPID.so: $(LIBRARIES)/PID/src/PID_v1.cpp
	g++ -$(FLAGS) -shared -fPIC -Wl,-undefined,dynamic_lookup \
	$(INCLUDE) -o $(BIN)/libPID.so $(LIBRARIES)/PID/src/PID_v1.cpp

ETHERNET=$(LIBRARIES)/Ethernet/src/Dhcp.cpp \
  $(LIBRARIES)/Ethernet/src/Dns.cpp \
  $(LIBRARIES)/Ethernet/src/Ethernet.cpp \
  $(LIBRARIES)/Ethernet/src/EthernetClient.cpp \
  $(LIBRARIES)/Ethernet/src/EthernetClient_CI.cpp \
  $(LIBRARIES)/Ethernet/src/EthernetServer.cpp \
  $(LIBRARIES)/Ethernet/src/EthernetServer_CI.cpp \
  $(LIBRARIES)/Ethernet/src/EthernetUdp.cpp \
  $(LIBRARIES)/Ethernet/src/Ethernet_CI.cpp \
  $(LIBRARIES)/Ethernet/src/socket.cpp \
  $(LIBRARIES)/Ethernet/src/utility/w5100.cpp
$(BIN)/libEthernet.so: $(ETHERNET)
	g++ -$(FLAGS) -shared -fPIC -Wl,-undefined,dynamic_lookup \
	$(INCLUDE) -o $(BIN)/libEthernet.so $(ETHERNET)

LCD=$(LIBRARIES)/LiquidCrystal/src/LiquidCrystal.cpp \
  $(LIBRARIES)/LiquidCrystal/src/LiquidCrystal_CI.cpp
$(BIN)/libLiquidCrystal.so: $(LCD)
	g++ -$(FLAGS) -shared -fPIC -Wl,-undefined,dynamic_lookup \
	$(INCLUDE) -o $(BIN)/libLiquidCrystal.so $(LCD)

RTC=$(LIBRARIES)/RTClib/src/RTC_DS1307.cpp \
  $(LIBRARIES)/RTClib/src/RTC_DS3231.cpp \
  $(LIBRARIES)/RTClib/src/RTC_Micros.cpp \
  $(LIBRARIES)/RTClib/src/RTC_Millis.cpp \
  $(LIBRARIES)/RTClib/src/RTC_PCF8523.cpp \
  $(LIBRARIES)/RTClib/src/RTC_PCF8563.cpp \
  $(LIBRARIES)/RTClib/src/RTClib.cpp \
  $(LIBRARIES)/RTClib/src/RTClib_CI.cpp
$(BIN)/libRTClib.so: $(RTC)
	g++ -$(FLAGS) -shared -fPIC -Wl,-undefined,dynamic_lookup \
	$(INCLUDE) -o $(BIN)/libRTClib.so $(RTC)

KEYPAD=$(LIBRARIES)/Keypad/src/Key.cpp \
  $(LIBRARIES)/Keypad/src/Keypad.cpp \
  $(LIBRARIES)/Keypad/src/Keypad_CI.cpp
$(BIN)/libKeypad.so: $(KEYPAD)
	g++ -$(FLAGS) -shared -fPIC -Wl,-undefined,dynamic_lookup \
	$(INCLUDE) -o $(BIN)/libKeypad.so $(KEYPAD)

SDFAT=$(LIBRARIES)/SdFat/src/FreeStack.cpp \
  $(LIBRARIES)/SdFat/src/MinimumSerial.cpp \
  $(LIBRARIES)/SdFat/src/ArduinoCI/File_CI.cpp \
  $(LIBRARIES)/SdFat/src/ArduinoCI/SD_CI.cpp \
  $(LIBRARIES)/SdFat/src/ExFatLib/ExFatDbg.cpp \
  $(LIBRARIES)/SdFat/src/ExFatLib/ExFatFile.cpp \
  $(LIBRARIES)/SdFat/src/ExFatLib/ExFatFilePrint.cpp \
  $(LIBRARIES)/SdFat/src/ExFatLib/ExFatFileWrite.cpp \
  $(LIBRARIES)/SdFat/src/ExFatLib/ExFatFormatter.cpp \
  $(LIBRARIES)/SdFat/src/ExFatLib/ExFatName.cpp \
  $(LIBRARIES)/SdFat/src/ExFatLib/ExFatPartition.cpp \
  $(LIBRARIES)/SdFat/src/ExFatLib/ExFatVolume.cpp \
  $(LIBRARIES)/SdFat/src/FatLib/FatDbg.cpp \
  $(LIBRARIES)/SdFat/src/FatLib/FatFile.cpp \
  $(LIBRARIES)/SdFat/src/FatLib/FatFormatter.cpp \
  $(LIBRARIES)/SdFat/src/FatLib/FatName.cpp \
  $(LIBRARIES)/SdFat/src/FatLib/FatPartition.cpp \
  $(LIBRARIES)/SdFat/src/FatLib/FatVolume.cpp \
  $(LIBRARIES)/SdFat/src/FsLib/FsFile.cpp \
  $(LIBRARIES)/SdFat/src/FsLib/FsNew.cpp \
  $(LIBRARIES)/SdFat/src/FsLib/FsVolume.cpp \
  $(LIBRARIES)/SdFat/src/SdCard/SdCardInfo.cpp \
  $(LIBRARIES)/SdFat/src/SdCard/SdSpiCard.cpp \
  $(LIBRARIES)/SdFat/src/SdCard/SdioTeensy.cpp \
  $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiArtemis.cpp \
  $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiChipSelect.cpp \
  $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiDue.cpp \
  $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiESP.cpp \
  $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiParticle.cpp \
  $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiSTM32.cpp \
  $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiSTM32Core.cpp \
  $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiTeensy3.cpp \
  $(LIBRARIES)/SdFat/src/common/FmtNumber.cpp \
  $(LIBRARIES)/SdFat/src/common/FsCache.cpp \
  $(LIBRARIES)/SdFat/src/common/FsDateTime.cpp \
  $(LIBRARIES)/SdFat/src/common/FsName.cpp \
  $(LIBRARIES)/SdFat/src/common/FsStructs.cpp \
  $(LIBRARIES)/SdFat/src/common/FsUtf.cpp \
  $(LIBRARIES)/SdFat/src/common/PrintBasic.cpp \
  $(LIBRARIES)/SdFat/src/common/upcase.cpp \
  $(LIBRARIES)/SdFat/src/iostream/StdioStream.cpp \
  $(LIBRARIES)/SdFat/src/iostream/StreamBaseClass.cpp \
  $(LIBRARIES)/SdFat/src/iostream/istream.cpp \
  $(LIBRARIES)/SdFat/src/iostream/ostream.cpp
$(BIN)/libSdFat.so: $(SDFAT)
	g++ -$(FLAGS) -shared -fPIC -Wl,-undefined,dynamic_lookup \
	$(INCLUDE) -o $(BIN)/libSdFat.so $(SDFAT)

.PHONY: clean
clean:
	rm -rf $(BIN)/*
