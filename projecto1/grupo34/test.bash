#!/usr/bin/env bash
make clean
make
cppcheck .
./test_data
./test_entry
./test_list
