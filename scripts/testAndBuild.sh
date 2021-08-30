#! /bin/sh
bundle install --path vendor/bundle
bundle exec arduino_ci.rb --min-free-space=5800
