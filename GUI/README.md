# TankControllerGUI
A Python GUI for TankController running on the mocks provided by `Arduino CI`

## Build Process
Follow these steps
* `python3 -m pip install wxPython pybind11`
* `cd GUI`
* ``filePath=`bundle exec which arduino_library_location.rb` ``
* `ARDUINO_CI=${filePath%/*}/`
* `make clean`
* `(mkdir -p build; cd build; make -f ../Makefile)`
* `python3 TankController.py`

The `Makefile` provides for making the following:
* `libtc.dylib` (on macOS) containing the following:
  * `Arduino CI` mocks (essentially the Arduino OS)
  * `TankControllerLib` (our library that can be called from a trivial sketch)
* `libTC.*` is a start on a Python extension
  * `pybind11` is used to build the Python extension
