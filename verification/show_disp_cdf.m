dat = csvread('/mnt/data/SPT_method/tracking_opti/Mito/200727/Mito_Halo-N1-paJF646_001.tif/rad:0.75_th:2600.0_dist:0.6_distgap:0.6_framegap:0/disps_cdf_delta=0.01.csv');

figure
plot(dat(:,1) + (dat(2,1) - dat(1,1)) / 2, dat(:,2))