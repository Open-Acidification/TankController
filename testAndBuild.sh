#! /bin/sh
bundle config --local path vendor/bundle
bundle exec arduino_ci_remote.rb
