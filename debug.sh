#!/bin/bash
#
# Script to clean up & rebuild Qompose.
# This will build our application in DEBUG mode.
#
# NOTE: Any arguments passed to this script will be propgated along to cmake. This can be
# used, e.g., to set various cmake defines or etc. for a custom build type.

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

./clean.sh

cd build
cmake -DCMAKE_BUILD_TYPE=Debug "$@" ..
make
cd ..
