#!/usr/bin/zsh

bin=/home/pierre/cam/SPT/ersptsim/release/gensim

DT=0.006

IFS=$'\n'
for f in `ls $1 | grep poly$ | grep 004`; do
    for D in 0.05 0.1 0.5 1 5 10 50; do
	for nspts in 5 10 15 20 25 30 35 40; do
	    dir="$2/${f}_${D}_${nspts}"
	    if [ ! -d $dir ]; then
		$bin --pxsize 0.0967821 --motion-D $D --nframes 5000 --fov-width 128 --fov-height 128 --spot-density `echo "$nspts/(128*128)" | bc -l` $dens --npts-exp 11 "$1/$f" 0.0001 $DT $dir
	    else
		echo "Skipped"
	    fi
	done
    done
done
