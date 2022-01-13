#!/usr/bin/zsh

bin=/home/pierre/cam/SPT/ersptsim/release/gensim


tlenMax=101

#use --fov-width 128 --fov-height 128
#to simulate microscope fov

tlen=100
while [[ $tlen -lt $tlenMax ]] ; do
    dir="$1/${tlen}"
    echo $dir
    if [ ! -d $dir ]; then
	$bin --start-point 10,10 --motion-D 50 --dt 0.0001 --npts-fixed $tlen --ntrajs 1000 0.006 $dir
    else
	echo "Skipped"
    fi
    tlen=$(($tlen + 1))
done
