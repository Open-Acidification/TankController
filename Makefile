LIBRARIES := $(shell bundle exec arduino_library_location.rb)
SRC=$(LIBRARIES)/TankController/src
TEST=$(LIBRARIES)/TankController/test
BIN=$(LIBRARIES)/TankController/.arduino_ci

FLAGS=-std=c++0x \
	-Wno-deprecated-declarations \
	-DARDUINO=100 \
	-fPIC \
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

HEADERS=$(wildcard src/*.h) $(wildcard src/Devices/*) $(wildcard src/UIState/*)

.PHONY : all
all : $(BIN)/BadPHCalibrationTest.cpp.bin $(BIN)/BlinkTest.cpp.bin $(BIN)/DateTimeTest.cpp.bin $(BIN)/EEPROMTest.cpp.bin \
  $(BIN)/EnablePIDTest.cpp.bin $(BIN)/EthernetServerTest.cpp.bin $(BIN)/EthernetTest.cpp.bin \
  $(BIN)/GetTimeTest.cpp.bin $(BIN)/JSONBuilderTest.cpp.bin $(BIN)/KeypadTest.cpp.bin \
	$(BIN)/LiquidCrystalTest.cpp.bin \
  $(BIN)/MenuTest.cpp.bin $(BIN)/NumberCollectorStateTest.cpp.bin $(BIN)/PHCalibrationHighTest.cpp.bin \
  $(BIN)/PHCalibrationLowTest.cpp.bin $(BIN)/PHCalibrationMidTest.cpp.bin \
	$(BIN)/PHCalibrationPromptTest.cpp.bin $(BIN)/PHControlTest.cpp.bin \
  $(BIN)/PHProbeTest.cpp.bin $(BIN)/PIDTest.cpp.bin $(BIN)/PushingBoxTest.cpp.bin $(BIN)/SDTest.cpp.bin \
  $(BIN)/SeeDeviceAddressTest.cpp.bin $(BIN)/SeeDeviceUptimeTest.cpp.bin $(BIN)/SeeFreeMemoryTest.cpp.bin \
  $(BIN)/SeeGoogleMinsTest.cpp.bin $(BIN)/SeeLogFileTest.cpp.bin $(BIN)/SeePHCalibrationTest.cpp.bin \
  $(BIN)/SeePIDConstantsTest.cpp.bin $(BIN)/SeePhTest.cpp.bin $(BIN)/SeeTankIDTest.cpp.bin \
  $(BIN)/SeeTempCalOffsetTest.cpp.bin $(BIN)/SeeVersionTest.cpp.bin $(BIN)/SerialTest.cpp.bin \
  $(BIN)/SetChillOrHeatTest.cpp.bin $(BIN)/SetGoogleSheetIntervalTest.cpp.bin $(BIN)/SetKDTest.cpp.bin \
  $(BIN)/SetKITest.cpp.bin $(BIN)/SetKPTest.cpp.bin $(BIN)/SetPHCalibClearTest.cpp.bin \
  $(BIN)/SetPHSetPointTest.cpp.bin $(BIN)/SetPHWithSineTest.cpp.bin $(BIN)/SetTankIDTest.cpp.bin \
  $(BIN)/SetTempCalibClearTest.cpp.bin $(BIN)/SetTempSetPointTest.cpp.bin $(BIN)/SetTempWithSineTest.cpp.bin \
  $(BIN)/SetTimeTest.cpp.bin $(BIN)/StringsTest.cpp.bin $(BIN)/TCLibTest.cpp.bin \
  $(BIN)/ThermalProbe_TCTest.cpp.bin $(BIN)/TemperatureCalibrationTest.cpp.bin \
  $(BIN)/ThermalControlTest.cpp.bin $(BIN)

GPP_TEST=g++ $(FLAGS) -L$(BIN) $(INCLUDE)

$(BIN)/BadPHCalibrationTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/BadPHCalibrationTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/BadPHCalibrationTest.cpp.bin $(TEST)/BadPHCalibrationTest.cpp -larduino

$(BIN)/BlinkTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/BlinkTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/BlinkTest.cpp.bin $(TEST)/BlinkTest.cpp -larduino

$(BIN)/DateTimeTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/DateTimeTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/DateTimeTest.cpp.bin $(TEST)/DateTimeTest.cpp -larduino

$(BIN)/EEPROMTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/EEPROMTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/EEPROMTest.cpp.bin $(TEST)/EEPROMTest.cpp -larduino

$(BIN)/EnablePIDTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/EnablePIDTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/EnablePIDTest.cpp.bin $(TEST)/EnablePIDTest.cpp -larduino

$(BIN)/EthernetServerTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/EthernetServerTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/EthernetServerTest.cpp.bin $(TEST)/EthernetServerTest.cpp -larduino

$(BIN)/EthernetTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/EthernetTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/EthernetTest.cpp.bin $(TEST)/EthernetTest.cpp -larduino

$(BIN)/GetTimeTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/GetTimeTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/GetTimeTest.cpp.bin $(TEST)/GetTimeTest.cpp -larduino

$(BIN)/JSONBuilderTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/JSONBuilderTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/JSONBuilderTest.cpp.bin $(TEST)/JSONBuilderTest.cpp -larduino

$(BIN)/KeypadTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/KeypadTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/KeypadTest.cpp.bin $(TEST)/KeypadTest.cpp -larduino

$(BIN)/LiquidCrystalTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/LiquidCrystalTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/LiquidCrystalTest.cpp.bin $(TEST)/LiquidCrystalTest.cpp -larduino

$(BIN)/MenuTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/MenuTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/MenuTest.cpp.bin $(TEST)/MenuTest.cpp -larduino

$(BIN)/NumberCollectorStateTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/NumberCollectorStateTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/NumberCollectorStateTest.cpp.bin $(TEST)/NumberCollectorStateTest.cpp -larduino

$(BIN)/PHCalibrationHighTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/PHCalibrationHighTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/PHCalibrationHighTest.cpp.bin $(TEST)/PHCalibrationHighTest.cpp -larduino

$(BIN)/PHCalibrationLowTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/PHCalibrationLowTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/PHCalibrationLowTest.cpp.bin $(TEST)/PHCalibrationLowTest.cpp -larduino

$(BIN)/PHCalibrationMidTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/PHCalibrationMidTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/PHCalibrationMidTest.cpp.bin $(TEST)/PHCalibrationMidTest.cpp -larduino

$(BIN)/PHCalibrationPromptTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/PHCalibrationPromptTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/PHCalibrationPromptTest.cpp.bin $(TEST)/PHCalibrationPromptTest.cpp -larduino

$(BIN)/PHControlTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/PHControlTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/PHControlTest.cpp.bin $(TEST)/PHControlTest.cpp -larduino

$(BIN)/PHProbeTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/PHProbeTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/PHProbeTest.cpp.bin $(TEST)/PHProbeTest.cpp -larduino

$(BIN)/PIDTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/PIDTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/PIDTest.cpp.bin $(TEST)/PIDTest.cpp -larduino

$(BIN)/PushingBoxTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/PushingBoxTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/PushingBoxTest.cpp.bin $(TEST)/PushingBoxTest.cpp -larduino

$(BIN)/SDTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SDTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SDTest.cpp.bin $(TEST)/SDTest.cpp -larduino

$(BIN)/SeeDeviceAddressTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SeeDeviceAddressTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SeeDeviceAddressTest.cpp.bin $(TEST)/SeeDeviceAddressTest.cpp -larduino

$(BIN)/SeeDeviceUptimeTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SeeDeviceUptimeTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SeeDeviceUptimeTest.cpp.bin $(TEST)/SeeDeviceUptimeTest.cpp -larduino

$(BIN)/SeeFreeMemoryTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SeeFreeMemoryTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SeeFreeMemoryTest.cpp.bin $(TEST)/SeeFreeMemoryTest.cpp -larduino

$(BIN)/SeeGoogleMinsTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SeeGoogleMinsTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SeeGoogleMinsTest.cpp.bin $(TEST)/SeeGoogleMinsTest.cpp -larduino

$(BIN)/SeeLogFileTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SeeLogFileTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SeeLogFileTest.cpp.bin $(TEST)/SeeLogFileTest.cpp -larduino

$(BIN)/SeePHCalibrationTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SeePHCalibrationTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SeePHCalibrationTest.cpp.bin $(TEST)/SeePHCalibrationTest.cpp -larduino

$(BIN)/SeePIDConstantsTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SeePIDConstantsTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SeePIDConstantsTest.cpp.bin $(TEST)/SeePIDConstantsTest.cpp -larduino

$(BIN)/SeePhTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SeePhTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SeePhTest.cpp.bin $(TEST)/SeePhTest.cpp -larduino

$(BIN)/SeeTankIDTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SeeTankIDTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SeeTankIDTest.cpp.bin $(TEST)/SeeTankIDTest.cpp -larduino

$(BIN)/SeeTempCalOffsetTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SeeTempCalOffsetTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SeeTempCalOffsetTest.cpp.bin $(TEST)/SeeTempCalOffsetTest.cpp -larduino

$(BIN)/SeeVersionTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SeeVersionTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SeeVersionTest.cpp.bin $(TEST)/SeeVersionTest.cpp -larduino

$(BIN)/SerialTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SerialTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SerialTest.cpp.bin $(TEST)/SerialTest.cpp -larduino

$(BIN)/SetChillOrHeatTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SetChillOrHeatTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SetChillOrHeatTest.cpp.bin $(TEST)/SetChillOrHeatTest.cpp -larduino

$(BIN)/SetGoogleSheetIntervalTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SetGoogleSheetIntervalTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SetGoogleSheetIntervalTest.cpp.bin $(TEST)/SetGoogleSheetIntervalTest.cpp -larduino

$(BIN)/SetKDTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SetKDTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SetKDTest.cpp.bin $(TEST)/SetKDTest.cpp -larduino

$(BIN)/SetKITest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SetKITest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SetKITest.cpp.bin $(TEST)/SetKITest.cpp -larduino

$(BIN)/SetKPTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SetKPTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SetKPTest.cpp.bin $(TEST)/SetKPTest.cpp -larduino

$(BIN)/SetPHCalibClearTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SetPHCalibClearTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SetPHCalibClearTest.cpp.bin $(TEST)/SetPHCalibClearTest.cpp -larduino

$(BIN)/SetPHSetPointTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SetPHSetPointTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SetPHSetPointTest.cpp.bin $(TEST)/SetPHSetPointTest.cpp -larduino

$(BIN)/SetPHWithSineTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SetPHWithSineTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SetPHWithSineTest.cpp.bin $(TEST)/SetPHWithSineTest.cpp -larduino

$(BIN)/SetTankIDTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SetTankIDTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SetTankIDTest.cpp.bin $(TEST)/SetTankIDTest.cpp -larduino

$(BIN)/SetTempCalibClearTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SetTempCalibClearTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SetTempCalibClearTest.cpp.bin $(TEST)/SetTempCalibClearTest.cpp -larduino

$(BIN)/SetTempSetPointTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SetTempSetPointTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SetTempSetPointTest.cpp.bin $(TEST)/SetTempSetPointTest.cpp -larduino

$(BIN)/SetTempWithSineTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SetTempWithSineTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SetTempWithSineTest.cpp.bin $(TEST)/SetTempWithSineTest.cpp -larduino

$(BIN)/SetTimeTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/SetTimeTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/SetTimeTest.cpp.bin $(TEST)/SetTimeTest.cpp -larduino

$(BIN)/StringsTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/StringsTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/StringsTest.cpp.bin $(TEST)/StringsTest.cpp -larduino

$(BIN)/TCLibTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/TCLibTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/TCLibTest.cpp.bin $(TEST)/TCLibTest.cpp -larduino

$(BIN)/ThermalProbe_TCTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/ThermalProbe_TCTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/ThermalProbe_TCTest.cpp.bin $(TEST)/ThermalProbe_TCTest.cpp -larduino

$(BIN)/TemperatureCalibrationTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/TemperatureCalibrationTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/TemperatureCalibrationTest.cpp.bin $(TEST)/TemperatureCalibrationTest.cpp -larduino

$(BIN)/ThermalControlTest.cpp.bin: $(BIN)/libarduino.so $(TEST)/ThermalControlTest.cpp $(HEADERS)
	$(GPP_TEST) -o $(BIN)/ThermalControlTest.cpp.bin $(TEST)/ThermalControlTest.cpp -larduino

BUSIO=$(BIN)/BusIO.o $(BIN)/I2CDevice.o $(BIN)/SPIDevice.o
ETHERNET=$(BIN)/Dhcp.o $(BIN)/Dns.o $(BIN)/Ethernet.o $(BIN)/EthernetClient.o $(BIN)/EthernetClient_CI.o \
$(BIN)/EthernetServer.o $(BIN)/EthernetServer_CI.o $(BIN)/EthernetUdp.o $(BIN)/Ethernet_CI.o \
$(BIN)/socket.o $(BIN)/w5100.o
LCD=$(BIN)/LiquidCrystal.o $(BIN)/LiquidCrystal_CI.o
MAX31865=$(BIN)/MAX31865.o $(BIN)/MAX31865_CI.o
RTC=$(BIN)/RTC_DS1307.o $(BIN)/RTC_DS3231.o $(BIN)/RTC_Micros.o $(BIN)/RTC_Millis.o \
  $(BIN)/RTC_PCF8523.o $(BIN)/RTC_PCF8563.o $(BIN)/RTClib.o $(BIN)/RTClib_CI.o
KEYPAD=$(BIN)/Key.o $(BIN)/Keypad.o $(BIN)/Keypad_CI.o
SDFAT=$(BIN)/FreeStack.o $(BIN)/MinimumSerial.o $(BIN)/File_CI.o $(BIN)/SD_CI.o $(BIN)/ExFatDbg.o \
  $(BIN)/ExFatFile.o $(BIN)/ExFatFilePrint.o $(BIN)/ExFatFileWrite.o $(BIN)/ExFatFormatter.o \
  $(BIN)/ExFatName.o $(BIN)/ExFatPartition.o $(BIN)/ExFatVolume.o $(BIN)/FatDbg.o $(BIN)/FatFile.o \
	$(BIN)/FatFileLFN.o  $(BIN)/FatFilePrint.o  $(BIN)/FatFileSFN.o \
  $(BIN)/FatFormatter.o $(BIN)/FatName.o $(BIN)/FatPartition.o $(BIN)/FatVolume.o $(BIN)/FsFile.o \
  $(BIN)/FsNew.o $(BIN)/FsVolume.o $(BIN)/SdCardInfo.o $(BIN)/SdSpiCard.o $(BIN)/SdioTeensy.o \
  $(BIN)/SdSpiArtemis.o $(BIN)/SdSpiChipSelect.o $(BIN)/SdSpiDue.o $(BIN)/SdSpiESP.o \
  $(BIN)/SdSpiParticle.o $(BIN)/SdSpiSTM32.o $(BIN)/SdSpiSTM32Core.o $(BIN)/SdSpiTeensy3.o \
  $(BIN)/FmtNumber.o $(BIN)/FsCache.o $(BIN)/FsDateTime.o $(BIN)/FsName.o $(BIN)/FsStructs.o \
  $(BIN)/FsUtf.o $(BIN)/PrintBasic.o $(BIN)/upcase.o $(BIN)/StdioStream.o $(BIN)/StreamBaseClass.o \
  $(BIN)/istream.o $(BIN)/ostream.o

OBJECTS=$(BIN)/Arduino.o $(BIN)/Godmode.o $(BIN)/stdlib.o $(BIN)/ArduinoUnitTests.o \
  $(BIN)/TC_util.o $(BIN)/TankController.o $(BIN)/DataLogger_TC.o $(BIN)/DateTime_TC.o \
  $(BIN)/EEPROM_TC.o $(BIN)/EthernetServer_TC.o $(BIN)/Ethernet_TC.o $(BIN)/GetTime.o \
	$(BIN)/JSONBuilder.o \
  $(BIN)/Keypad_TC.o $(BIN)/LiquidCrystal_TC.o $(BIN)/PHControl.o $(BIN)/PHProbe.o \
  $(BIN)/PID_TC.o $(BIN)/PushingBox.o $(BIN)/SD_TC.o $(BIN)/Serial_TC.o $(BIN)/ThermalProbe_TC.o \
  $(BIN)/ThermalControl.o $(BIN)/BadPHCalibration.o $(BIN)/CalibrationManagement.o $(BIN)/EnablePID.o $(BIN)/MainMenu.o \
  $(BIN)/NumberCollectorState.o $(BIN)/PHCalibrationHigh.o $(BIN)/PHCalibrationLow.o \
  $(BIN)/PHCalibrationMid.o $(BIN)/PHCalibrationPrompt.o $(BIN)/SeeDeviceAddress.o \
	$(BIN)/SeeDeviceUptime.o $(BIN)/SeeFreeMemory.o \
  $(BIN)/SeeGoogleMins.o $(BIN)/SeeLogFile.o $(BIN)/SeePHCalibration.o \
	$(BIN)/SeePIDConstants.o \
  $(BIN)/SeePh.o $(BIN)/SeeTankID.o $(BIN)/SeeTempCalOffset.o $(BIN)/SeeVersion.o \
  $(BIN)/SetChillOrHeat.o $(BIN)/SetGoogleSheetInterval.o $(BIN)/SetKD.o $(BIN)/SetKI.o \
  $(BIN)/SetKP.o $(BIN)/SetPHCalibClear.o $(BIN)/SetPHSetPoint.o $(BIN)/SetPHWithSine.o \
  $(BIN)/SetTankID.o $(BIN)/SetTempCalibClear.o $(BIN)/SetTempSetPoint.o $(BIN)/SetTempWithSine.o \
  $(BIN)/SetTime.o $(BIN)/TemperatureCalibration.o $(BIN)/UIState.o $(BIN)/Wait.o \
	$(BUSIO) $(LCD) $(MAX31865) $(BIN)/PID.o $(ETHERNET) $(RTC) $(KEYPAD) $(SDFAT)

$(BIN)/libarduino.so: $(OBJECTS)
	g++ $(FLAGS) -shared -Wl,-undefined,dynamic_lookup -L$(BIN) $(INCLUDE) \
	-o $(BIN)/libarduino.so $(OBJECTS)

$(BIN)/Arduino.o: $(ARDUINO_CI)/arduino/Arduino.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Arduino.o $(ARDUINO_CI)/arduino/Arduino.cpp

$(BIN)/Godmode.o: $(ARDUINO_CI)/arduino/Godmode.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Godmode.o $(ARDUINO_CI)/arduino/Godmode.cpp

$(BIN)/stdlib.o: $(ARDUINO_CI)/arduino/stdlib.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/stdlib.o $(ARDUINO_CI)/arduino/stdlib.cpp

$(BIN)/ArduinoUnitTests.o: $(ARDUINO_CI)/unittest/ArduinoUnitTests.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/ArduinoUnitTests.o $(ARDUINO_CI)/unittest/ArduinoUnitTests.cpp

$(BIN)/TC_util.o: $(SRC)/TC_util.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/TC_util.o $(SRC)/TC_util.cpp

$(BIN)/TankController.o: $(SRC)/TankController.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/TankController.o $(SRC)/TankController.cpp

$(BIN)/DataLogger_TC.o: $(SRC)/Devices/DataLogger_TC.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/DataLogger_TC.o $(SRC)/Devices/DataLogger_TC.cpp

$(BIN)/DateTime_TC.o: $(SRC)/Devices/DateTime_TC.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/DateTime_TC.o $(SRC)/Devices/DateTime_TC.cpp

$(BIN)/EEPROM_TC.o: $(SRC)/Devices/EEPROM_TC.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/EEPROM_TC.o $(SRC)/Devices/EEPROM_TC.cpp

$(BIN)/EthernetServer_TC.o: $(SRC)/Devices/EthernetServer_TC.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/EthernetServer_TC.o $(SRC)/Devices/EthernetServer_TC.cpp

$(BIN)/Ethernet_TC.o: $(SRC)/Devices/Ethernet_TC.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Ethernet_TC.o $(SRC)/Devices/Ethernet_TC.cpp

$(BIN)/JSONBuilder.o: $(SRC)/Devices/JSONBuilder.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/JSONBuilder.o $(SRC)/Devices/JSONBuilder.cpp

$(BIN)/GetTime.o: $(SRC)/Devices/GetTime.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/GetTime.o $(SRC)/Devices/GetTime.cpp

$(BIN)/Keypad_TC.o: $(SRC)/Devices/Keypad_TC.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Keypad_TC.o $(SRC)/Devices/Keypad_TC.cpp

$(BIN)/LiquidCrystal_TC.o: $(SRC)/Devices/LiquidCrystal_TC.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/LiquidCrystal_TC.o $(SRC)/Devices/LiquidCrystal_TC.cpp

$(BIN)/PHControl.o: $(SRC)/Devices/PHControl.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PHControl.o $(SRC)/Devices/PHControl.cpp

$(BIN)/PHProbe.o: $(SRC)/Devices/PHProbe.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PHProbe.o $(SRC)/Devices/PHProbe.cpp

$(BIN)/PID_TC.o: $(SRC)/Devices/PID_TC.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PID_TC.o $(SRC)/Devices/PID_TC.cpp

$(BIN)/PushingBox.o: $(SRC)/Devices/PushingBox.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PushingBox.o $(SRC)/Devices/PushingBox.cpp

$(BIN)/SD_TC.o: $(SRC)/Devices/SD_TC.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SD_TC.o $(SRC)/Devices/SD_TC.cpp

$(BIN)/Serial_TC.o: $(SRC)/Devices/Serial_TC.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Serial_TC.o $(SRC)/Devices/Serial_TC.cpp

$(BIN)/ThermalProbe_TC.o: $(SRC)/Devices/ThermalProbe_TC.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/ThermalProbe_TC.o $(SRC)/Devices/ThermalProbe_TC.cpp

$(BIN)/ThermalControl.o: $(SRC)/Devices/ThermalControl.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/ThermalControl.o $(SRC)/Devices/ThermalControl.cpp

$(BIN)/BadPHCalibration.o: $(SRC)/UIState/BadPHCalibration.cpp  $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/BadPHCalibration.o $(SRC)/UIState/BadPHCalibration.cpp

$(BIN)/CalibrationManagement.o: $(SRC)/UIState/CalibrationManagement.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/CalibrationManagement.o $(SRC)/UIState/CalibrationManagement.cpp

$(BIN)/EnablePID.o: $(SRC)/UIState/EnablePID.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/EnablePID.o $(SRC)/UIState/EnablePID.cpp

$(BIN)/MainMenu.o: $(SRC)/UIState/MainMenu.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/MainMenu.o $(SRC)/UIState/MainMenu.cpp

$(BIN)/NumberCollectorState.o: $(SRC)/UIState/NumberCollectorState.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/NumberCollectorState.o $(SRC)/UIState/NumberCollectorState.cpp

$(BIN)/PHCalibrationHigh.o: $(SRC)/UIState/PHCalibrationHigh.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PHCalibrationHigh.o $(SRC)/UIState/PHCalibrationHigh.cpp

$(BIN)/PHCalibrationLow.o: $(SRC)/UIState/PHCalibrationLow.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PHCalibrationLow.o $(SRC)/UIState/PHCalibrationLow.cpp

$(BIN)/PHCalibrationMid.o: $(SRC)/UIState/PHCalibrationMid.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PHCalibrationMid.o $(SRC)/UIState/PHCalibrationMid.cpp

$(BIN)/PHCalibrationPrompt.o: $(SRC)/UIState/PHCalibrationPrompt.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PHCalibrationPrompt.o $(SRC)/UIState/PHCalibrationPrompt.cpp

$(BIN)/SeeDeviceAddress.o: $(SRC)/UIState/SeeDeviceAddress.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeDeviceAddress.o $(SRC)/UIState/SeeDeviceAddress.cpp

$(BIN)/SeeDeviceUptime.o: $(SRC)/UIState/SeeDeviceUptime.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeDeviceUptime.o $(SRC)/UIState/SeeDeviceUptime.cpp

$(BIN)/SeeFreeMemory.o: $(SRC)/UIState/SeeFreeMemory.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeFreeMemory.o $(SRC)/UIState/SeeFreeMemory.cpp

$(BIN)/SeeGoogleMins.o: $(SRC)/UIState/SeeGoogleMins.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeGoogleMins.o $(SRC)/UIState/SeeGoogleMins.cpp

$(BIN)/SeeLogFile.o: $(SRC)/UIState/SeeLogFile.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeLogFile.o $(SRC)/UIState/SeeLogFile.cpp

$(BIN)/SeePHCalibration.o: $(SRC)/UIState/SeePHCalibration.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeePHCalibration.o $(SRC)/UIState/SeePHCalibration.cpp

$(BIN)/SeePIDConstants.o: $(SRC)/UIState/SeePIDConstants.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeePIDConstants.o $(SRC)/UIState/SeePIDConstants.cpp

$(BIN)/SeePh.o: $(SRC)/UIState/SeePh.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeePh.o $(SRC)/UIState/SeePh.cpp

$(BIN)/SeeTankID.o: $(SRC)/UIState/SeeTankID.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeTankID.o $(SRC)/UIState/SeeTankID.cpp

$(BIN)/SeeTempCalOffset.o: $(SRC)/UIState/SeeTempCalOffset.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeTempCalOffset.o $(SRC)/UIState/SeeTempCalOffset.cpp

$(BIN)/SeeVersion.o: $(SRC)/UIState/SeeVersion.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SeeVersion.o $(SRC)/UIState/SeeVersion.cpp

$(BIN)/SetChillOrHeat.o: $(SRC)/UIState/SetChillOrHeat.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetChillOrHeat.o $(SRC)/UIState/SetChillOrHeat.cpp

$(BIN)/SetGoogleSheetInterval.o: $(SRC)/UIState/SetGoogleSheetInterval.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetGoogleSheetInterval.o $(SRC)/UIState/SetGoogleSheetInterval.cpp

$(BIN)/SetKD.o: $(SRC)/UIState/SetKD.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetKD.o $(SRC)/UIState/SetKD.cpp

$(BIN)/SetKI.o: $(SRC)/UIState/SetKI.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetKI.o $(SRC)/UIState/SetKI.cpp

$(BIN)/SetKP.o: $(SRC)/UIState/SetKP.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetKP.o $(SRC)/UIState/SetKP.cpp

$(BIN)/SetPHCalibClear.o: $(SRC)/UIState/SetPHCalibClear.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetPHCalibClear.o $(SRC)/UIState/SetPHCalibClear.cpp

$(BIN)/SetPHSetPoint.o: $(SRC)/UIState/SetPHSetPoint.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetPHSetPoint.o $(SRC)/UIState/SetPHSetPoint.cpp

$(BIN)/SetPHWithSine.o: $(SRC)/UIState/SetPHWithSine.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetPHWithSine.o $(SRC)/UIState/SetPHWithSine.cpp

$(BIN)/SetTankID.o: $(SRC)/UIState/SetTankID.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetTankID.o $(SRC)/UIState/SetTankID.cpp

$(BIN)/SetTempCalibClear.o: $(SRC)/UIState/SetTempCalibClear.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetTempCalibClear.o $(SRC)/UIState/SetTempCalibClear.cpp

$(BIN)/SetTempSetPoint.o: $(SRC)/UIState/SetTempSetPoint.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetTempSetPoint.o $(SRC)/UIState/SetTempSetPoint.cpp

$(BIN)/SetTempWithSine.o: $(SRC)/UIState/SetTempWithSine.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetTempWithSine.o $(SRC)/UIState/SetTempWithSine.cpp

$(BIN)/SetTime.o: $(SRC)/UIState/SetTime.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SetTime.o $(SRC)/UIState/SetTime.cpp

$(BIN)/TemperatureCalibration.o: $(SRC)/UIState/TemperatureCalibration.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/TemperatureCalibration.o $(SRC)/UIState/TemperatureCalibration.cpp

$(BIN)/UIState.o: $(SRC)/UIState/UIState.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/UIState.o $(SRC)/UIState/UIState.cpp

$(BIN)/Wait.o: $(SRC)/UIState/Wait.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Wait.o $(SRC)/UIState/Wait.cpp

$(BIN)/BusIO.o: $(LIBRARIES)/Adafruit_BusIO/src/Adafruit_BusIO_Register.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/BusIO.o $(LIBRARIES)/Adafruit_BusIO/src/Adafruit_BusIO_Register.cpp

$(BIN)/I2CDevice.o: $(LIBRARIES)/Adafruit_BusIO/src/Adafruit_I2CDevice.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/I2CDevice.o $(LIBRARIES)/Adafruit_BusIO/src/Adafruit_I2CDevice.cpp

$(BIN)/SPIDevice.o: $(LIBRARIES)/Adafruit_BusIO/src/Adafruit_SPIDevice.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SPIDevice.o $(LIBRARIES)/Adafruit_BusIO/src/Adafruit_SPIDevice.cpp

$(BIN)/MAX31865.o: $(LIBRARIES)/Adafruit_MAX31865_library/src/Adafruit_MAX31865.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/MAX31865.o $(LIBRARIES)/Adafruit_MAX31865_library/src/Adafruit_MAX31865.cpp

$(BIN)/MAX31865_CI.o: $(LIBRARIES)/Adafruit_MAX31865_library/src/Adafruit_MAX31865_CI.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/MAX31865_CI.o $(LIBRARIES)/Adafruit_MAX31865_library/src/Adafruit_MAX31865_CI.cpp

$(BIN)/PID.o: $(LIBRARIES)/PID/src/PID_v1.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PID.o $(LIBRARIES)/PID/src/PID_v1.cpp

$(BIN)/Dhcp.o: $(LIBRARIES)/Ethernet/src/Dhcp.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Dhcp.o $(LIBRARIES)/Ethernet/src/Dhcp.cpp

$(BIN)/Dns.o: $(LIBRARIES)/Ethernet/src/Dns.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Dns.o $(LIBRARIES)/Ethernet/src/Dns.cpp

$(BIN)/Ethernet.o: $(LIBRARIES)/Ethernet/src/Ethernet.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Ethernet.o $(LIBRARIES)/Ethernet/src/Ethernet.cpp

$(BIN)/EthernetClient.o: $(LIBRARIES)/Ethernet/src/EthernetClient.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/EthernetClient.o $(LIBRARIES)/Ethernet/src/EthernetClient.cpp

$(BIN)/EthernetClient_CI.o: $(LIBRARIES)/Ethernet/src/EthernetClient_CI.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/EthernetClient_CI.o $(LIBRARIES)/Ethernet/src/EthernetClient_CI.cpp

$(BIN)/EthernetServer.o: $(LIBRARIES)/Ethernet/src/EthernetServer.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/EthernetServer.o $(LIBRARIES)/Ethernet/src/EthernetServer.cpp

$(BIN)/EthernetServer_CI.o: $(LIBRARIES)/Ethernet/src/EthernetServer_CI.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/EthernetServer_CI.o $(LIBRARIES)/Ethernet/src/EthernetServer_CI.cpp

$(BIN)/EthernetUdp.o: $(LIBRARIES)/Ethernet/src/EthernetUdp.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/EthernetUdp.o $(LIBRARIES)/Ethernet/src/EthernetUdp.cpp

$(BIN)/Ethernet_CI.o: $(LIBRARIES)/Ethernet/src/Ethernet_CI.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Ethernet_CI.o $(LIBRARIES)/Ethernet/src/Ethernet_CI.cpp

$(BIN)/socket.o: $(LIBRARIES)/Ethernet/src/socket.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/socket.o $(LIBRARIES)/Ethernet/src/socket.cpp

$(BIN)/w5100.o: $(LIBRARIES)/Ethernet/src/utility/w5100.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/w5100.o $(LIBRARIES)/Ethernet/src/utility/w5100.cpp

$(BIN)/LiquidCrystal.o: $(LIBRARIES)/LiquidCrystal/src/LiquidCrystal.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/LiquidCrystal.o $(LIBRARIES)/LiquidCrystal/src/LiquidCrystal.cpp

$(BIN)/LiquidCrystal_CI.o: $(LIBRARIES)/LiquidCrystal/src/LiquidCrystal_CI.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/LiquidCrystal_CI.o $(LIBRARIES)/LiquidCrystal/src/LiquidCrystal_CI.cpp

$(BIN)/RTC_DS1307.o: $(LIBRARIES)/RTClib/src/RTC_DS1307.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/RTC_DS1307.o $(LIBRARIES)/RTClib/src/RTC_DS1307.cpp

$(BIN)/RTC_DS3231.o: $(LIBRARIES)/RTClib/src/RTC_DS3231.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/RTC_DS3231.o $(LIBRARIES)/RTClib/src/RTC_DS3231.cpp

$(BIN)/RTC_Micros.o: $(LIBRARIES)/RTClib/src/RTC_Micros.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/RTC_Micros.o $(LIBRARIES)/RTClib/src/RTC_Micros.cpp

$(BIN)/RTC_Millis.o: $(LIBRARIES)/RTClib/src/RTC_Millis.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/RTC_Millis.o $(LIBRARIES)/RTClib/src/RTC_Millis.cpp

$(BIN)/RTC_PCF8523.o: $(LIBRARIES)/RTClib/src/RTC_PCF8523.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/RTC_PCF8523.o $(LIBRARIES)/RTClib/src/RTC_PCF8523.cpp

$(BIN)/RTC_PCF8563.o: $(LIBRARIES)/RTClib/src/RTC_PCF8563.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/RTC_PCF8563.o $(LIBRARIES)/RTClib/src/RTC_PCF8563.cpp

$(BIN)/RTClib.o: $(LIBRARIES)/RTClib/src/RTClib.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/RTClib.o $(LIBRARIES)/RTClib/src/RTClib.cpp

$(BIN)/RTClib_CI.o: $(LIBRARIES)/RTClib/src/RTClib_CI.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/RTClib_CI.o $(LIBRARIES)/RTClib/src/RTClib_CI.cpp

$(BIN)/Key.o: $(LIBRARIES)/Keypad/src/Key.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Key.o $(LIBRARIES)/Keypad/src/Key.cpp

$(BIN)/Keypad.o: $(LIBRARIES)/Keypad/src/Keypad.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Keypad.o $(LIBRARIES)/Keypad/src/Keypad.cpp

$(BIN)/Keypad_CI.o: $(LIBRARIES)/Keypad/src/Keypad_CI.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/Keypad_CI.o $(LIBRARIES)/Keypad/src/Keypad_CI.cpp

$(BIN)/FreeStack.o: $(LIBRARIES)/SdFat/src/FreeStack.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FreeStack.o $(LIBRARIES)/SdFat/src/FreeStack.cpp

$(BIN)/MinimumSerial.o: $(LIBRARIES)/SdFat/src/MinimumSerial.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/MinimumSerial.o $(LIBRARIES)/SdFat/src/MinimumSerial.cpp

$(BIN)/File_CI.o: $(LIBRARIES)/SdFat/src/ArduinoCI/File_CI.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/File_CI.o $(LIBRARIES)/SdFat/src/ArduinoCI/File_CI.cpp

$(BIN)/SD_CI.o: $(LIBRARIES)/SdFat/src/ArduinoCI/SD_CI.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SD_CI.o $(LIBRARIES)/SdFat/src/ArduinoCI/SD_CI.cpp

$(BIN)/ExFatDbg.o: $(LIBRARIES)/SdFat/src/ExFatLib/ExFatDbg.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/ExFatDbg.o $(LIBRARIES)/SdFat/src/ExFatLib/ExFatDbg.cpp

$(BIN)/ExFatFile.o: $(LIBRARIES)/SdFat/src/ExFatLib/ExFatFile.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/ExFatFile.o $(LIBRARIES)/SdFat/src/ExFatLib/ExFatFile.cpp

$(BIN)/ExFatFilePrint.o: $(LIBRARIES)/SdFat/src/ExFatLib/ExFatFilePrint.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/ExFatFilePrint.o $(LIBRARIES)/SdFat/src/ExFatLib/ExFatFilePrint.cpp

$(BIN)/ExFatFileWrite.o: $(LIBRARIES)/SdFat/src/ExFatLib/ExFatFileWrite.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/ExFatFileWrite.o $(LIBRARIES)/SdFat/src/ExFatLib/ExFatFileWrite.cpp

$(BIN)/ExFatFormatter.o: $(LIBRARIES)/SdFat/src/ExFatLib/ExFatFormatter.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/ExFatFormatter.o $(LIBRARIES)/SdFat/src/ExFatLib/ExFatFormatter.cpp

$(BIN)/ExFatName.o: $(LIBRARIES)/SdFat/src/ExFatLib/ExFatName.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/ExFatName.o $(LIBRARIES)/SdFat/src/ExFatLib/ExFatName.cpp

$(BIN)/ExFatPartition.o: $(LIBRARIES)/SdFat/src/ExFatLib/ExFatPartition.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/ExFatPartition.o $(LIBRARIES)/SdFat/src/ExFatLib/ExFatPartition.cpp

$(BIN)/ExFatVolume.o: $(LIBRARIES)/SdFat/src/ExFatLib/ExFatVolume.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/ExFatVolume.o $(LIBRARIES)/SdFat/src/ExFatLib/ExFatVolume.cpp

$(BIN)/FatDbg.o: $(LIBRARIES)/SdFat/src/FatLib/FatDbg.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FatDbg.o $(LIBRARIES)/SdFat/src/FatLib/FatDbg.cpp

$(BIN)/FatFile.o: $(LIBRARIES)/SdFat/src/FatLib/FatFile.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FatFile.o $(LIBRARIES)/SdFat/src/FatLib/FatFile.cpp

$(BIN)/FatFileLFN.o: $(LIBRARIES)/SdFat/src/FatLib/FatFileLFN.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FatFileLFN.o $(LIBRARIES)/SdFat/src/FatLib/FatFileLFN.cpp

$(BIN)/FatFilePrint.o: $(LIBRARIES)/SdFat/src/FatLib/FatFilePrint.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FatFilePrint.o $(LIBRARIES)/SdFat/src/FatLib/FatFilePrint.cpp

$(BIN)/FatFileSFN.o: $(LIBRARIES)/SdFat/src/FatLib/FatFileSFN.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FatFileSFN.o $(LIBRARIES)/SdFat/src/FatLib/FatFileSFN.cpp

$(BIN)/FatFormatter.o: $(LIBRARIES)/SdFat/src/FatLib/FatFormatter.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FatFormatter.o $(LIBRARIES)/SdFat/src/FatLib/FatFormatter.cpp

$(BIN)/FatName.o: $(LIBRARIES)/SdFat/src/FatLib/FatName.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FatName.o $(LIBRARIES)/SdFat/src/FatLib/FatName.cpp

$(BIN)/FatPartition.o: $(LIBRARIES)/SdFat/src/FatLib/FatPartition.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FatPartition.o $(LIBRARIES)/SdFat/src/FatLib/FatPartition.cpp

$(BIN)/FatVolume.o: $(LIBRARIES)/SdFat/src/FatLib/FatVolume.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FatVolume.o $(LIBRARIES)/SdFat/src/FatLib/FatVolume.cpp

$(BIN)/FsFile.o: $(LIBRARIES)/SdFat/src/FsLib/FsFile.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FsFile.o $(LIBRARIES)/SdFat/src/FsLib/FsFile.cpp

$(BIN)/FsNew.o: $(LIBRARIES)/SdFat/src/FsLib/FsNew.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FsNew.o $(LIBRARIES)/SdFat/src/FsLib/FsNew.cpp

$(BIN)/FsVolume.o: $(LIBRARIES)/SdFat/src/FsLib/FsVolume.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FsVolume.o $(LIBRARIES)/SdFat/src/FsLib/FsVolume.cpp

$(BIN)/SdCardInfo.o: $(LIBRARIES)/SdFat/src/SdCard/SdCardInfo.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SdCardInfo.o $(LIBRARIES)/SdFat/src/SdCard/SdCardInfo.cpp

$(BIN)/SdSpiCard.o: $(LIBRARIES)/SdFat/src/SdCard/SdSpiCard.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SdSpiCard.o $(LIBRARIES)/SdFat/src/SdCard/SdSpiCard.cpp

$(BIN)/SdioTeensy.o: $(LIBRARIES)/SdFat/src/SdCard/SdioTeensy.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SdioTeensy.o $(LIBRARIES)/SdFat/src/SdCard/SdioTeensy.cpp

$(BIN)/SdSpiArtemis.o: $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiArtemis.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SdSpiArtemis.o $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiArtemis.cpp

$(BIN)/SdSpiChipSelect.o: $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiChipSelect.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SdSpiChipSelect.o $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiChipSelect.cpp

$(BIN)/SdSpiDue.o: $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiDue.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SdSpiDue.o $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiDue.cpp

$(BIN)/SdSpiESP.o: $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiESP.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SdSpiESP.o $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiESP.cpp

$(BIN)/SdSpiParticle.o: $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiParticle.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SdSpiParticle.o $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiParticle.cpp

$(BIN)/SdSpiSTM32.o: $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiSTM32.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SdSpiSTM32.o $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiSTM32.cpp

$(BIN)/SdSpiSTM32Core.o: $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiSTM32Core.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SdSpiSTM32Core.o $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiSTM32Core.cpp

$(BIN)/SdSpiTeensy3.o: $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiTeensy3.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/SdSpiTeensy3.o $(LIBRARIES)/SdFat/src/SpiDriver/SdSpiTeensy3.cpp

$(BIN)/FmtNumber.o: $(LIBRARIES)/SdFat/src/common/FmtNumber.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FmtNumber.o $(LIBRARIES)/SdFat/src/common/FmtNumber.cpp

$(BIN)/FsCache.o: $(LIBRARIES)/SdFat/src/common/FsCache.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FsCache.o $(LIBRARIES)/SdFat/src/common/FsCache.cpp

$(BIN)/FsDateTime.o: $(LIBRARIES)/SdFat/src/common/FsDateTime.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FsDateTime.o $(LIBRARIES)/SdFat/src/common/FsDateTime.cpp

$(BIN)/FsName.o: $(LIBRARIES)/SdFat/src/common/FsName.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FsName.o $(LIBRARIES)/SdFat/src/common/FsName.cpp

$(BIN)/FsStructs.o: $(LIBRARIES)/SdFat/src/common/FsStructs.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FsStructs.o $(LIBRARIES)/SdFat/src/common/FsStructs.cpp

$(BIN)/FsUtf.o: $(LIBRARIES)/SdFat/src/common/FsUtf.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/FsUtf.o $(LIBRARIES)/SdFat/src/common/FsUtf.cpp

$(BIN)/PrintBasic.o: $(LIBRARIES)/SdFat/src/common/PrintBasic.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/PrintBasic.o $(LIBRARIES)/SdFat/src/common/PrintBasic.cpp

$(BIN)/upcase.o: $(LIBRARIES)/SdFat/src/common/upcase.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/upcase.o $(LIBRARIES)/SdFat/src/common/upcase.cpp

$(BIN)/StdioStream.o: $(LIBRARIES)/SdFat/src/iostream/StdioStream.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/StdioStream.o $(LIBRARIES)/SdFat/src/iostream/StdioStream.cpp

$(BIN)/StreamBaseClass.o: $(LIBRARIES)/SdFat/src/iostream/StreamBaseClass.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/StreamBaseClass.o $(LIBRARIES)/SdFat/src/iostream/StreamBaseClass.cpp

$(BIN)/istream.o: $(LIBRARIES)/SdFat/src/iostream/istream.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/istream.o $(LIBRARIES)/SdFat/src/iostream/istream.cpp

$(BIN)/ostream.o: $(LIBRARIES)/SdFat/src/iostream/ostream.cpp $(HEADERS)
	g++ -c $(FLAGS) $(INCLUDE) -o $(BIN)/ostream.o $(LIBRARIES)/SdFat/src/iostream/ostream.cpp

.PHONY: clean
clean:
	rm -rf $(BIN)/*
