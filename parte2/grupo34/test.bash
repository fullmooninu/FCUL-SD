#!/usr/bin/env bash
if ! hash cppcheck &> /dev/null; then
  echo 'please install cppcheck: apt-get install cppcheck'
fi
if ! hash flawfinder &> /dev/null; then
  echo 'please install flawfinder: apt-get install flawfinder'
fi
make clean
make
#flawfinder -C table.c
cppcheck ./include --enable=all .
#./test_table
#./test_message
