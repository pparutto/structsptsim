addpath('/tmp/a')

pxs = 0.0967821;
[bp, dp] = polys();


figure
hold on
for i=1:length(bp)
    plot(bp{i}([1:size(bp{i}, 1) 1],1), bp{i}([1:size(bp{i}, 1) 1],2), 'k')
    text(mean(bp{i}(:,1)), mean(bp{i}(:,2)), sprintf('%d', i))
end
for i=1:length(dp)
    for j=1:length(dp{i})
        plot(dp{i}{j}([1:size(dp{i}{j}, 1) 1], 1), dp{i}{j}([1:size(dp{i}{j}, 1) 1], 2), ':', 'LineWidth', 2)
    end
end


plot(1.18408 * [1 1], [5.41125 350], 'r')

plot([66.500000,66.500000], [155.000000,151.500000], 'LineWidth', 2, 'Color', 'm');
plot([65.900000,68.100000], [211.300000,215.800000], 'LineWidth', 2, 'Color', 'r');



hold off