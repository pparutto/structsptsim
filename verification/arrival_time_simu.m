
tab = [];
M = 0;
for k=1:8
    ttab = csvread(sprintf('/mnt/data3/SPT_method/simu/fullcell/simus/arrival_times%d/trajs.csv', k));
    ttab(:,1) = ttab(:,1) + M;
    M = max(ttab(:,1)) + 1;
    tab = [tab; ttab];
end



tabf = csvread('/mnt/data3/SPT_method/simu/fullcell/simus/arrival_times_freespace/trajs.csv');

start_pt = [41.1 64.3];


d_euc = @(p1, p2) sqrt(sum((p1 - p2).^2, 2));
dr = 0.5;
rs = 0.5:dr:15.5;

arr_times = cell(length(rs), 1);
for k=unique(tab(:,1))'
    tr = tab(tab(:,1) == k, :);
    tr_d = d_euc(tr(:,3:4), start_pt);
    tr_r = floor(single((tr_d - rs(1)) / dr)) + 1;

    for l=1:length(rs)
        idx = min(find(tr_r == l));

        arr_times{l} = [arr_times{l}; tr(idx,2) - tr(1,2)];
    end
end

arr_times_free = cell(length(rs), 1);
for k=unique(tabf(:,1))'
    tr = tabf(tabf(:,1) == k, :);
    tr_d = d_euc(tr(:,3:4), start_pt);
    tr_r = floor(single((tr_d - rs(1)) / dr)) + 1;

    for l=1:length(rs)
        idx = min(find(tr_r == l));

        arr_times_free{l} = [arr_times_free{l}; tr(idx,2) - tr(1,2)];
    end
end

D = 1;

figure
hold on
plot(rs, cellfun(@(x) mean(x), arr_times), 'k');
plot(rs, cellfun(@(x) mean(x), arr_times_free), 'b');
plot(rs, 1 / 4 / D * rs.^2, 'r')
plot(rs, 1 / 2 / D * rs.^2, 'm')
hold off
xlim([0 10])
axis square
xlabel('Distance (µm)')
ylabel('Average arrival time (s)')
print('/tmp/arrival_time.svg', '-dsvg')