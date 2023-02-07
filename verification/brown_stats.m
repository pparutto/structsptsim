%disps_bins = 0.0004:0.0008:0.02;
disps_bins = 0.0025:0.005:2;

%tab = dlmread('/tmp/sim/trajs_D=1.500000_dt=0.000100_DT=0.006000_lambdaNpts=0.050000_nframes=0_width=128_height=128_dens=0.000123.csv');
tab = dlmread('/mnt/data/SPT_method/simu/freespace/density/1_25_0.006_2000/trajs.csv');
%tab = dlmread('/mnt/data/SPT_method/simu/very_low/trajs.csv');
DT = Utils.find_dt(tab);

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
%plot(b, raylpdf(b, sqrt(2 * 20 * 0.004)) / sum(raylpdf(b, sqrt(2 * 20 * 0.004))), 'm--')
plot(b, raylpdf(b, sqrt(2 * Dest * DT)) / sum(raylpdf(b, sqrt(2 * Dest * DT))), 'm--')
hold off
title(sprintf('Estimated D=%.4f', Dest), 'Interpreter', 'None')
axis square
xlabel('Displacement (µm)')
ylabel('Frequency')
%print(sprintf('/tmp/disp_distrib_%s.png', SNR), '-dpng')

% [o, b] = hist(disps ./ DT, 1:2:100);
% figure
% bar(b, o / sum(o), 'hist')
% axis square

% D = 3;
% DTs = [0.002 0.004 0.006 0.008 0.01 0.012 0.014];
% 
% figure
% hold on
% for i=1:length(DTs)
%    plot(0.01:0.02:1.5, raylpdf(0.01:0.02:1.5, sqrt(2*D * DTs(i))))
% end
% hold off