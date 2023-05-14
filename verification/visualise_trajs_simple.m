addpath('../external')

%fname = '/tmp/toto/C2-211217_COS7_4MTS-Halo-PA646_4MTS-mNG_DMSO_1.czi.tif_avg17.tif_musical_NImgs=11_NFrames=10_th=4.0_alpha=6.0_gauss=1.5pxs_croped=67_usharp_r=4_mask=0.9_Simple_Segmentation_erode_disk2_close_disk2_poly.poly/50/trajs.csv';
fname = '/mnt/data3/yutong/masks/cell3/test/trajs.csv';
tab = csvread(fname);

idxs = tab(:,1);

figure
hold on
for i=idxs(1:50:end)'
    tr = tab(tab(:,1) == i, :);
    plot(tr(:,3), tr(:,4), 'Color', rand(1,3))
end
hold off
axis square
daspect([1 1 1])