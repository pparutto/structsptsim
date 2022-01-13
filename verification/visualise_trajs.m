addpath('../external')

addpath('/mnt/data/SPT_method/simu/fullcell/simus/regions/test2');
%addpath('/mnt/data/SPT_method/simu/fullcell/simus/regions/1');
addpath('/mnt/data/SPT_method/simu/fullcell/simus/regions/test');


%addpath('/tmp/toto')
%addpath('/mnt/data/SPT_method/simu/fullcell/simus/6ms_50pts_D=3')


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

tab = dlmread('trajs.csv', ',');


for k=1%unique(tab(:,1))'
    figure%('Visible', 'off')
    hold on
    for i=1:length(bp)
        plot(bp{i}([1:size(bp{i}, 1) 1],1), bp{i}([1:size(bp{i}, 1) 1],2), 'k')
%         for kk=1:(size(bp{i}, 1)-1)
%             p = bp{i}(kk,:) + (bp{i}(kk+1,:) - bp{i}(kk,:)) / 2;
%             v = bp{i}(kk+1,:) - bp{i}(kk,:);
%             n = sqrt(sum(v.^2));
%             norm =  [-v(2) / n, v(1) / n];
%             %plot(p(1), p(2), 'xr')
%             plot([p(1) p(1) + norm(1) * 0.005], [p(2) p(2) + norm(2) * 0.005], 'm')
%         end
        for j=1:length(dp{i})
            plot(dp{i}{j}([1:size(dp{i}{j}, 1) 1], 1), dp{i}{j}([1:size(dp{i}{j}, 1) 1], 2), 'k')
%             for kk=1:(size(dp{i}{j})-1)
%                 p = dp{i}{j}(kk,:) + (dp{i}{j}(kk+1,:) - dp{i}{j}(kk,:)) / 2;
%                 v = dp{i}{j}(kk+1,:) - dp{i}{j}(kk,:);
%                 n = sqrt(sum(v.^2));
%                 norm =  [-v(2) / n, v(1) / n];
%                 plot([p(1) p(1) + norm(1) * 0.1], [p(2) p(2) + norm(2) * 0.1], 'b')
%             end
        end
    end

%     tr = tab(tab(:,1) == k, :);
%     cm = jet(size(tr, 1) - 1);
%     for i=1:(size(tr,1)-1)
%         plot(tr([i i+1], 3), tr([i i+1], 4), 'Color', cm(i, :), 'LineWidth', 0.2)
%     end

    %plot([67.6094063, 67.6089001], [21.2413533 21.2256399], 'b', 'LineWidth', 2)
    %Utils.show_trajectories(tab(tab(:,1) == k, :));
    Utils.show_trajectories(tab);
    for u=unique(tab(:,1))'
        tr = tab(tab(:,1) == u, :);
        plot(tr(1,3), tr(1,4), 'xk')
    end
    %plot(20.3233146, 55.128996, 'xm')
    %plot(20.3661914 * [1 1], 55.1078195 * [1 0], 'r')
    
    %plot(20.3233063707433, 55.1290140731416, 'x')
    %plot(20.3233064, 55.1290141, 'o')

    
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
    axis([0 12 0 12])
    axis off
    print('/tmp/trajs.png', '-dpng')
    %set(gcf, 'WindowButtonMotionFcn', @mouseMove);

    %tightfig();
    %print(sprintf('%s/trajs_%d.png', outdir, k), '-dpng', '-r1000')
    %pause(5);
    %close all;
end
