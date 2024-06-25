#! /bin/bash
bundle config set --local path 'vendor/bundle'
bundle install
bundle exec arduino_ci.rb --min-free-space=5672 --skip-unittests 1> >(tee stdout.txt ) 2> >(tee stderr.txt >&2 ); result=$?
exit "$result"
