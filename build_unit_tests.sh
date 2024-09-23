#!/bin/zsh -x

# Run unit tests
rm -rf tests/build
cmake -S tests -B tests/build
cmake --build tests/build
