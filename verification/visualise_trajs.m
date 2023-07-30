restoredefaultpath
addpath('../external/plot2svg')

%addpath('/mnt/data3/SPT_method/simu/fullcell/simus/arrival_times_freespace')
%addpath('/mnt/data3/SPT_method/simu/fullcell/simus/arrival_times_pts=15000')
%addpath('/mnt/data3/SPT_method/simu/211217_COS7_4MTS-Halo-PA646_4MTS-mNG_Ionomycin15min-10uM_4/sim')

%addpath('/mnt/data/SPT_method/simu/fullcell/simus/regions/1');
%addpath('/mnt/data/SPT_method/simu/fullcell/simus/regions/test');


%addpath('/tmp/b')
%addpath('/mnt/data/SPT_method/simu/fullcell/simus/6ms_50pts_D=3')


%addpath('/mnt/data/Dropbox/Avezov_lab/Pierre/simu/mito')

%addpath('/mnt/data3/yutong/simu/simu_cell2_230623_100ms_60killRate')


%addpath('/mnt/data/SPT_method/simu/mito/211217/C2-211217_COS7_4MTS-Halo-P  _4MTS-Halo-PA646_4MTS-mNG_FCCP15min-10uM_10.czi.tif_avg17.tif_musical_NImgs=11_NFrames=10_th=4.0_alpha=6.0_gauss=1.5pxs_croped=67_usharp_r=4_mask=0.9_Simple_Segmentation_erode_disk2_close_disk2_poly.poly/1_30_0.006_2000');

%addpath('/mnt/data2/SPT_method/simu/ER/debug_struct/simus/D=5_DT=0.01_1spot')

%addpath('/mnt/data3/yutong/simu/simu_cell2_230623_100ms_60killRate/simu_cell2_230623_100ms_60killRate_1')

addpath('/mnt/data2/SPT_method/simu/ER/density2/C2-Sec61b_Halo-paJF646+400uMBSA_001.czi.tif_avg17.tif_musical_4_6_50frames_croped46pxs_avg_norm_gauss1.5px_Simple_Segmentation_binary_close1px_erode1px_erode1px_poly.poly/1/1_15_0.01_4000')


outdir = '/tmp';
%outdir = '/tmp/simu_fullcell';
%outdir = '/mnt/data3/yutong/masks/cell3/test';
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
    stop_b = stop_box();end

% for k=11%:length(bp)
%     figure
%     hold on
%     plot(bp{k}(:,1), bp{k}(:,2), 'k')
%     for l=1:length(dp{k})
%         plot(dp{k}{l}(:,1), dp{k}{l}(:,2), 'r')
%     end
%     plot(3.3792228084523, 1.98157779367967, 'xr')
%     hold off
%     title(sprintf('%d', k))
%     axis square
% end

tab = dlmread('trajs.csv', ',');
%tab = zeros(0,5);

figure%('Visible', 'off')
hold on
for i=1:length(bp)
    plot(bp{i}([1:size(bp{i}, 1) 1],1), bp{i}([1:size(bp{i}, 1) 1],2), 'k', 'LineWidth', 2)

    %%show normals
    for kk=1:(size(bp{i}, 1)-1)
        p = bp{i}(kk,:) + (bp{i}(kk+1,:) - bp{i}(kk,:)) / 2;
        v = bp{i}(kk+1,:) - bp{i}(kk,:);
        n = sqrt(sum(v.^2));
        norm =  [-v(2) / n, v(1) / n];
        %plot(p(1), p(2), 'xr')
        plot([p(1) p(1) + norm(1) * 0.005], [p(2) p(2) + norm(2) * 0.005], 'm')
    end
    for j=1:length(dp{i})
        plot(dp{i}{j}([1:size(dp{i}{j}, 1) 1], 1), dp{i}{j}([1:size(dp{i}{j}, 1) 1], 2), 'r', 'LineWidth', 2)
        %%show normals
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
%Utils.show_trajectories(tab);

idxs = unique(tab(:,1));


% for k=unique(tab(:,1))'
%     tr = tab(tab(:,1) == k, :);
%     plot(tr(1,3), tr(1,4), 'xk')
%     plot(tr(2:(end-1), 3), tr(2:(end-1), 4), 'k*')
%     plot(tr(end,3), tr(end,4), 'ok')
%     if inpolygon(tr(1,3), tr(1,4), dp{1}{1}(:,1), dp{1}{1}(:,2))
%         display(sprintf('({%g, %g});', tr(1,3), tr(1,4)))
%     end
% end

% plot([108.229, 108.242] - 100, [104.666 , 104.683] - 100, 'r')
% plot([108.235, 108.246] - 100, [104.674 , 104.677] - 100, 'r')
% 
% plot(108.235 - 100, 104.674 - 100, 'xr')
% plot([108.236, 108.125] - 100, [104.675 104.6] - 100, 'm')

%plot(0.568540541483617, 1.28632113263532, 'xr')
%plot([0.825973, 0.827797], [1.72776 , 1.71335], 'r')


% for k=unique(tab(:,1))'
%     tr = tab(tab(:,1) == k, :);
%     in = 1;
%     for l=1:length(bp)
%         s = sum(~inpolygon(tr(:,3), tr(:,4), bp{l}(:,1), bp{l}(:,2)));
%         if s > 0 && s < size(tr,1)
%             tr(end,:)
%             plot(tr(:,3), tr(:,4), 'Color', rand(1,3))
%         end
%     end
% end

for k=idxs'%(1:10:length(idxs))
    plot(tab(tab(:,1) == k,3), tab(tab(:,1) == k,4), 'Color', rand(1,3));
end
%axis([0 2204*0.01625 0 1902*0.01625])

%plot(20.3233146, 55.128996, 'xm')
%plot(20.3661914 * [1 1], 55.1078195 * [1 0], 'r')

%plot(20.3233063707433, 55.1290140731416, 'x')
%plot(20.3233064, 55.1290141, 'o')

% plot(41.1, 64.3, 'xk')b 
% dths = 0.025:0.05:(2*pi+0.025);
% r = 15;
% plot(41.1 + r * cos(dths), 64.3 + r * sin(dths), 'r')

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
%set(gca, 'YDir','reverse')
%plot2svg('/tmp/trajs_struct.svg')
%)axis([0 12 0 12])
%axis off
%print('/tmp/trajs.png', '-dpng')
%set(gcf, 'WindowButtonMotionFcn', @mouseMove);

%tightfig();
%print(sprintf('%s/trajs_%d.png', outdir, k), '-dpng', '-r1000')
%pause(5);


disps = [];
for k=unique(tab(:,1))'
    tr = tab(tab(:,1) == k, :);
    disps = [disps; sqrt(sum((tr(2:end, 3:4) - tr(1:(end-1), 3:4)).^2, 2))];
end
figure
plot(zeros(length(disps), 1), disps, 'x')