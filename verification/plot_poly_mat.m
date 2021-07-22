%addpath('/tmp/a')
addpath('/tmp/b')
%addpath('/tmp/c')
%addpath('/tmp/d')
%addpath('/mnt/data/SPT_method/simu/fullcell/simus/ER9_VERYNICE_MMStack_Pos0.ome_Simple Segmentation_cleaned_binary_poly.poly_0.05_10')

pxs = 0.0967821;
[bp, dp] = polys();
%[bp, dp] = load_polys();


figure
hold on
%plot(bp(:,1), bp(:,2), 'k')
for i=1:length(bp)
    plot(bp{i}([1:size(bp{i}, 1) 1],1), bp{i}([1:size(bp{i}, 1) 1],2), 'k')
    text(mean(bp{i}(:,1)), mean(bp{i}(:,2)), sprintf('%d', i))
end
plot(1194, 207.3, 'or')
plot(1194 * [1 1], [207.3 1300], 'r')

% plot(102, 95, 'or')
% plot(102 * [1 1], [95 150], 'r')
%plot(152.1, 61, 'or')
%plot(152.1 * [1 1], [61 350], 'r')

%plot(189, 183.8, 'or')
%plot(189 * [1 1], [183.8 350], 'r')
%plot(203.6, 183.2, 'or')
%plot(203.6 * [1 1], [183.2 350], 'r')
%plot(1155, 545.3, 'or')
%plot(1155 * [1 1], [545.3 1300], 'r')

% plot(1155, 674.999999999999, '*m')
% plot(1152.8, 674.599999999999, 'ob')
%plot(90.60437, 41.0, 'or')
%plot(90.60437 * [1 1], [41.0 110], 'r')

%plot(141.38558, 41.0, 'or')
%plot(141.38558 * [1 1], [41.0 110], 'r')
%plot(115.5578, 20.0629, 'or')
%plot(115.5578 * [1 1], [20.0629 145], 'r')
%plot(1155, 545.3, 'or')
%plot(1155 * [1 1], [545.3, 1400], 'r')
for i=1:length(dp)
    for j=1:length(dp{i})
        plot(dp{i}{j}([1:size(dp{i}{j}, 1) 1], 1), dp{i}{j}([1:size(dp{i}{j}, 1) 1], 2), ':', 'LineWidth', 2)
    end
end
hold off
daspect([1 1 1])