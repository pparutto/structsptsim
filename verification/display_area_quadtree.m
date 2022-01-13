dat = csvread('/tmp/lol2');

dat = [0, 0, 87.333, 81.399;
0, 40.6995, 43.6665, 81.399;
0, 40.6995, 21.83325, 61.04925;
10.916625, 40.6995, 21.83325, 50.874375;
16.3749375, 45.7869375, 21.83325, 50.874375];

figure
hold on

for k=1:size(dat,1)
    w = dat(k, 3) - dat(k, 1);
    h = dat(k, 4) - dat(k, 2);
    col = rand(1,3);
    plot(dat(k, 1) + [w/2 w/2], dat(k, 2) + [0 h], 'Color', col)
    plot(dat(k, 1) + [0 w], dat(k, 2) + [h/2 h/2], 'Color', col)
end

plot([19.1052, 19.0821], [47.5987 47.5797], 'r')
hold off
axis square