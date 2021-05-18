#! /bin/sh
bundle config --local path vendor/bundle
bundle install
if [ -z "$1" ]; then
  bundle exec arduino_ci.rb --skip-examples-compilation
else
  bundle exec arduino_ci.rb --skip-examples-compilation --testfile-select=$1
fi
