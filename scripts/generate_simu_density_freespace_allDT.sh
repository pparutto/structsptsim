#!/usr/bin/zsh

bin=/home/pierre/cam/SPT/ersptsim/release/genfree2Dsim
nobound=''
width=420
height=420
pxsize=0.024195525
tlambda=13
dt=0.0001
D=1

Nrep=5

DTs=(0.0001 0.0006 0.0025 0.0056 0.01 0.0156 0.0625 0.146 0.25 0.3906 0.5625 0.7656 1)
Nspots=(1 5 10 15 20 25 30 35 40 45 50)
Nframes=(60000 12000 6000 4000 3000 2400 2000 1715 1500 1334 1200)

outdir=$1

if [ $# -ne 1 ]; then
    echo $0 outdir
    exit 2
fi

n=1
Nrep=5
while [[ n -le $Nrep ]]; do
    echo $outdir
    mkdir -p "$outdir/$n"

    k=1
    while [[ k -le ${#DTs[@]} ]]; do
	i=1
	while [[ i -le ${#Nspots[@]} ]]; do
	    outfile="$outdir/$n/${D}_${Nspots[$i]}_${DTs[$k]}_${Nframes[$i]}"
	    if [ -f "$outfile/trajs.csv" ]; then
		echo "SKIPPED: $outfile"
	    else
		dens=`echo "$Nspots[$i] / ($width * $height - 1)" | bc -l`
		#$bin --noimg --fixed-size $nobound $width $height $Nframes[$i] $pxsize $tlambda $dt $DTs[$k] $D $dens "$outfile"
		$bin --noimg $width $height $Nframes[$i] $pxsize $tlambda $dt $DTs[$k] $D $dens "$outdir/$n/${D}_${Nspots[$i]}_${DTs[$k]}_${Nframes[$i]}"
	    fi
	    i=$(($i+1))	
	done
	k=$(($k+1))
    done
    n=$(($n+1))
done
