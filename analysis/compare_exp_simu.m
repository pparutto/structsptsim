addpath('/mnt/data2/ER_net simu/190418_COS7_Halo-KDEL_YFP-Z_day1_JF646PC_SPT_9')
addpath('/home/pierre/cam/SPT/SPT_analyzer/src/external/plot2svg');
exp_tab = Utils.load_trackmate('/mnt/data2/ER_net simu/190418_COS7_Halo-KDEL_YFP-Z_day1_JF646PC_SPT_9/experimental_trajs/preproc_subbckg_paraboloid_50pxs_enhcont_.3_normalise_spot_rad=0.75_th=350_traj_dist=0.6_gap=0/Spots in tracks statistics.csv');
sim_tab = csvread('/mnt/data2/ER_net simu/190418_COS7_Halo-KDEL_YFP-Z_day1_JF646PC_SPT_9/trajs_D=9.000000_dt=0.000100_DT=0.004000_maxNpts=5_maxNtrajs=500.csv');
[base_poly, polys] = load_polys();

figure
subplot(1,2,1)
hold on
for i=unique(exp_tab(:,1))'
    plot(exp_tab(exp_tab(:,1) == i, 3), exp_tab(exp_tab(:,1) == i, 4), 'Color', rand(1, 3))
end
plot(base_poly(:,1), base_poly(:,2), 'b')
for i=1:length(polys)
   plot(polys{i}(:,1), polys{i}(:,2), 'b') 
end
hold off
axis square
daspect([1 1 1])
set(gca, 'YDir', 'Reverse')
title(sprintf('Experiment: num. trajs=%d, avg size=%g', length(unique(exp_tab(:,1))), mean(arrayfun(@(i) sum(exp_tab(:,1) == i), unique(exp_tab(:,1))))));

subplot(1,2,2)
hold on
for i=unique(sim_tab(:,1))'
    plot(sim_tab(sim_tab(:,1) == i, 3), sim_tab(sim_tab(:,1) == i, 4), 'Color', rand(1, 3))
end
plot(base_poly(:,1), base_poly(:,2), 'b')
for i=1:length(polys)
   plot(polys{i}(:,1), polys{i}(:,2), 'b') 
end
hold off
axis square
daspect([1 1 1])
set(gca, 'YDir', 'Reverse')
title(sprintf('Simulation: num. trajs=%d, avg size=%g', length(unique(sim_tab(:,1))), mean(arrayfun(@(i) sum(sim_tab(:,1) == i), unique(sim_tab(:,1))))));
plot2svg('/tmp/toto.svg')

ivels_dat = Utils.inst_vels(exp_tab);
ivels_sim = Utils.inst_vels(sim_tab);

bins = 2.5:5:max([max(ivels_dat), max(ivels_sim)]);

figure
hold on
[o,b] = hist(ivels_dat, bins);
plot(b, o / sum(o), 'k')
[o,b] = hist(ivels_sim, bins);
plot(b, o / sum(o), 'r')
plot(b, raylpdf(b, sqrt(2 * 9 / 0.004)) / sum(raylpdf(b, sqrt(2 * 9 / 0.004))), 'm')
hold off
legend({'Data', 'Simulation', 'Purely diffusive'})
axis square
xlabel('Instantaneous velocity (µm/s)')
ylabel('Frequency')
print('/tmp/toto2.svg', '-dsvg')