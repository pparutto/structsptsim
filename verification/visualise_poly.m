addpath('/mnt/data/SPT_method/simu/_2019_10_02__16_23_00_simu/data/6spots')

[base_polys, diff_polys] = polys();


figure
hold on
for k=1:length(base_polys)
    plot(base_polys{k}(:,1), base_polys{k}(:,2), 'k')
end
hold off
axis square
daspect([1 1 1])
