base_dir = '/mnt/data/SPT_method/tracking_opti/Mito/200727/Mito_Halo-N1-paJF646_001.tif/rad:0.75_th:2600.0_dist:0.6_distgap:0.6_framegap:0';
fname = 'tracks.csv';
delta_disp = 0.01;

tab = csvread(sprintf('%s/%s', base_dir, fname), 1, 0);
tab = tab(:, [1 5 3 4]);

disps = [];
for i=unique(tab(:,1))'
    tr = tab(tab(:,1) == i, :);
    disps = [disps; sqrt(sum((tr(2:end, 3:4) - tr(1:(end-1), 3:4)).^2, 2))];
end

[o,b] = hist(disps, (delta_disp / 2):delta_disp:max(disps));

%figure
%plot(b, cumsum(o) / sum(o))

xs = b - delta_disp / 2;
ys = cumsum(o) / sum(o);

fileID = fopen(sprintf('%s/disps_cdf_delta=%g.csv', base_dir, delta_disp), 'w');
for i=1:length(xs)
    fprintf(fileID, sprintf('%g, %g\n', xs(i), ys(i)));
end
fclose(fileID);