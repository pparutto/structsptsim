addpath('/tmp/a')

[bp, dp] = polys();
start_b = [];
if exist('start_box.m', 'file') == 2
    start_b = start_box();
end

figure
hold on
for i=1:length(bp)
    plot(bp{i}([1:size(bp{i}, 1) 1],1), bp{i}([1:size(bp{i}, 1) 1],2), 'k')
    for j=1:length(dp{i})
        plot(dp{i}{j}([1:size(dp{i}{j}, 1) 1], 1), dp{i}{j}([1:size(dp{i}{j}, 1) 1], 2), 'r')
    end
end
if ~isempty(start_b)
    plot([start_b(1,1), start_b(2,1)], [1 1] * start_b(1,2), 'r', 'LineWidth', 2)
    plot([start_b(1,1), start_b(2,1)], [1 1] * start_b(2,2), 'r', 'LineWidth', 2)
    plot([1 1] * start_b(1,1), [start_b(1,2), start_b(2,2)], 'r', 'LineWidth', 2)
    plot([1 1] * start_b(2,1), [start_b(1,2), start_b(2,2)], 'r', 'LineWidth', 2)
end

hold off
axis square
daspect([1 1 1])
