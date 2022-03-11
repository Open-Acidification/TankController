#!/bin/bash
bundle config set --local path 'vendor/bundle'
bundle install
bundle exec arduino_ci.rb --min-free-space=5900 1> >(tee stdout.txt ) 2> >(tee stderr.txt >&2 )
result="${PIPESTATUS[0]}"
tail -n 3 stdout.txt | head -n 1 > size.txt
exit "$result"
