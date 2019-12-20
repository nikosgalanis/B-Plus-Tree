#!/bin/bash
rm *.db
make test
./build/test_main
