#!/bin/bash
#
# Script to clean up & rebuild Qompose.
# This will build our application in RELEASE mode.
#
# NOTE: Any arguments passed to this script will be propgated along to cmake. This can be
# used, e.g., to set various cmake defines or etc. for a custom build type.

# Clean up our previous build, if any.

./clean.sh

if [[ $? -ne 0 ]];
then
	echo "FATAL: Couldn't find clean.sh. Are you in the root of the source tree?"
	exit 1
fi

# Make sure the directories we need exist.

./dir-prepare.sh

if [[ $? -ne 0 ]];
then
	echo "FATAL: Couldn't find dir-prepare.sh. Are you in the root of the source tree?"
	exit 1
fi

# Build the project.

cd build
cmake -DCMAKE_BUILD_TYPE=Release "$@" ..
make
cd ..
