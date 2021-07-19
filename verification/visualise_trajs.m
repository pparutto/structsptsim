%addpath('/tmp');

%[base_poly, polys] = load_polys();

tab = dlmread('/tmp/sim/trajs_empirical_DT=0.006000_lambdaNpts=0.090909.csv', ',');

figure
hold on
Utils.show_trajectories(tab);
daspect([1 1 1])


frames = 0:4999;
cnts = zeros(length(frames), 1);
for i=1:length(frames)
    cnts(i) = sum(round(tab(:,2) ./ 0.006) == frames(i));
end

figure
plot(frames, cnts)

figure
hist(Utils.displacements(tab) / 0.006, 1:2:101)

figure
hold on
for k=unique(tab(:,1))'
    tr = tab(tab(:,1) == k, :);
    
    plot(tr(1,3), tr(1,4), 'xk')
    for l=1:(size(tr, 1)-1)
        if sqrt(sum((tr(l+1, 3:4) - tr(l, 3:4)).^2)) > 0.6
            plot(tr([l, l+1], 3), tr([l, l+1], 4))
        end
    end
end
hold off
daspect([1, 1, 1])