addpath('../external')

fname = '/tmp/b/trajs.csv';
tab = csvread(fname);

idxs = tab(:,1);

figure
hold on
for i=idxs(1:50:end)'
    tr = tab(tab(:,1) == i, :);
    plot(tr(:,3), tr(:,4), 'Color', rand(1,3))
end
hold off
axis square
daspect([1 1 1])