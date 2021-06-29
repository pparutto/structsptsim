#!/usr/bin/zsh

bin=/home/pierre/cam/SPT/ersptsim/release/gensim

IFS=$'\n'
for f in `ls $1 | grep poly$`; do
    $bin --pxsize 0.0967821 --nframes 5000 --fov-width 128 --fov-height 128 --spot-density 0.000915527 --npts-lambda 11 "$1/$f" 0.0001 0.006 1.2 "$2/$f"
done
