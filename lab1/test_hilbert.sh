#!/bin/bash

if [[ $# -lt 1 ]] ; then
	echo "Usage: $0 float|double|scal"
	exit 1
fi

if [[ ! -f "./$1.out" ]] ; then
	echo "id10t! File './$1.out' not found."
	exit 1
fi

ts=$(runghc Hilbert.hs | sed -e 's/[\[()]//g' -e 's/\]//g' -e 's/,/ /g')
last=$(echo "$ts" | wc -l)
for n in $(seq 1 $last) ; do
	res=$(echo -n "${ts}" | sed "${n}q;d" | ./$1.out)
	echo "$res"
done
