#! /bin/bash
(cd .git; mkdir -p hooks)
cp extras/scripts/p*-commit .git/hooks/
bundle config set --local path 'vendor/bundle'
bundle install
if [ -z "$1" ]; then
  bundle exec arduino_ci.rb --skip-examples-compilation 1> >(tee stdout.txt ) 2> >(tee stderr.txt >&2 ); result=$?
else
  bundle exec arduino_ci.rb --skip-examples-compilation --testfile-select="$1" 1> >(tee stdout.txt ) 2> >(tee stderr.txt >&2 ); result=$?
fi
exit "$result"
