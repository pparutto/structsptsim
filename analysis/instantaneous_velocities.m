addpath('/mnt/data2/ER_net simu/190418_COS7_Halo-KDEL_YFP-Z_day1_JF646PC_SPT_9')
[base_poly, polys] = load_polys();

base_dir = '/mnt/data2/ER_net simu/190418_COS7_Halo-KDEL_YFP-Z_day1_JF646PC_SPT_9/varying_D';
exps = {'trajs_D=1.000000_dt=0.000100_DT=0.004000_npts=10_ntrajs=500.csv';
        'trajs_D=2.000000_dt=0.000100_DT=0.004000_npts=10_ntrajs=500.csv';
        'trajs_D=3.000000_dt=0.000100_DT=0.004000_npts=10_ntrajs=500.csv';
        'trajs_D=4.000000_dt=0.000100_DT=0.004000_npts=10_ntrajs=500.csv';
        'trajs_D=5.000000_dt=0.000100_DT=0.004000_npts=10_ntrajs=500.csv';
        'trajs_D=6.000000_dt=0.000100_DT=0.004000_npts=10_ntrajs=500.csv';
        'trajs_D=7.000000_dt=0.000100_DT=0.004000_npts=10_ntrajs=500.csv';
        'trajs_D=8.000000_dt=0.000100_DT=0.004000_npts=10_ntrajs=500.csv';
        'trajs_D=9.000000_dt=0.000100_DT=0.004000_npts=10_ntrajs=500.csv'};
    
exps = {'trajs_D=1.000000_dt=0.000100_DT=0.004000_lambdaNpts=10.000000_ntrajs=500.csv';
        'trajs_D=2.000000_dt=0.000100_DT=0.004000_lambdaNpts=10.000000_ntrajs=500.csv';
        'trajs_D=3.000000_dt=0.000100_DT=0.004000_lambdaNpts=10.000000_ntrajs=500.csv';
        'trajs_D=4.000000_dt=0.000100_DT=0.004000_lambdaNpts=10.000000_ntrajs=500.csv';
        'trajs_D=5.000000_dt=0.000100_DT=0.004000_lambdaNpts=10.000000_ntrajs=500.csv';
        'trajs_D=6.000000_dt=0.000100_DT=0.004000_lambdaNpts=10.000000_ntrajs=500.csv';
        'trajs_D=7.000000_dt=0.000100_DT=0.004000_lambdaNpts=10.000000_ntrajs=500.csv';
        'trajs_D=8.000000_dt=0.000100_DT=0.004000_lambdaNpts=10.000000_ntrajs=500.csv';
        'trajs_D=9.000000_dt=0.000100_DT=0.004000_lambdaNpts=10.000000_ntrajs=500.csv'};
    
%base_dir = '/mnt/data2/ER_net simu/190418_COS7_Halo-KDEL_YFP-Z_day1_JF646PC_SPT_9/mem_vs_lum';
%exps = {'trajs_D=1.000000_dt=0.000100_DT=0.004000_npts=20_ntrajs=5000.csv';
%        'trajs_D=9.000000_dt=0.000100_DT=0.004000_npts=20_ntrajs=5000.csv'};


tabs = cell(length(exps), 1);
ivels = cell(length(exps), 1);
ivels_tr = cell(length(exps), 1);
for k=1:length(exps)
    tabs{k} = csvread(sprintf('%s/%s', base_dir, exps{k}));
    ivels{k} = Utils.inst_vels(tabs{k});
    ivels_tr{k} = Utils.inst_vels_along_traj(tabs{k});
end
cm = jet(length(tabs));

u = 1;
tab = tabs{u};

figure
subplot(1,2,1)
hold on
for i=unique(tab(:,1))'
    plot(tab(tab(:,1) == i, 3), tab(tab(:,1) == i, 4), 'Color', cm(u,:))
end
plot(base_poly(:,1), base_poly(:,2), 'k')
for i=1:length(polys)
   plot(polys{i}(:,1), polys{i}(:,2), 'k') 
end
hold off
axis square
daspect([1 1 1])
set(gca, 'YDir', 'Reverse')
print(sprintf('/tmp/trajs_u=%d.svg', u), '-dsvg')

bins = 4:8:220;%280;

figure
hold on
for k=[1 5 9]%1:length(exps)
    [o,b] = hist(ivels{k}, bins);
    plot(b, o / sum(o), 'Color', cm(k,:))
end
hold off
axis square
xlabel('Instantaneous velocity (µm/s)')
print('/tmp/ivels_simu.svg', '-dsvg')


% bins = 2:4:130;
% 
% figure
% hold on
% for k=1:length(exps)
%     [o,b] = hist(cellfun(@(x) mean(x), ivels_tr{k}), bins);
%     plot(b, o / sum(o), 'Color', cm(k,:))
% end
% hold off
% axis square
% 
% bins = 4:8:280;
% 
% figure
% hold on
% for k=1:length(exps)
%     [o,b] = hist(cellfun(@(x) max(x), ivels_tr{k}), bins);
%     plot(b, o / sum(o), 'Color', cm(k,:))
% end
% hold off
% axis square