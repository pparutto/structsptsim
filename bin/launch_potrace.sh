#!/usr/bin/zsh

bin="/home/pierre/cam/potrace-1.16.linux-x86_64/potrace"

for f in `ls -1 $1 | grep "pgm$"`; do
    $bin -a 0 -b svg -o "$1/${f%.pgm}_poly.svg" "$1/$f"
done
