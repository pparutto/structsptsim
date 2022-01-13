#!/usr/bin/zsh

bin=/home/pierre/cam/SPT/ersptsim/release/gensim

D=1
DT=0.006
pxsize=0.0967821

# IFS=$'\n'
# for f in `ls $1 | grep poly$`; do
#     $bin --pxsize 0.0967821 --nframes 5000 --fov-size 128,128 --spot-density 0.000915527 --npts-fixed 11 "$1/$f" 0.0001 0.006 1.2 "$2/$f"
# done

tlenMax=201
tlen=1
while [[ $tlen -lt $tlenMax ]] ; do
    dir="$2/${tlen}"
    echo $dir
    if [ ! -d $dir ]; then
	$bin --export-poly-mat --pxsize $pxsize --fov-size 128,128 --poly "$1" --motion-D $D --dt 0.0001 --npts-fixed $tlen --ntrajs 1000 $DT $dir
    else
	echo "Skipped"
    fi
    tlen=$(($tlen + 1))
done
