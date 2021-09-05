#! /bin/sh
bundle install --path vendor/bundle
bundle exec arduino_ci.rb --min-free-space=5800 | tee output.txt
tail -n 4 output.txt | head -n 2 > size.txt
