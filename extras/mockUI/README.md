# TankControllerGUI
A Python GUI for TankController running on the mocks provided by `Arduino CI`.

## Build Process (Mac/Linux)
Follow these steps
* `python3 -m pip install wxPython pybind11`
* `cd extras/mockUI`
* ``filePath=`bundle exec which arduino_library_location.rb` ``
* `export ARDUINO_CI=${filePath%/bin/*}/gems/arduino_ci-1.3.0/cpp/arduino`
* `make clean`
* `(mkdir -p build; cd build; make -f ../Makefile)`
* `python3 TankController.py`

## Build Process (Windows)
Follow these steps
* `install Ruby via https://rubyinstaller.org`
* In the Ruby command prompt `gem install bunder arduino_ci`
* `python3 -m pip install wxPython pybind11`
* `cd extras/mockUI`
* `irb`
* `require 'arduino_ci'; puts Gem::Specification.find_by_name('arduino_ci').gem_dir`
* Copy the returned directory
* `exit`
* In powershell terminal `$env:ARDUINO_CI = "[INSERT DIRECTORY HERE]"`
* `echo $env:ARDUINO_CI` Should return the directory that was given earlier
* `make clean`
* If the make file fails, ensure you have make installed on the MINGW64 version of Git Bash
* `(mkdir -p build; cd build; make -f ../Makefile)`
* `python3 TankController.py`

## Makefile Specs
The `Makefile` provides for making the following:
* `libtc.dylib` (on macOS) containing the following:
  * `Arduino CI` mocks (essentially the Arduino OS)
  * `TankController` (our library that can be called from a trivial sketch)
* `libTC.*` is a start on a Python extension
  * `pybind11` is used to build the Python extension
