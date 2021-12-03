#! /bin/sh
bundle config set --local path 'vendor/bundle'
bundle install
if [ -z "$1" ]; then
  bundle exec arduino_ci.rb --skip-examples-compilation; result=$?
else
  bundle exec arduino_ci.rb --skip-examples-compilation --testfile-select="$1"; result=$?
fi
exit "$result"
