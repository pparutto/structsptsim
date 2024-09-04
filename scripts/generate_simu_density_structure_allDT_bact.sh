#!/usr/bin/zsh

bin=/home/pierre/cam/ersptsim/release/gensim
width=250
height=250
pxsize=0.03225015
tlambda=13
dt=0.0001
D=1

Nrep=5

DTs=(0.0001 0.0006 0.0025 0.0056 0.01 0.0156 0.0625 0.146 0.25 0.3906 0.5625 0.7656 1)
Nspots=(1 2 3 4 5 6 7 8 9 10 11)
Nframes=(60000 30000 20000 15000 12000 10000 8572 7500 6667 6000 5455)

if [ $# -ne 2 ]; then
    echo $0 structdir outdir
    echo " Runs simulation for all .poly files present in structdir and saves the results in outdir"
    exit 2
fi


IFS=$'\n'
n=2
while [[ n -le $Nrep ]]; do
    for f in `ls $1 | grep poly$`; do
	outdir="$2/$f/$n"
	echo $outdir
	mkdir -p $outdir

	k=1
	while [[ k -le ${#DTs[@]} ]]; do
	    i=1
	    while [[ i -le ${#Nspots[@]} ]]; do
		if [[ ! -f "$outdir/${D}_${Nspots[$i]}_${DTs[$k]}_${Nframes[$i]}/trajs.csv" ]]; then
		    dens=`echo "$Nspots[$i] / ($width * $height - 1)" | bc -l`
		    echo $bin --export-poly-txt --poly "$1/$f" --pxsize $pxsize --noimg --fov-size "$width,$height" --nframes $Nframes[$i] --npts-exp $tlambda --dt $dt --motion-D $D --spot-density $dens $DTs[$k] "$outdir/${D}_${Nspots[$i]}_${DTs[$k]}_${Nframes[$i]}"
		    $bin --export-poly-txt --poly "$1/$f" --pxsize $pxsize --noimg --fov-size "$width,$height" --nframes $Nframes[$i] --npts-exp $tlambda --dt $dt --motion-D $D --spot-density $dens $DTs[$k] "$outdir/${D}_${Nspots[$i]}_${DTs[$k]}_${Nframes[$i]}"
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
