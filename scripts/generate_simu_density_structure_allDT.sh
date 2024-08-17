#!/usr/bin/zsh

bin=/home/pierre/cam/SPT/ersptsim/release/gensim
width=420
height=420
pxsize=0.024195525
tlambda=13
dt=0.0001
D=1

Nrep=10

DTs=(0.0001 0.0006 0.0025 0.0056 0.01 0.0156 0.0625 0.146 0.25 0.3906 0.5625 0.7656 1)
Nspots=(1 5 10 15 20 25 30 35 40 45 50)
Nframes=(60000 12000 6000 4000 3000 2400 2000 1715 1500 1334 1200)

if [ $# -ne 2 ]; then
    echo $0 structdir outdir
    echo " Runs simulation for all .poly files present in structdir and saves the results in outdir"
    exit 2
fi


IFS=$'\n'
n=2
while [[ n -le $Nrep ]]; do
    for f in `echo $3`; do
	outdir="$2/$f/$n"
	echo $outdir
	mkdir -p $outdir

	k=1
	while [[ k -le ${#DTs[@]} ]]; do
	    i=1
	    while [[ i -le ${#Nspots[@]} ]]; do
		if [[ ! -f "$outdir/${D}_${Nspots[$i]}_${DTs[$k]}_${Nframes[$i]}/trajs.csv" ]]; then
		    dens=`echo "$Nspots[$i] / ($width * $height - 1)" | bc -l`
		    echo $bin --export-poly-mat --poly "$1/$f" --pxsize $pxsize --noimg --fov-size "$width,$height" --nframes $Nframes[$i] --npts-exp $tlambda --dt $dt --motion-D $D --spot-density $dens $DTs[$k] "$outdir/${D}_${Nspots[$i]}_${DTs[$k]}_${Nframes[$i]}"
		    $bin --export-poly-mat --poly "$1/$f" --pxsize $pxsize --noimg --fov-size "$width,$height" --nframes $Nframes[$i] --npts-exp $tlambda --dt $dt --motion-D $D --spot-density $dens $DTs[$k] "$outdir/${D}_${Nspots[$i]}_${DTs[$k]}_${Nframes[$i]}"
		else
		    echo "[SKIPPED] $outdir/${D}_${Nspots[$i]}_${DTs[$k]}_${Nframes[$i]}/trajs.csv"
		fi
		i=$(($i+1))
	    done
	    k=$(($k+1))
	done
    done
    n=$(($n+1))
done
