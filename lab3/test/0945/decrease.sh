if [[ $# -lt 1 ]] ; then
	echo "Usage: $0 file" >&2
	exit 1
fi

mapfile -t arr < $1
for i in "${arr[@]}" ; do
	echo "$(expr $i - 1)"
	#echo $i
done
