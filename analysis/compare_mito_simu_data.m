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
        'Mito_Halo-N1-paJF646_2uMFCCP_15min_008_mask_38_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv'};

cats_cols = [253,174,97; 171,217,233; 44,123,182] ./ 255;
cats_names = {'Unt', 'Oligo', 'FCCP'};
cats_idxs = [ones(9, 1); 2 * ones(6, 1); 3 * ones(7, 1)];    
    
base_dat_dir = '/mnt/data/SPT_method/analysis';
dats = {'Mito_treat/C1-Mito Halo-N1-paJF646_001.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_002.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_003.czi'
        %'Mito_treat/C1-Mito Halo-N1-paJF646_004.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_005.czi'
        %'Mito_treat/C1-Mito Halo-N1-paJF646_006.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_007.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_008.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_009.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_10uMOligo_30min_001.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_10uMOligo_30min_002.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_10uMOligo_30min_003.czi'
        %'Mito_treat/C1-Mito Halo-N1-paJF646_10uMOligo_30min_004.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_10uMOligo_30min_005.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_10uMOligo_30min_006.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_2uMFCCP_15min_001.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_2uMFCCP_15min_002.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_2uMFCCP_15min_003.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_2uMFCCP_15min_004.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_2uMFCCP_15min_005.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_2uMFCCP_15min_007.czi'
        'Mito_treat/C1-Mito Halo-N1-paJF646_2uMFCCP_15min_008.czi'};


cats_dat_idxs = [ones(7, 1); 2 * ones(5, 1); 3 * ones(7, 1)];

grid_dx = 0.2;
min_D_grid_npts = 10;
    
ivels = cell(length(sims), 1);
ivels_pooled = cell(length(unique(cats_idxs)), 1);
diffs = cell(length(sims), 1);
diffs_pooled = cell(length(unique(cats_idxs)), 1);
for n=1:length(sims)
    tab = csvread(sprintf('%s/%s', base_dir, sims{n}));

    ivels{n} = Utils.inst_vels(tab);
    ivels_pooled{cats_idxs(n)} = [ivels_pooled{cats_idxs(n)}; ivels{n}];

    g = Utils.gen_grid(tab, grid_dx);
    diffs{n} = Utils.diffusion_map(tab, g, g, min_D_grid_npts);
    diffs{n} = diffs{n}(diffs{n} > 0)';
    diffs_pooled{cats_idxs(n)} = [diffs_pooled{cats_idxs(n)} diffs{n}];
end

i1 = 2;
i2 = 2;
i3 = 2;

ivels_dat = cell(length(sims), 1);
ivels_dat_pooled = cell(length(unique(cats_idxs)), 1);
diffs_dat = cell(length(sims), 1);
diffs_dat_pooled = cell(length(unique(cats_idxs)), 1);
for n=1:length(dats)    
    ana_t = load(sprintf('%s/%s/pipeline_1_trajectories.mat', base_dat_dir, dats{n}), 'tabs');
    ana_d = load(sprintf('%s/%s/pipeline_2_maps_and_diff.mat', base_dat_dir, dats{n}), 'diffs_maps');
    ana = load(sprintf('%s/%s/pipeline_3_ivels.mat', base_dat_dir, dats{n}));

    ivels_dat{n} = ana.ivels{i1}{i2}{i3};
    ivels_dat_pooled{cats_dat_idxs(n)} = [ivels_dat_pooled{cats_dat_idxs(n)}; ivels{n}];

    diffs_dat{n} = ana_d.diffs_maps{i1}{i2}{i3}(ana_d.diffs_maps{i1}{i2}{i3} > 0);
    diffs_dat_pooled{cats_dat_idxs(n)} = [diffs_dat_pooled{cats_dat_idxs(n)} diffs_dat{n}'];
end
 
figure
hold on
for n=1:length(sims)
    [o, b] = hist(ivels{n}, 1:2:150);
    plot(b, o / sum(o), 'Color', cats_cols(cats_idxs(n),:), 'LineWidth', 1)
end
for n=1:length(dats)
    [o, b] = hist(ivels_dat{n}, 1:2:150);
    plot(b, o / sum(o), '--', 'Color', cats_cols(cats_dat_idxs(n),:), 'LineWidth', 1)
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
for u=unique(cats_dat_idxs)'
    [o, b] = hist(ivels_dat_pooled{u}, 1:2:150);
    plot(b, o / sum(o), '--', 'Color', cats_cols(u,:), 'LineWidth', 1)
end
hold off
axis square
ylabel('Frequency')
xlabel('Pooled instantaneous velocity (µm/s)')
tightfig();
print('/tmp/mito_sims_dats_ivels_pooled.png', '-dpng')

figure
hold on
for n=1:length(sims)
    [o, b] = hist(diffs{n}, 40);
    plot(b, o / sum(o), 'Color', cats_cols(cats_idxs(n),:), 'LineWidth', 1)
end
for n=1:length(dats)
    [o, b] = hist(diffs_dat{n}, 40);
    plot(b, o / sum(o), '--', 'Color', cats_cols(cats_dat_idxs(n),:), 'LineWidth', 1)
end
hold off
axis square
ylabel('Frequency')
xlabel('Diffusion coefficient (µm²/s)')
tightfig();
print('/tmp/mito_sims_diff.png', '-dpng')
for n=1:length(sims)
    %display(sprintf('%d AVG diff %.3f ± %.3f n=%d', n, mean(diffs{n}), std(diffs{n}), length(diffs{n})));
    display(sprintf('%d AVG diff %.3f ± %.3f n=%d', n, mean(diffs_dat{n}), std(diffs_dat{n}), length(diffs_dat{n})));
end

figure
hold on
for u=unique(cats_idxs)'
    [o, b] = hist(diffs_pooled{u}, 40);
    plot(b, o / sum(o), 'Color', cats_cols(u,:), 'LineWidth', 1.5)
end
for u=unique(cats_dat_idxs)'
    [o, b] = hist(diffs_dat_pooled{u}, 40);
    plot(b, o / sum(o), '--', 'Color', cats_cols(u,:), 'LineWidth', 1.5)
end
hold off
axis square
ylabel('Frequency')
xlabel('Pooled diffusion coefficient (µm²/s)')
legend(cats_names)
tightfig();
print('/tmp/mito_sims_dats_diff_pooled.png', '-dpng')
for u=unique(cats_idxs)'
    %display(sprintf('POOLED %d AVG diff %.3f ± %.3f n=%d', u, mean(diffs_pooled{u}), std(diffs_pooled{u}), length(diffs_pooled{u})));
    display(sprintf('dat POOLED %d AVG diff %.3f ± %.3f n=%d', u, mean(diffs_dat_pooled{u}), std(diffs_dat_pooled{u}), length(diffs_dat_pooled{u})));
end