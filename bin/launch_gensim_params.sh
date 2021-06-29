#!/usr/bin/zsh

bin=/home/pierre/cam/SPT/ersptsim/release/gensim

#5
IFS=$'\n'
for f in `ls $1 | grep _001_ | grep poly$`; do
    for D in 0.2 0.6 1.0 1.4 1.8 2.2 2.6 3.0 3.4 3.8 4.2; do
	for nspts in 10 15 20 25 30 35; do
	    dir="$2/${f}_${D}_${nspts}"
	    if [ ! -d $dir ]; then
		$bin --pxsize 0.0967821 --motion-D $D --nframes 5000 --fov-width 128 --fov-height 128 --spot-density `echo "$nspts/(128*128)" | bc -l` $dens --npts-exp 11 "$1/$f" 0.0001 0.006 $dir
	    else
		echo "Skipped"
	    fi
	done
    done
done
