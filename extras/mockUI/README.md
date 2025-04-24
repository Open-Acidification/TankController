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
* `gem install bunder arduino_ci`
* `python3 -m pip install wxPython pybind11`
* `cd extras/mockUI`
* `irb`
* `require 'arduino_ci'; puts Gem::Specification.find_by_name('arduino_ci').gem_dir`
* `copy the returned directory`
* `exit`
* In powershell terminal `$env:ARDUINO_CI = "[INSERT DIRECTORY HERE]"`
* `echo $env:ARDUINO_CI` Should return the directory that was given earlier
* `make clean`
* If the make file fails, launch the Git Bash terminal in the IDE and retry
* `(mkdir -p build; cd build; make -f ../Makefile)`
* `python3 TankController.py`

The `Makefile` provides for making the following:
* `libtc.dylib` (on macOS) containing the following:
  * `Arduino CI` mocks (essentially the Arduino OS)
  * `TankController` (our library that can be called from a trivial sketch)
* `libTC.*` is a start on a Python extension
  * `pybind11` is used to build the Python extension
