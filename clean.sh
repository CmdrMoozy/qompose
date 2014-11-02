#!/bin/bash
#
# Script to clean our source tree.

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

if [[ ! -d ./build ]]; then
	mkdir build
fi

rm -r build/*

exit 0
