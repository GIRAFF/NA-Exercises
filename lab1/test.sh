#!/bin/bash
make float
#make double
di1="5"
offset=".1"
di_r="2 5 6"
x1="10.9"
x_r="-1 1 11"
res=""
while [[ $(expr "$res" : ".nan") == "0" ]] ; do
	res=$(printf \
		"4\n0 0 1 3 6\n\
		${di1}${offset} ${di_r}\n\
		-1 -2 0 -2 -1 -3\n\
		-${x1} ${x_r}" | ./a.out)
	echo "o: ${offset}"
	echo "x1: ${x1}"
	echo ${res}
	offset=$(printf ".0%s" "$(echo "$offset" | sed 's/\.//')")
	x1=$( echo "11-$offset" | bc )
done
