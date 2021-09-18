addpath('/tmp/a')


p = [1187.5 219];

pxs = 0.0967821;
[bp, dp] = polys();
%[bp, dp] = load_polys();

start_b = [];
if exist('start_box.m', 'file') == 2
    start_b = start_box();
end

stop_b = [];
if exist('stop_box.m', 'file') == 2
    stop_b = stop_box();
end

figure
hold on
%plot(bp(:,1), bp(:,2), 'k')
for i=1:length(bp)
    plot(bp{i}([1:size(bp{i}, 1) 1],1), bp{i}([1:size(bp{i}, 1) 1],2), 'k')
    text(mean(bp{i}(:,1)), mean(bp{i}(:,2)), sprintf('%d', i))
end
for i=1:length(dp)
    for j=1:length(dp{i})
        plot(dp{i}{j}([1:size(dp{i}{j}, 1) 1], 1), dp{i}{j}([1:size(dp{i}{j}, 1) 1], 2), ':', 'LineWidth', 2)
    end
end

plot(p(1) * [1 5], p(2) * [1 1])
plot([1304.1,1306], [220.9,219], 'm', 'LineWidth', 5)
plot([1306,1306], [219,217.4], 'm', 'LineWidth', 5)



if ~isempty(start_b)
    plot([start_b(1,1), start_b(2,1)], [1 1] * start_b(1,2), 'r')
    plot([start_b(1,1), start_b(2,1)], [1 1] * start_b(2,2), 'r')
    plot([1 1] * start_b(1,1), [start_b(1,2), start_b(2,2)], 'r')
    plot([1 1] * start_b(2,1), [start_b(1,2), start_b(2,2)], 'r')
end
if ~isempty(stop_b)
    plot([stop_b(1,1), stop_b(2,1)], [1 1] * stop_b(1,2), 'r')
    plot([stop_b(1,1), stop_b(2,1)], [1 1] * stop_b(2,2), 'r')
    plot([1 1] * stop_b(1,1), [stop_b(1,2), stop_b(2,2)], 'r')
    plot([1 1] * stop_b(2,1), [stop_b(1,2), stop_b(2,2)], 'r')
end
hold off

daspect([1 1 1])
set (gcf, 'WindowButtonMotionFcn', @mouseMove);