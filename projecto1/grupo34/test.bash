#!/usr/bin/env bash

make
cppcheck .
./testData
./testEntry
./testList
