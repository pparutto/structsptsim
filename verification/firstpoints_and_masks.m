pts = csvread('/mnt/data/SPT_method/analysis_opti/Mito/201217/Mito Halo-N1-paJF646_001.czi/traj_characs_th=2400_dist=0.6_minLen=0.csv');
%pts = csvread('/mnt/data/SPT_method/analysis_opti/varying_dens/210418_SPTdata/210209_COS7_mEM-M_Halo-ER-646_day1_7/Nsplit=1/traj_characs_th=1600_dist=1_minLen=5.csv');
tab = csvread('/tmp/sim/trajs.csv');

addpath('/tmp/sim');

[bp, dp] = polys();

figure
hold on
plot(pts(:,1), pts(:,2), 'xk')
for k=1:length(bp)
    plot(bp{k}(:,1), bp{k}(:,2), 'k')
end
for k=1:length(dp)
    for l=1:length(dp{k})
        plot(dp{k}{l}(:,1), dp{k}{l}(:,2), 'r')
    end
end
hold off
daspect([1 1 1])

figure
hold on
plot(pts(:,1), pts(:,2), 'xk')
for k=unique(tab(:,1))'
    tr = tab(tab(:,1) == k, :);
    plot(tr(1,3), tr(1,4), 'or')
end
for k=1:length(bp)
    plot(bp{k}(:,1), bp{k}(:,2), 'k')
end
for k=1:length(dp)
    for l=1:length(dp{k})
        plot(dp{k}{l}(:,1), dp{k}{l}(:,2), 'r')
    end
end
hold off
daspect([1 1 1])

figure
hold on
Utils.show_trajectories(tab)
for k=1:length(bp)
    plot(bp{k}(:,1), bp{k}(:,2), 'k')
end
for k=1:length(dp)
    for l=1:length(dp{k})
        plot(dp{k}{l}(:,1), dp{k}{l}(:,2), 'r')
    end
end
hold off
daspect([1 1 1])