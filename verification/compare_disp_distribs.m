restoredefaultpath
addpath('../external/plot2svg')

pths = {'/mnt/data2/SPT_method/simu/freespace_fbm2/density/1/0.25_1_1_0.0001_60000';
        '/mnt/data2/SPT_method/simu/freespace_fbm2/density/1/0.75_1_1_0.0001_60000';
        '/mnt/data2/SPT_method/simu/freespace_fbm2/density/1/0.5_1_1_0.0001_60000';
        '/mnt/data2/SPT_method/simu/freespace/density3/sim/1/1_1_0.0001_60000'};

cols = {'g', 'r', 'b', 'k'};

%addpath('/mnt/data2/SPT_method/simu/freespace_fbm2/density/1/0.25_1_1_0.0001_60000')
%addpath('/mnt/data2/SPT_method/simu/freespace_fbm2/density/1/0.75_1_1_0.0001_60000')
%addpath('/mnt/data2/SPT_method/simu/freespace_fbm2/density/1/0.5_1_1_0.0001_60000')
%addpath('/mnt/data2/SPT_method/simu/freespace/density3/sim/1/1_1_0.0001_60000')

tabs = cell(length(pths), 1);
disps = cell(length(pths), 1);
for n=1:length(pths)
    addpath(pths{n});

    tab = dlmread('trajs.csv', ',');
    tabs{n} = tab;

    for k=unique(tab(:,1))'
        tr = tab(tab(:,1) == k, :);
        disps{n} = [disps{n}; sqrt(sum((tr(2:end, 3:4) - tr(1:(end-1), 3:4)).^2, 2))];
    end
end

bins = 0.0005:0.001:0.07;
figure
hold on
for k=1:length(disps)
    [o,b] = hist(disps{k}, bins);
    plot(bins, o / sum(o), 'LineWidth', 2, 'Color', cols{k})
end
hold off
axis square

for k=1:length(disps)
    figure
    hold on
    Utils.show_trajectories(tabs{k})
    hold off
    daspect([1 1 1])
    title(sprintf('%d', k))
end