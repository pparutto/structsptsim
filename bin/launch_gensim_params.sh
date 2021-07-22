#!/usr/bin/zsh

bin=/home/pierre/cam/SPT/ersptsim/release/gensim

DT=0.006

IFS=$'\n'
for f in `ls $1 | grep poly$`; do
    for D in 0.05 0.1 0.5 1 5 10 50; do
	for nspts in 10; do
	    dir="$2/${f}_${D}_${nspts}"
	    if [ ! -d $dir ]; then
		$bin --export-poly-mat --pxsize 0.0967821 --motion-D $D --dt 0.0001 --nframes 5000 --fov-width 128 --fov-height 128 --spot-density `echo "$nspts/(128*128)" | bc -l` $dens --npts-exp 11 --poly "$1/$f" $DT $dir
	    else
		echo "Skipped"
	    fi
	done
    done
done
