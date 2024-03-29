CC = g++ -std=c++17 -O3 -fPIC
DEFINES = -D__AVR__ -D__AVR_ATmega2560__ -DARDUINO_ARCH_AVR -DARDUINO_AVR_MEGA2560 -DARDUINO=100
WARNINGS = -Wno-unknown-attributes -Wno-address-of-packed-member
LIBRARIES = $(shell bundle exec arduino_library_location.rb)
PY_PATH = $(shell cd ../pybind11; python3 -m pybind11 --includes)
SUFFIX = $(shell python3-config --extension-suffix)
PY_LIB = ../libTC$(SUFFIX)
OBJDIR = obj

.PHONY: all
all : $(PY_LIB)

$(PY_LIB) : TankController.o ../pybind11/setup.py ../libTC.cpp
	echo "===== Compiling PY_LIB ($(PY_LIB)) =====" > /dev/null
	$(CC) -shared -fPIC						  					\
	$(DEFINES)                            	  \
	$(WARNINGS)                            	  \
	-Wl,-undefined,dynamic_lookup							\
	$(PY_PATH)																\
	-I$(TC_PATH)                      				\
	-I$(TC_PATH)/Devices               				\
	-I$(TC_PATH)/UIState               				\
	-I$(ARDUINO_CI)														\
	-I$(LIBRARIES)/Adafruit_BusIO/src					\
	-I$(LIBRARIES)/Adafruit_MAX31865/src			\
	-I$(LIBRARIES)/Arduino-PID-Library/src    \
	-I$(LIBRARIES)/Ethernet/src       				\
	-I$(LIBRARIES)/Ethernet/src/utility				\
	-I$(LIBRARIES)/Keypad/src       					\
	-I$(LIBRARIES)/LiquidCrystal/src  				\
	-I$(LIBRARIES)/RTClib/src									\
	-I$(LIBRARIES)/SdFat/src									\
	-I../pybind11/include											\
	-I../../src																\
	../libTC.cpp *.o -o $(PY_LIB)
	echo

TankController.o : Godmode.o
	echo "===== Compiling TankController =====" > /dev/null
	$(CC) -c                              			\
	$(DEFINES)                            			\
	$(WARNINGS)                            		  \
	-I$(TC_PATH)                      				  \
	-I$(TC_PATH)/Devices               			  	\
	-I$(TC_PATH)/UIState               			  	\
	-I$(ARDUINO_CI)															\
	-I$(LIBRARIES)/Adafruit_BusIO/src						\
	-I$(LIBRARIES)/Adafruit_MAX31865/src				\
	-I$(LIBRARIES)/Arduino-PID-Library/src    	\
	-I$(LIBRARIES)/Ethernet/src       					\
	-I$(LIBRARIES)/Ethernet/src/utility					\
	-I$(LIBRARIES)/Keypad/src       						\
	-I$(LIBRARIES)/LiquidCrystal/src  					\
	-I$(LIBRARIES)/RTClib/src										\
	-I$(LIBRARIES)/SdFat/src										\
	$(TC_PATH)/*.cpp                      			\
	$(TC_PATH)/Devices/*.cpp              			\
	$(TC_PATH)/UIState/*.cpp              			\
	$(LIBRARIES)/Adafruit_BusIO/src/*.cpp				\
	$(LIBRARIES)/Adafruit_MAX31865/src/*.cpp		\
	$(LIBRARIES)/Arduino-PID-Library/src/*.cpp	\
	$(LIBRARIES)/Ethernet/src/*.cpp							\
	$(LIBRARIES)/Ethernet/src/utility/*.cpp 		\
	$(LIBRARIES)/Keypad/src/*.cpp								\
	$(LIBRARIES)/LiquidCrystal/src/*.cpp  			\
	$(LIBRARIES)/RTClib/src/*.cpp 							\
	$(LIBRARIES)/SdFat/src/*.cpp 						  	\
	$(LIBRARIES)/SdFat/src/*/*.cpp 							\

	echo

Godmode.o :
	echo "===== Compiling Arduino CI mocks =====" > /dev/null
	$(CC) -c          \
	$(DEFINES)        \
	-I$(ARDUINO_CI)   \
	$(ARDUINO_CI)/*.cpp
	echo

.ONESHELL:
../pybind11/setup.py :
	echo "===== Install pybind11 =====" > /dev/null
	git submodule update --init --recursive
	# install pytest
	python -m pip install pytest
	# install wx
	python -m pip install -U wxPython
	# build pybind11
	mkdir -p ../pybind11/.build
	cd ../pybind11/.build; cmake ..
	# compile and run tests
	# make check -j 4
	echo

.PHONY: clean
clean :
	rm -rf .build *.o *.so *.dylib $(PY_LIB) 2> /dev/null || echo
