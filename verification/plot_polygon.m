dat = csvread('../resources/U.csv');
%dat = csvread('/tmp/a.csv');

p = dat(1, :);
figure
hold on
for k=1:(size(dat, 1)-1)
    pp = p + dat(k+1, :);
    plot([p(1), pp(1)], [p(2) pp(2)], 'k')
    p = pp;
end
plot([p(1) dat(1, 1)], [p(2) dat(1, 2)], 'k')
hold off
axis square
daspect([1 1 1])


dat = [
116.984 93.7433;
115.518 117.399;
102.054 129.746;
50.6049 129.478;
50.4045 120.423;
100.731 120.958;
107.592 112.292;
108.323 96.5429;
96.8922 86.4158;
51.965 86.8721;
53.1677 78.5918;
99.3132 78.0573;
116.984 93.7433;
];
figure
hold on
plot(dat(:,1), dat(:,2), 'k')
hold off