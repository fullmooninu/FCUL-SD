#!/usr/bin/env bash
if ! hash cppcheck &> /dev/null; then
  echo 'please install cppcheck: apt-get install cppcheck'
fi
if ! hash flawfinder &> /dev/null; then
  echo 'please install flawfinder: apt-get install flawfinder'
fi
make clean
make
flawfinder -C .
cppcheck ./include --enable=all .
./test_data
./test_entry
./test_list
