#!/usr/bin/env bash

if ! hash cppcheck &> /dev/null; then
    echo 'please install cppcheck: apt-get install cppcheck'
fi

make clean
make
cppcheck .
./test_data
./test_entry
./test_list
