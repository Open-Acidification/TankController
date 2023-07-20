#!/bin/bash
(cd .git; mkdir -p hooks)
cp extras/scripts/p*-commit .git/hooks/
bundle config set --local path 'vendor/bundle'
bundle install
bundle exec arduino_ci.rb --min-free-space=5790 1> >(tee stdout.txt ) 2> >(tee stderr.txt >&2 )
result="${PIPESTATUS[0]}"
exit "$result"
