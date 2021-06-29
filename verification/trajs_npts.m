%disps_bins = 0.0004:0.0008:0.02;
disps_bins = 0.0025:0.005:2;

%tab = dlmread('/mnt/data/SPT_method/simu/mito/simus/Mito Halo-N1-paJF646_006_mask_4208_poly.poly_2.2_25/trajs_D=2.200000_dt=0.000100_DT=0.006000_lambdaNpts=11.000000_nframes=0_width=128_height=128_dens=0.001526.csv');
%tab = dlmread('/mnt/data/SPT_method/simu/mito/simus/Mito Halo-N1-paJF646_006_mask_4208_poly.poly_2.2_5/trajs_D=2.200000_dt=0.000100_DT=0.006000_lambdaNpts=11.000000_nframes=0_width=128_height=128_dens=0.000305.csv');
tab = dlmread('/tmp/f/trajs.csv');

npts = arrayfun(@(i) sum(tab(:,1) == i), unique(tab(:,1)));

figure
hist(npts)