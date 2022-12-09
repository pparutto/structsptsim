addpath('../external')

fname = '/tmp/test2/trajs.csv';
tab = csvread(fname);

figure
hold on
for i=tab(:,1)'
    tr = tab(tab(:,1) == i, :);
    plot(tr(:,3), tr(:,4), 'Color', rand(1,3))
end
hold off
axis square
daspect([1 1 1])