#!/bin/bash
#make float
#make double
di1="16"
offset=".1"
di_r="5 6 6 13 12 15 12 3 5"
x1="87.9"
x_r="-27 -19 -9 -10 12 62 52 14 34"
res=""
while [[ $(expr "$res" : ".nan") == "0" ]] ; do
	res=$(printf \
		"10\n0 0 1 3 6 10 15 21 28 36 45\n\
		${di1}${offset} ${di_r}\n\
-1 -1 0 -1 0 0 -2 0 -1 0 -1 0 0 -4 -4 -4 0 -3 0 -3 -1 -4 -2 0 -1 -2 -2 0 -1 0 0 0 -1 0 -1 0 -1 -2 -1 0 0 0 0 -1 0 \n\
		-${x1} ${x_r}" | ./a.out)
	echo "o: ${offset}"
	echo "x1: ${x1}"
	echo ${res}
	offset=$(printf ".0%s" "$(echo "$offset" | sed 's/\.//')")
	x1=$( echo "88-$offset" | bc )
done
