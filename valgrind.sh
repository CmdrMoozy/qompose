#!/usr/bin/env bash

./debug.sh

if [[ $? -ne 0 ]];
then
	echo "FATAL: Building Qompose in debug mode failed."
	exit 1
fi

valgrind --track-origins=yes --leak-check=full --gen-suppressions=yes --suppressions=valgrind.supp ./build/Qompose

exit 0

