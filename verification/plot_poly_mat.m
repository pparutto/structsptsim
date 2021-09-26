addpath('/tmp')


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


% plot([203 203], [128, 0], 'r')
% 
% plot([202.9,203.5],[5.3,2.7], 'b', 'LineWidth', 2)
% plot([208.1,202.9],[52,56.2], 'b', 'LineWidth', 2)
% plot([202.7,203.3],[82.5,78.6], 'b', 'LineWidth', 2)
% plot([203.9,202.9],[74.6,73.4], 'b', 'LineWidth', 2)
% plot([202.5,203.1],[70.1,68.1], 'b', 'LineWidth', 2)
% plot([203,203],[114.3,115.9], 'c', 'LineWidth', 2)
% plot([203,203.1],[115.9,117.5], 'm', 'LineWidth', 2)
% plot([203,203.1],[115.9,117.5], 'c', 'LineWidth', 2)

plot([90.2 90.2], [64, 0], 'r')

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
