addpath('../../SPT_analyzer/src/analysis')
addpath('../../SPT_analyzer/src/external/tightfig')

base_dir = '/mnt/data/SPT_method/simu/mito_treat/simus/';

% sims = {'Mito_Halo-N1-paJF646_001/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_002/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_003/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_004/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_005/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_006/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv'};



sims = {'Mito_Halo-N1-paJF646_001_mask_11075_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_002_mask_13069_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_003_mask_45_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_004_mask_12330_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_005_mask_6982_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_006_mask_11009_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_007_mask_7791_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_008_mask_13_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_009_mask_1_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_10uMOligo_30min_001_mask_10305_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_10uMOligo_30min_002_mask_5016_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_10uMOligo_30min_003_mask_10797_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_10uMOligo_30min_004_mask_6374_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_10uMOligo_30min_005_mask_13147_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_10uMOligo_30min_006_mask_9_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_2uMFCCP_15min_001_mask_5086_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_2uMFCCP_15min_002_mask_65_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_2uMFCCP_15min_003_mask_14060_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_2uMFCCP_15min_004_mask_8950_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_2uMFCCP_15min_005_mask_7040_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_2uMFCCP_15min_007_mask_10454_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
        'Mito_Halo-N1-paJF646_2uMFCCP_15min_008_mask_38_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';};

cats_cols = [253,174,97; 171,217,233; 44,123,182] ./ 255;
cats_names = {'Unt', 'Oligo', 'FCCP'};
cats_idxs = [ones(9, 1); 2 * ones(6, 1); 3 * ones(7, 1)];

grid_dx = 0.2;
min_D_grid_npts = 10;
    
ivels = cell(length(sims), 1);
ivels_pooled = cell(length(unique(cats_idxs)), 1);
diffs = cell(length(sims), 1);
diffs_pooled = cell(length(unique(cats_idxs)), 1);
for n=1:length(sims)
    tab = csvread(sprintf('%s/%s', base_dir, sims{n}));

    figure
    hold on
    for i=unique(tab(:,1))'
        plot(tab(tab(:,1) == i, 3), tab(tab(:,1) == i, 4), 'Color', rand(1,3))
    end
    hold off
    daspect([1 1 1])
    tightfig();
    print(sprintf('/tmp/trajs_sim_%d.png', n), '-dpng')

    ivels{n} = Utils.inst_vels(tab);
    ivels_pooled{cats_idxs(n)} = [ivels_pooled{cats_idxs(n)}; ivels{n}];

    g = Utils.gen_grid(tab, grid_dx);
    diffs{n} = Utils.diffusion_map(tab, g, g, min_D_grid_npts);
    diffs{n} = diffs{n}(diffs{n} > 0)';
    diffs_pooled{cats_idxs(n)} = [diffs_pooled{cats_idxs(n)} diffs{n}];
end
 
figure
hold on
for n=1:length(sims)
    [o, b] = hist(ivels{n}, 1:2:150);
    plot(b, o / sum(o), 'Color', cats_cols(cats_idxs(n),:), 'LineWidth', 1)
end
hold off
axis square
ylabel('Frequency')
xlabel('Instantaneous velocity (µm/s)')
tightfig();
print('/tmp/mito_sims_ivels.png', '-dpng')

figure
hold on
for u=unique(cats_idxs)'
    [o, b] = hist(ivels_pooled{u}, 1:2:150);
    plot(b, o / sum(o), 'Color', cats_cols(u,:), 'LineWidth', 1)
end
hold off
axis square
ylabel('Frequency')
xlabel('Pooled instantaneous velocity (µm/s)')
tightfig();
print('/tmp/mito_sims_ivels_pooled.png', '-dpng')

figure
hold on
for n=1:length(sims)
    [o, b] = hist(diffs{n}, 40);
    plot(b, o / sum(o), 'Color', cats_cols(cats_idxs(n),:), 'LineWidth', 1)
end
hold off
axis square
ylabel('Frequency')
xlabel('Diffusion coefficient (µm²/s)')
tightfig();
print('/tmp/mito_sims_diff.png', '-dpng')
for n=1:length(sims)
    display(sprintf('%d AVG diff %.3f ± %.3f n=%d', n, mean(diffs{n}), std(diffs{n}), length(diffs{n})));
end

figure
hold on
for u=unique(cats_idxs)'
    [o, b] = hist(diffs_pooled{u}, 40);
    plot(b, o / sum(o), 'Color', cats_cols(u,:), 'LineWidth', 1.5)
end
hold off
axis square
ylabel('Frequency')
xlabel('Pooled diffusion coefficient (µm²/s)')
legend(cats_names)
tightfig();
print('/tmp/mito_sims_diff_pooled.png', '-dpng')
for u=unique(cats_idxs)'
    display(sprintf('POOLED %d AVG diff %.3f ± %.3f n=%d', u, mean(diffs_pooled{u}), std(diffs_pooled{u}), length(diffs_pooled{u})));
end