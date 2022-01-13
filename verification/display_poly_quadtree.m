dat = csvread('/tmp/lol');

col_idxs = unique(dat(:,5));
cols = rand(length(col_idxs), 3);

figure
hold on
for k=unique(dat(:,5))'
    segs = dat(dat(:,5) == k, :);
    tmp = [];
    for l=1:size(segs, 1)
        tmp = [tmp; segs(l, 1:2); segs(l, 3:4); nan nan];
    end
    plot(tmp(:,1), tmp(:,2), 'Color', cols(find(col_idxs == k), :))
end
hold off
axis square