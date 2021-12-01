#! /bin/sh
bundle config set --local path 'vendor/bundle'
bundle install
set -o pipefail
bundle exec arduino_ci.rb --min-free-space=6000 | tee output.txt; result=$?
set +o pipefail
tail -n 4 output.txt | head -n 2 > size.txt
exit "$result"
