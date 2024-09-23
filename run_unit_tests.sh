#!/bin/zsh -x

# if build directory doesn't exist, run build_unit_tests.sh
if [ ! -d "tests/build" ]; then
    ./build_unit_tests.sh
fi

# Run unit tests
ctest --test-dir tests/build
