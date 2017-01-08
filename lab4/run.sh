#!/bin/bash
if [[ $# -lt 1 ]] ; then
	echo "Usage: $0 name of test"
fi
cp "turtle_$1" turtle && ./a.out < "test/$1"
