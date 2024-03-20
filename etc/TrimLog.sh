#!/bin/sh

files=""
while [ $# -gt 1 ]
do
	files="${files} $1"
	shift
done

max=$1

for f in $files
do
	lines=$(cat $f | wc -l | sed -e's/ //g')
	if [ $lines -gt $max ]
	then
		tail -$max $f > $0.tmpfile
		mv $0.tmpfile $f
	fi
done

	
