#!/usr/bin/zsh

bin=/home/pierre/cam/SPT/ersptsim/release/gensim

# DT=0.2
# FOVw=1354
# FOVh=1262
# pxsize=0.0645
# dt=0.0001
# Nframes=1000
# avgNptsLen=100

DT=0.006
FOVw=128
FOVh=128
pxsize=0.0967821
dt=0.0001
Nframes=5000
avgNptsLen=11

#0.05 0.1 0.5 1 5 10 50
#5 10 15 20 25 30 35 40 45 50

IFS=$'\n'
for f in `ls $1 | grep poly$ | grep 004`; do
    for D in 10 50; do
	for nspts in 5 10 15 20 25 30 35 40 45 50; do
	    dir="$2/${f}_${D}_${nspts}"
	    if [ ! -d $dir ]; then
		echo $bin --export-poly-mat --pxsize $pxsize --motion-D $D --dt $dt --nframes $Nframes --fov-width $FOVw --fov-height $FOVh --spot-density `echo "$nspts/($FOVw*$FOVh)" | bc -l` $dens --npts-exp $avgNptsLen --poly "$1/$f" $DT $dir
		$bin --export-poly-mat --pxsize $pxsize --motion-D $D --dt $dt --nframes $Nframes --fov-width $FOVw --fov-height $FOVh --spot-density `echo "$nspts/($FOVw*$FOVh)" | bc -l` $dens --npts-exp $avgNptsLen --poly "$1/$f" $DT $dir
	    else
		echo "Skipped"
	    fi
	done
    done
done
