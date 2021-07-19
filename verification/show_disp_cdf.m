dat = csvread('/mnt/data/SPT_method/analysis_opti/Mito/201217/Mito Halo-N1-paJF646_001.czi/ivels_th=2400_dist=0.6_minLen=5.csv');


figure
hist(dat, sqrt(length(dat)))
