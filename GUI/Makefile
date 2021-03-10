CC = g++ -std=c++17 -O3 -fPIC
DEFINES = -D__AVR__ -D__AVR_ATmega2560__ -DARDUINO_ARCH_AVR -DARDUINO_AVR_MEGA2560 -DARDUINO=100
ifndef ARDUINO_CI
	ARDUINO_CI = $(HOME)/Arduino/arduino_ci/cpp/arduino
endif
ifndef LIBRARIES
	LIBRARIES = $(shell bundle exec arduino_library_location.rb)
endif
TC_PATH = $(LIBRARIES)/TankControllerLib/src
PY_PATH = $(shell cd ../pybind11; python3 -m pybind11 --includes)
SUFFIX = $(shell python3-config --extension-suffix)
PY_LIB = ../libTC$(SUFFIX)
OBJDIR = obj

.PHONY: all
all : $(PY_LIB)

$(PY_LIB) : TankControllerLib.o ../pybind11/setup.py ../libTC.cpp
	echo "===== Compiling PY_LIB ($(PY_LIB)) =====" > /dev/null
	$(CC) -shared -fPIC               				\
	-Wl,-undefined,dynamic_lookup     				\
	$(PY_PATH)                        				\
	-I$(TC_PATH)                      				\
	-I$(ARDUINO_CI)                   				\
	-I$(LIBRARIES)/Adafruit_BusIO/src					\
	-I$(LIBRARIES)/Adafruit_MAX31865/src			\
	-I$(LIBRARIES)/LiquidCrystal/src  				\
	-I$(LIBRARIES)/RTClib/src         				\
	-I$(LIBRARIES)/Ethernet/src       				\
	-I$(LIBRARIES)/Ethernet/src/utility				\
	-I$(LIBRARIES)/Keypad/src       					\
	-I../pybind11/include					\
	-I../../src										\
	../libTC.cpp *.o -o $(PY_LIB)
	echo

TankControllerLib.o : Godmode.o
	echo "===== Compiling TankControllerLib =====" > /dev/null
	$(CC) -c                              		\
	$(DEFINES)                            		\
	-I$(TC_PATH)                          		\
	-I$(ARDUINO_CI)                       		\
	-I$(LIBRARIES)/Adafruit_BusIO/src					\
	-I$(LIBRARIES)/Adafruit_MAX31865/src			\
	-I$(LIBRARIES)/LiquidCrystal/src      		\
	-I$(LIBRARIES)/RTClib/src             		\
	-I$(LIBRARIES)/Ethernet/src       				\
	-I$(LIBRARIES)/Ethernet/src/utility				\
	-I$(LIBRARIES)/Keypad/src       					\
	$(TC_PATH)/*.cpp                      		\
	$(TC_PATH)/Devices/*.cpp              		\
	$(TC_PATH)/UIState/*.cpp              		\
	$(LIBRARIES)/Adafruit_BusIO/src/*.cpp			\
	$(LIBRARIES)/Adafruit_MAX31865/src/*.cpp	\
	$(LIBRARIES)/LiquidCrystal/src/*.cpp  		\
	$(LIBRARIES)/RTClib/src/*.cpp 						\
	$(LIBRARIES)/Ethernet/src/*.cpp						\
	$(LIBRARIES)/Ethernet/src/utility/*.cpp 	\
	$(LIBRARIES)/Keypad/src/*.cpp							\

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
	mkdir -p ../pybind11/build
	cd ../pybind11/build; cmake ..
	# compile and run tests
	# make check -j 4
	echo

.PHONY: clean
clean :
	rm *.o *.so *.dylib $(PY_LIB) 2> /dev/null || echo
