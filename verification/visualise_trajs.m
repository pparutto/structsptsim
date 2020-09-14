addpath('/tmp');

[base_poly, polys] = load_polys();

tab = dlmread('/tmp/trajs.csv');

figure
hold on
Utils.show_trajectories(tab);
plot(base_poly(:, 1), base_poly(:,2), 'k')
for k=1:length(polys)
    plot(polys{k}(:,1), polys{k}(:,2), 'k') 
end
hold off
axis square
daspect([1 1 1])