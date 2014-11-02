#!/usr/bin/env bash

./debug.sh

valgrind --track-origins=yes --leak-check=full --suppressions=valgrind.supp ./build/Qompose

exit 0
