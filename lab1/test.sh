#!/bin/bash
make float double scal > /dev/null
di1="16"
di_r="5 6 6 13 12 15 12 3 5"
x_r="-27 -19 -9 -10 12 62 52 14 34"

echo "\documentclass[oneside, final, 12pt]{extarticle}"
echo "\usepackage[utf8]{inputenc}"
echo "\usepackage[russian]{babel}"
echo "\usepackage{vmargin}"
echo "\usepackage{listings}"
echo "\usepackage{graphicx}"
echo "\usepackage{amsmath}"
echo "\usepackage{longtable}"
echo "\setpapersize{A4}"
echo "\setmarginsrb{2cm}{2cm}{2cm}{2cm}{0pt}{0mm}{0pt}{13mm}"
echo "\usepackage{indentfirst}%красная строка"
echo "\sloppy"
echo ""
echo "\begin{document}"
echo "\section{Исследования}"
echo "\subsection{Диагональное преобладание}"
#echo "\resizebox{\textwidth}{!} {"
echo "\begin{longtable}{|c|c|c|c|c|c|c|} \\hline"
echo "    ~ & \\multicolumn{2}{|c|}{Одинарная} & \\multicolumn{2}{|c|}{Двойная} & \\multicolumn{2}{|c|}{Скалярн. пр.} \\\\ \\hline"
echo "    \(k\) & \(x^k\) & \(x^*-x^k\) & \(x^k\) & \(x^*-x^k\) & \(x^k\) & \(x^*-x^k\) \\\\ \\hline"

req="10\n0 0 1 3 6 10 15 21 28 36 45\n\
	17 ${di_r}\n\
-1 -1 0 -1 0 0 -2 0 -1 0 -1 0 0 -4 -4 -4 0 -3 0 -3 -1 -4 -2 0 -1 -2 -2 0 -1 0 0 0 -1 0 -1 0 -1 -2 -1 0 0 0 0 -1 0 \n\
	-87 ${x_r}"
rf=$(printf "$req" | ./float.out)
rd=$(printf "$req" | ./double.out)
rs=$(printf "$req" | ./scal.out)

echo -n "    \(0\) "
for res in "$rf" "$rd" "$rs" ; do
	echo "& \\( \\begin{aligned}"
	echo "${res}" | sed 's/\([\.0-9]\+\)/\& \1 \\\\/g' \
		| sed 's/.\{3\}$//'
	echo "\end{aligned} \\)"
	echo "& ~ "
done
echo "\\\\ \\hline"

offset=".1"
x1="87.9"
for k in $(seq 1 15) ; do
	req="10\n0 0 1 3 6 10 15 21 28 36 45\n\
		${di1}${offset} ${di_r}\n\
-1 -1 0 -1 0 0 -2 0 -1 0 -1 0 0 -4 -4 -4 0 -3 0 -3 -1 -4 -2 0 -1 -2 -2 0 -1 0 0 0 -1 0 -1 0 -1 -2 -1 0 0 0 0 -1 0 \n\
		-${x1} ${x_r}"
	rf=$(printf "$req" | ./float.out)
	rd=$(printf "$req" | ./double.out)
	rs=$(printf "$req" | ./scal.out)

	echo -n "    \($k\) "
	for res in "$rf" "$rd" "$rs" ; do
		echo "& \\( \\begin{aligned}"
		echo "${res}" | sed 's/\([\.0-9]\+\)/\& \1 \\\\/g' \
			| sed 's/.\{3\}$//'
		echo "\end{aligned} \\)"
		echo "& ~ "
	done
	echo "\\\\ \\hline"

	offset=$(printf ".0%s" "$(echo "$offset" | sed 's/\.//')")
	x1=$( echo "88-$offset" | bc )
done

echo "\end{longtable}"
#echo "}" #resizebox

echo "\subsection{Матрица Гильберта}"
echo "\begin{longtable}{|c|c|c|c|c|c|c|} \\hline"
echo "    ~ & \\multicolumn{2}{|c|}{Одинарная} & \\multicolumn{2}{|c|}{Двойная} & \\multicolumn{2}{|c|}{Скалярн. пр.} \\\\ \\hline"
echo "    \(k\) & \(x^k\) & \(x^*-x^k\) & \(x^k\) & \(x^*-x^k\) & \(x^k\) & \(x^*-x^k\) \\\\ \\hline"
rf=$(./test_hilbert.sh float)
rd=$(./test_hilbert.sh double)
rs=$(./test_hilbert.sh scal)
for k in $(seq 3 10) ; do
	# remove nans
	echo -n "    \($k\) "
	for res in "$rf" "$rd" "$rs" ; do
		echo "& \\( \\begin{aligned}"
		echo "${res}" | sed "$(expr $k - 2)q;d" | sed 's/\([\.0-9]\+\)/\& \1 \\\\/g' \
			| sed 's/.\{3\}$//'
		echo "\end{aligned} \\)"
		echo "& ~ "
	done
	echo "\\\\ \\hline"
done
echo "\end{longtable}"

echo "\end{document}"
