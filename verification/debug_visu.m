addpath('../external')

%addpath('/mnt/data/SPT_method/simu/fullcell/simus/regions/1');
addpath('/mnt/data/SPT_method/simu/fullcell/simus/regions/test');


%addpath('/tmp/toto')
%addpath('/mnt/data/SPT_method/simu/fullcell/simus/6ms_50pts_D=3')

%addpath('/mnt/data/SPT_method/simu/ER/simus/lumen/lumen_C2-_2019_10_02__16_23_00_Simple Segmentation_binary_1_D=6_DT=0.006_50pts_50000trajs')

%addpath('/mnt/data/Dropbox/Avezov_lab/Pierre/simu/mito')

%outdir = '/tmp/simu_fullcell';
outdir = '/tmp/joe';
if ~isfolder(outdir)
    mkdir(outdir)
end

bp = {};
dp = {};
if exist('polys.m') == 2
    [bp, dp] = polys();
end

start_b = [];
if exist('start_box.m', 'file') == 2
    start_b = start_box();
end

stop_b = [];
if exist('stop_box.m', 'file') == 2
    stop_b = stop_box();
end


figure%('Visible', 'off')
hold on
for i=1:length(bp)
    plot(bp{i}([1:size(bp{i}, 1) 1],1), bp{i}([1:size(bp{i}, 1) 1],2), 'k')

    for j=1:length(dp{i})
        plot(dp{i}{j}([1:size(dp{i}{j}, 1) 1], 1), dp{i}{j}([1:size(dp{i}{j}, 1) 1], 2), 'k')
    end
end



%plot([23.3753 23.34], [47.6183 47.6948], 'k')
%plot(23.3506, 47.6727, 'xr')
%plot([17.7383 17.7515], [45.5377 45.5534], 'r')


%plot([19.1076, 19.0873], [48.7332, 48.7135], 'r')
%plot([19.092 19.092], [48.98775 48.7104], 'm')

plot([19.1204, 19.0863], [48.6695 48.6734], 'r')



if ~isempty(start_b)
    plot([start_b(1,1), start_b(2,1)], [1 1] * start_b(1,2), 'r', 'LineWidth', 2)
    plot([start_b(1,1), start_b(2,1)], [1 1] * start_b(2,2), 'r', 'LineWidth', 2)
    plot([1 1] * start_b(1,1), [start_b(1,2), start_b(2,2)], 'r', 'LineWidth', 2)
    plot([1 1] * start_b(2,1), [start_b(1,2), start_b(2,2)], 'r', 'LineWidth', 2)
end
if ~isempty(stop_b)
    plot([stop_b(1,1), stop_b(2,1)], [1 1] * stop_b(1,2), 'm', 'LineWidth', 2)
    plot([stop_b(1,1), stop_b(2,1)], [1 1] * stop_b(2,2), 'm', 'LineWidth', 2)
    plot([1 1] * stop_b(1,1), [stop_b(1,2), stop_b(2,2)], 'm', 'LineWidth', 2)
    plot([1 1] * stop_b(2,1), [stop_b(1,2), stop_b(2,2)], 'm', 'LineWidth', 2)
end
hold off
daspect([1 1 1])
