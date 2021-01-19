addpath('../../SPT_analyzer/src/analysis')
addpath('../../SPT_analyzer/src/external/tightfig')

%base_dir = '/mnt/data/SPT_method/simu/mito_treat/simus/';
base_dir = '/mnt/data/SPT_method/simu';

% sims = {'Mito_Halo-N1-paJF646_001/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_002/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_003/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_004/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_005/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_006/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv'};


% 
% sims = {'Mito_Halo-N1-paJF646_001_mask_11075_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_002_mask_13069_poly.path/trajs_D=1.600000_dt=0.000100_)DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_003_mask_45_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_004_mask_12330_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_005_mask_6982_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_006_mask_11009_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_007_mask_7791_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_008_mask_13_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_009_mask_1_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_10uMOligo_30min_001_mask_10305_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_10uMOligo_30min_002_mask_5016_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_10uMOligo_30min_003_mask_10797_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_10uMOligo_30min_004_mask_6374_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_10uMOligo_30min_005_mask_13147_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_10uMOligo_30min_006_mask_9_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_2uMFCCP_15min_001_mask_5086_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_2uMFCCP_15min_002_mask_65_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_2uMFCCP_15min_003_mask_14060_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_2uMFCCP_15min_004_mask_8950_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_2uMFCCP_15min_005_mask_7040_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_2uMFCCP_15min_007_mask_10454_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_2uMFCCP_15min_008_mask_38_poly.path/trajs_D=1.600000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';};

%cats_cols = [253,174,97; 171,217,233; 44,123,182] ./ 255;
%cats_names = {'Unt', 'Oligo', 'FCCP'};
%cats_idxs = [ones(9, 1); 2 * ones(6, 1); 3 * ones(7, 1)];

% sims = {'Mito_Halo-N1-paJF646_001_mask_11075_poly.path/trajs_D=1.500000_dt=0.000100_DT=%s000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_002_mask_13069_poly.path/trajs_D=1.500000_dt=0.000100_DT=%s000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_003_mask_45_poly.path/trajs_D=1.500000_dt=0.000100_DT=%s000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_004_mask_12330_poly.path/trajs_D=1.500000_dt=0.000100_DT=%s000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_005_mask_6982_poly.path/trajs_D=1.500000_dt=0.000100_DT=%s000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_006_mask_11009_poly.path/trajs_D=1.500000_dt=0.000100_DT=%s000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_007_mask_7791_poly.path/trajs_D=1.500000_dt=0.000100_DT=%s000_npts=20_ntrajs=5000.csv'};
% 
% free_sims = {'free_space/trajs_width=128_height=128_pxsize=0.967821_D=1.5_dt=0.0001_DT=0.002_lambda=20_dens=0.0015.csv';
%              'free_space/trajs_width=128_height=128_pxsize=0.967821_D=1.5_dt=0.0001_DT=0.004_lambda=20_dens=0.0015.csv';
%              'free_space/trajs_width=128_height=128_pxsize=0.967821_D=1.5_dt=0.0001_DT=0.006_lambda=20_dens=0.0015.csv';
%              'free_space/trajs_width=128_height=128_pxsize=0.967821_D=1.5_dt=0.0001_DT=0.008_lambda=20_dens=0.0015.csv';
%              'free_space/trajs_width=128_height=128_pxsize=0.967821_D=1.5_dt=0.0001_DT=0.01_lambda=20_dens=0.0015.csv';
%              'free_space/trajs_width=128_height=128_pxsize=0.967821_D=1.5_dt=0.0001_DT=0.012_lambda=20_dens=0.0015.csv'
%              'free_space/trajs_width=128_height=128_pxsize=0.967821_D=1.5_dt=0.0001_DT=0.014_lambda=20_dens=0.0015.csv'};
% vary = {'0.002', '0.004', '0.006', '0.008' '0.010', '0.012', '0.014'};
% cats_idxs = 1:length(sims);
% cats_names = {'DT=2', 'DT=4', 'DT=6', 'DT=8', 'DT=10', 'DT=12', 'DT=14'};
% cats_cols = jet(length(sims));


% sims = {'Mito_Halo-N1-paJF646_001_mask_11075_poly.path/trajs_D=%s00000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_002_mask_13069_poly.path/trajs_D=%s00000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_003_mask_45_poly.path/trajs_D=%s00000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_004_mask_12330_poly.path/trajs_D=%s00000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_005_mask_6982_poly.path/trajs_D=%s00000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_006_mask_11009_poly.path/trajs_D=%s00000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv';
%         'Mito_Halo-N1-paJF646_007_mask_7791_poly.path/trajs_D=%s00000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv'};
% 
% free_sims = {'free_space/trajs_width=128_height=128_pxsize=0.967821_D=1.5_dt=0.0001_DT=0.006_lambda=20_dens=0.0015.csv';
%              'free_space/trajs_width=128_height=128_pxsize=0.967821_D=3_dt=0.0001_DT=0.006_lambda=20_dens=0.0015.csv';
%              'free_space/trajs_width=128_height=128_pxsize=0.967821_D=4.5_dt=0.0001_DT=0.006_lambda=20_dens=0.0015.csv';
%              'free_space/trajs_width=128_height=128_pxsize=0.967821_D=6_dt=0.0001_DT=0.006_lambda=20_dens=0.0015.csv';
%              'free_space/trajs_width=128_height=128_pxsize=0.967821_D=7.5_dt=0.0001_DT=0.006_lambda=20_dens=0.0015.csv';
%              'free_space/trajs_width=128_height=128_pxsize=0.967821_D=9_dt=0.0001_DT=0.006_lambda=20_dens=0.0015.csv'};
% cats_idxs = 1:length(sims);
% vary = {'1.5', '3.0', '4.5', '6.0', '7.5', '9.0'};
% cats_names = {'D=1.5', 'D=3.0', 'D=4.5', 'D=6.0', 'D=7.5', 'D=9.0'};
% cats_cols = jet(length(sims));

sims = {'ER/simus/C2-_2019_10_02__16_23_00_mask_5073.path/trajs_D=%s00000_dt=0.000100_DT=0.006000_npts=20_ntrajs=5000.csv'};
free_sim = 'free_space/trajs_width=128_height=128_pxsize=0.967821_D=%s_dt=0.0001_DT=0.006_lambda=20_dens=0.0015.csv';
vary = {'1.5', '3.0', '4.5', '6.0', '7.5', '9.0'};
cats_idxs = 1:length(vary);
cats_names = {'D=1.5', 'D=3.0', 'D=4.5', 'D=6.0', 'D=7.5', 'D=9.0'};
cats_cols = jet(length(vary));

% sims = {'ER/simus/C2-_2019_10_02__16_23_00_mask_5073.path/trajs_D=1.500000_dt=0.000100_DT=%s000_npts=20_ntrajs=5000.csv'};
% free_sim = 'free_space/trajs_width=128_height=128_pxsize=0.967821_D=1.5_dt=0.0001_DT=%s_lambda=20_dens=0.0015.csv';
% vary = {'0.002', '0.004', '0.006', '0.008', '0.010', '0.012', '0.014'};
% cats_idxs = 1:length(vary);
% cats_names = {'DT=0.002', 'D=0.004', 'D=0.006', 'D=0.008', 'D=0.010', 'D=0.012', 'D=0.014'};
% cats_cols = jet(length(vary));


show_trajs = 0;

grid_dx = 0.2;
min_D_grid_npts = 10;
    
disps = cell(length(sims), 1);
ivels = cell(length(sims), 1);
ivels_pooled = cell(length(unique(cats_idxs)), 1);
diffs = cell(length(sims), 1);
diffs_pooled = cell(length(unique(cats_idxs)), 1);

for n=1:length(sims)
    disps{n} = cell(length(vary), 1);
    ivels{n} = cell(length(vary), 1);
    ivels_pooled{n} = {};
    diffs{n} = cell(length(vary), 1);
    diffs_pooled{n} = {};
    for k=1:length(vary)
            fname = sprintf(sims{n}, vary{k});
            tab = csvread(sprintf('%s/%s', base_dir, fname));

        if show_trajs
            figure
            hold on
            for i=unique(tab(:,1))'
                plot(tab(tab(:,1) == i, 3), tab(tab(:,1) == i, 4), 'Color', rand(1,3))
            end
            hold off
            title(fname, 'Interpreter', 'None')
            daspect([1 1 1])
            tightfig();
            print(sprintf('/tmp/trajs_sim_%d.png', n), '-dpng')
        end

        ivels{n}{k} = Utils.inst_vels(tab);
        disps{n}{k} = ivels{n}{k} * Utils.find_dt(tab);
        ivels_pooled{cats_idxs(n)} = [ivels_pooled{cats_idxs(n)}; ivels{n}{k}];

        g = Utils.gen_grid(tab, grid_dx);
        diffs{n}{k} = Utils.diffusion_map(tab, g, g, min_D_grid_npts);
        diffs{n}{k} = diffs{n}{k}(diffs{n}{k} > 0)';
        diffs_pooled{cats_idxs(n)} = [diffs_pooled{cats_idxs(n)} diffs{n}{k}];
    end
end

disps_free = cell(length(sims), 1);
ivels_free = cell(length(sims), 1);
ivels_pooled_free = cell(length(unique(cats_idxs)), 1);
diffs_free = cell(length(sims), 1);
diffs_pooled_free = cell(length(unique(cats_idxs)), 1);
for k=1:length(vary)
    fname = sprintf(free_sim, vary{k});
    tab = csvread(sprintf('%s/%s', base_dir, fname));

%     if show_trajs
%         figure
%         hold on
%         for i=unique(tab(:,1))'
%             plot(tab(tab(:,1) == i, 3), tab(tab(:,1) == i, 4), 'Color', rand(1,3))
%         end
%         hold off
%         title(fname, 'Interpreter', 'None')
%         daspect([1 1 1])
%         tightfig();
%         print(sprintf('/tmp/trajs_free_sim_%d.png', n), '-dpng')
%     end

    ivels_free{k} = Utils.inst_vels(tab);
    disps_free{k} = ivels_free{k} * Utils.find_dt(tab);
    ivels_pooled_free{cats_idxs(k)} = [ivels_pooled_free{cats_idxs(k)}; ivels_free{k}];

    g = Utils.gen_grid(tab, grid_dx);
    diffs_free{k} = Utils.diffusion_map(tab, g, g, min_D_grid_npts);
    diffs_free{k} = diffs_free{k}(diffs_free{k} > 0)';
    diffs_pooled_free{cats_idxs(k)} = [diffs_pooled_free{cats_idxs(k)} diffs_free{k}];
end

 
for n=1:length(sims)
    figure
    hold on
    for k=1:length(vary)
        [o, b] = hist(ivels{n}{k}, 1:2:200);
        plot(b, o / sum(o), 'Color', cats_cols(cats_idxs(k),:), 'LineWidth', 1)
    end
    for k=1:length(vary)
        [o, b] = hist(ivels_free{k}, 1:2:200);
        plot(b, o / sum(o), '--', 'Color', cats_cols(cats_idxs(k),:), 'LineWidth', 1)
    end
    hold off
    axis square
    ylabel('Frequency')
    xlabel('Instantaneous velocity (µm/s)')
    legend(cats_names)
    tightfig();
    print(sprintf('/tmp/mito_sims_ivels_%d.png', n), '-dpng')
end

for n=1:length(sims)
    figure
    hold on
    for k=1:length(vary)
    [o, b] = hist(disps{n}{k}, 0.01:0.02:1.5);
    plot(b, o / sum(o), 'Color', cats_cols(cats_idxs(k),:), 'LineWidth', 1)
    end
    for k=1:length(vary)
        [o, b] = hist(disps_free{k}, 0.01:0.02:1.5);
        plot(b, o / sum(o), '--', 'Color', cats_cols(cats_idxs(k),:), 'LineWidth', 1)
    end
    hold off
    axis square
    ylabel('Frequency')
    xlabel('Displacements (µm)')
    legend(cats_names)
    tightfig();
    print(sprintf('/tmp/mito_sims_disps_%d.png', n), '-dpng')
end

% figure
% hold on
% for u=unique(cats_idxs)'
%     [o, b] = hist(ivels_pooled{u}, 1:2:150);
%     plot(b, o / sum(o), 'Color', cats_cols(u,:), 'LineWidth', 1)
% end
% hold off
% axis square
% ylabel('Frequency')
% xlabel('Pooled instantaneous velocity (µm/s)')
% tightfig();
% print('/tmp/mito_sims_ivels_pooled.png', '-dpng')


for n=1:length(sims)
    figure
    hold on
    for k=1:length(vary)
        [o, b] = hist(diffs{n}{k}, 40);
        plot(b, o / sum(o), 'Color', cats_cols(cats_idxs(k),:), 'LineWidth', 1)
    end
    for k=1:length(vary)
        [o, b] = hist(diffs_free{k}, 40);
        plot(b, o / sum(o), '--', 'Color', cats_cols(cats_idxs(k),:), 'LineWidth', 1)
    end
    hold off
    axis square
    ylabel('Frequency')
    xlabel('Diffusion coefficient (µm²/s)')
    legend(cats_names)
    tightfig();
    print(sprintf('/tmp/mito_sims_diff_%d.png', n), '-dpng')
end
for n=1:length(sims)
    for k=1:length(vary)
        display(sprintf('%d AVG diff %.3f ± %.3f n=%d', n, mean(diffs{n}{k}), std(diffs{n}{k}), length(diffs{n}{k})));
    end
end
for n=1:length(vary)
    display(sprintf('FREE %d AVG diff %.3f ± %.3f n=%d', n, mean(diffs_free{n}), std(diffs_free{n}), length(diffs_free{n})));
end

figure
hold on
for n=1:length(sims)
    %plot(Ds(n), mean(diffs{n}), 'x', 'Color', cats_cols(cats_idxs(n),:), 'LineWidth', 1)
    plot(cellfun(@(x) str2num(x), vary), cellfun(@(x) mean(x), diffs{n}), 'k', 'LineWidth', 1)
end
for k=1:length(vary)
    %plot(Ds(n), mean(diffs_free{n}), 'o', 'Color', cats_cols(cats_idxs(n),:), 'LineWidth', 1)
    plot(str2num(vary{k}), mean(diffs_free{k}), 'o', 'Color', cats_cols(cats_idxs(k),:), 'LineWidth', 1)
end
plot(cellfun(@(x) str2num(x), vary), cellfun(@(x) str2num(x), vary), 'k')
%plot(cellfun(@(x) str2num(x), vary), 1.5 * ones(length(vary), 1), 'k--')
hold off
axis square
%xlabel('Simulation timestep (ms)')
xlabel('Simulation diffusion coefficient(μm²/s)')
ylabel('Estimated diffusion coefficient (µm²/s)')
tightfig();
print('/tmp/mito_sims_diff_vs_expected.png', '-dpng')


% figure
% hold on
% for u=unique(cats_idxs)'
%     [o, b] = hist(diffs_pooled{u}, 40);
%     plot(b, o / sum(o), 'Color', cats_cols(u,:), 'LineWidth', 1.5)
% end
% hold off
% axis square
% ylabel('Frequency')
% xlabel('Pooled diffusion coefficient (µm²/s)')
% legend(cats_names)
% tightfig();
% print('/tmp/mito_sims_diff_pooled.png', '-dpng')
% for u=unique(cats_idxs)'
%     display(sprintf('POOLED %d AVG diff %.3f ± %.3f n=%d', u, mean(diffs_pooled{u}), std(diffs_pooled{u}), length(diffs_pooled{u})));
%end