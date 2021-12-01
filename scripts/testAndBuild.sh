#! /bin/sh
bundle config set --local path 'vendor/bundle'
bundle install
bundle exec arduino_ci.rb --min-free-space=6000 | tee output.txt
result=${PIPESTATUS[0]}
tail -n 4 output.txt | head -n 2 > size.txt
exit $result
