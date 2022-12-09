addpath('../../SPT_analyzer/src/analysis')

tab = csvread('/tmp/totati/trajs.csv');

idxs = unique(tab(:,1))';

figure
hold on
for i=idxs
    plot(tab(tab(:,1) == i, 3), tab(tab(:,1) == i, 4), 'Color', rand(1,3))
end
hold off
daspect([1 1 1])

ivels = Utils.inst_vels(tab);

figure
[o, b] = hist(ivels, 1:2:150);
plot(b, o / sum(o), 'k')
axis square
ylabel('Frequency')
xlabel('Instantaneous velocity (µm/s)')


grid_dx = 0.2;
min_D_grid_npts = 10;
g = Utils.gen_grid(tab, grid_dx);
diff = Utils.diffusion_map(tab, g, g, min_D_grid_npts);
diff = diff(diff > 0);

figure
[o, b] = hist(diff, 40);
stairs(b, o / sum(o), 'k')
axis square
ylabel('Frequency')
xlabel('Diffusion coefficient (µm²/s)')
display(sprintf('AVG diff %.3f ± %.3f n=%d', mean(diff), std(diff), length(diff)));