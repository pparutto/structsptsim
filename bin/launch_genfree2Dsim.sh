#!/usr/bin/zsh

bin=/home/pierre/cam/SPT/ersptsim/release/genfree2Dsim

DT=0.006


IFS=$'\n'
for D in 0.05 0.1 0.5 1 5 10 50; do
    for nspts in 5 10 15 20 25 30 35 40; do
	dir="$1/${f}_${D}_${nspts}"
	echo $dir
	if [ ! -d $dir ]; then
	    $bin 128 128 5000 0.0967821 11 0.0001 $DT $D `echo "$nspts/(128*128)" | bc -l` $dir
	else
	    echo "Skipped"
	fi
    done
done
