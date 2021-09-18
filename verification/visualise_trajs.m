addpath('/mnt/data/SPT_method/simu/fullcell/simus/regions/10');

%addpath('/mnt/data/SPT_method/simu/fullcell/simus/regions/1')
[bp, dp] = polys();

start_b = [];
if exist('start_box.m', 'file') == 2
    start_b = start_box();
end

stop_b = [];
if exist('stop_box.m', 'file') == 2
    stop_b = stop_box();
end

%tab = dlmread('/tmp/sim/trajs_empirical_DT=0.006000_lambdaNpts=0.090909.csv', ',');
%tab = dlmread('/mnt/data/SPT_method/simu/fullcell/simus/ER9_VERYNICE_MMStack_Pos0.ome_Simple Segmentation_cleaned_binary_poly.poly_0.05_15/trajs.csv');
%tab = dlmread('/mnt/data/SPT_method/simu/fullcell/simus/regions/1/trajs.csv', ',');
tab = dlmread('trajs.csv', ',');

figure
hold on
for i=1:length(bp)
    plot(bp{i}([1:size(bp{i}, 1) 1],1), bp{i}([1:size(bp{i}, 1) 1],2), 'k')
    for j=1:length(dp{i})
        plot(dp{i}{j}([1:size(dp{i}{j}, 1) 1], 1), dp{i}{j}([1:size(dp{i}{j}, 1) 1], 2), 'k')
    end
end

if ~isempty(start_b)
    plot([start_b(1,1), start_b(2,1)], [1 1] * start_b(1,2), 'r')
    plot([start_b(1,1), start_b(2,1)], [1 1] * start_b(2,2), 'r')
    plot([1 1] * start_b(1,1), [start_b(1,2), start_b(2,2)], 'r')
    plot([1 1] * start_b(2,1), [start_b(1,2), start_b(2,2)], 'r')
end
if ~isempty(stop_b)
    plot([stop_b(1,1), stop_b(2,1)], [1 1] * stop_b(1,2), 'm')
    plot([stop_b(1,1), stop_b(2,1)], [1 1] * stop_b(2,2), 'm')
    plot([1 1] * stop_b(1,1), [stop_b(1,2), stop_b(2,2)], 'm')
    plot([1 1] * stop_b(2,1), [stop_b(1,2), stop_b(2,2)], 'm')
end

Utils.show_trajectories(tab);
hold off
daspect([1 1 1])


frames = 0:4999;
cnts = zeros(length(frames), 1);
for i=1:length(frames)
    cnts(i) = sum(round(tab(:,2) ./ 0.006) == frames(i));
end

figure
plot(frames, cnts)

figure
hist(Utils.displacements(tab) / 0.006, 1:2:101)

figure
hold on
for k=unique(tab(:,1))'
    tr = tab(tab(:,1) == k, :);
    
    plot(tr(1,3), tr(1,4), 'xk')
    for l=1:(size(tr, 1)-1)
        if sqrt(sum((tr(l+1, 3:4) - tr(l, 3:4)).^2)) > 0.6
            plot(tr([l, l+1], 3), tr([l, l+1], 4))
        end
    end
end
hold off
daspect([1, 1, 1])