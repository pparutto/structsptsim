#!/usr/bin/zsh

bin=/home/pierre/cam/SPT/ersptsim/release/gensim

for f in `ls $1 | grep path$`; do
    $bin --pxsize 0.0967821 $1/$f 0.0001 0.006 1.6 5000 20 $2/$f
done
