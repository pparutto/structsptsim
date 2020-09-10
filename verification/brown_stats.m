D = 0.05;
DT = 0.0001;
disps_bins = 0.0004:0.0008:0.02;

tab = dlmread('/tmp/trajs.csv');


sds = Utils.ensemble_MSD(tab);

%tab_sim = Utils.gen_brown_trajs(0.05, 0.0001, 20, 100);
%tab = tab_sim;

disps = [];
for i=unique(tab(:,1))'
    tr = tab(tab(:,1) == i, :);
    disps = [disps sqrt(sum((tr(2:end, 3:4) - tr(1:(end-1), 3:4)).^2, 2))'];
end
    
[o, b] = hist(disps, disps_bins);


[f, fci] = raylfit(disps);
Dest = f^2 / DT / 2;

%mypdf = @(bins, sd) raylpdf(bins, sd) / sum(raylpdf(b, sd));
%[f, gof] = fit(b', o' / sum(o), @(sd, x) mypdf(x, sd), 'Robust', 'LAR', 'Lower', 0', 'Upper', 5);
%Dest = f.sd.^2 / DT / 2;

display(sprintf('Estimated D=%g', Dest))

figure
hold on
bar(b, o / sum(o), 'hist')
plot(b, raylpdf(b, f) / sum(raylpdf(b, f)), 'r')
hold off
title(sprintf('Estimated D=%.4f', Dest), 'Interpreter', 'None')
axis square
xlabel('Displacement (µm)')
ylabel('Frequency')
%print(sprintf('/tmp/disp_distrib_%s.png', SNR), '-dpng')
