#!/bin/bash
targ=$(cat test.tex | grep '\& [0-9]\+\.*[0-9]* \\\\' | head)
echo "$targ" | sed '6q;d'
n=$(echo -n "$targ" | wc -l)
res=""

for row in $(seq 1 $n) ; do
	str=$(echo -n "$targ" | sed "${row}q;d")
	echo "row = $row"
	# TODO make for all, not for 11
	for col in $(seq 2 11) ; do
		delta=""
		num=$( echo "$str" | \
			awk -F"&" "/1/ {print \$$col}" | sed 's/[^0-9.]//g')
		h=$(echo "$num" | sed 's/\.[0-9]\+$//')
		t=$(echo "$num" | sed 's/^[0-9]\+\.//')
		ord=$(expr "$(echo "$t" | wc -c)" - 1)
		if [[ "$(echo "$num" | sed 's/[^\.]//g')" != "." ]] ; then
			delta=$num
		elif [[ $h -eq $(expr $col - 2) ]] ; then
			delta="-$(echo "10^$ord - $t" | bc)e-$ord"
		else
			delta=$(printf "$(echo "$t" | bc)e-$ord")
		fi
		str="$(echo $str | sed "s/$(echo "$num" | \
			sed 's/\./\\\./')/$delta/")"
	done
	res=$(printf "$res\n$str")
done

echo "$res"
