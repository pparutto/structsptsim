classdef Utils
    methods(Static)
        function [files, cats] = find_all_files(based, fname)
            files = {};
            cats = {};

            todo = {based};
            while ~isempty(todo)
                curd = todo{1};
                todo = todo(2:end);

                subds = dir(curd);
                for k=1:length(subds)
                    if ~strcmp(subds(k).name, '.') && ~strcmp(subds(k).name, '..') && subds(k).isdir
                        todo{length(todo)+1} = sprintf('%s/%s', subds(k).folder, subds(k).name);
                    elseif strcmp(subds(k).name, fname)
                        files{length(files)+1} = sprintf('%s/%s', subds(k).folder, fname);
                        cats{length(cats)+1} = strsplit(subds(k).folder(length(based)+1:end), '/');
                    end
                end
            end
        end

        function dt = find_dt(tab)
            dts = [];
            for k=unique(tab(:,1))'
                tr = tab(tab(:,1) == k, 2);
                dts = unique([dts; unique(tr(2:end) - tr(1:(end-1)))]);
            end

            dt = min(dts);
        end

        function tab = load_trackmate(fname)
            dat = dlmread(fname, ',', 1, 1);
            tab = dat(:, [2, 7, 4, 5]);
        end

        function tab = load_trackmate_script(fname)
            dat = dlmread(fname, ',', 1, 0);
            tab = dat(:, [1, 5, 3, 4]);
        end

        function tab = load_trackmate_spots(fname)
            dat = dlmread(fname, ',', 1, 0);
            tab = dat(:, [4, 2, 3]);
        end

        function grid = gen_grid(tab, r)
            Xmax=max(tab(:,3));
            Ymax=max(tab(:,4));

            Nx=ceil(Xmax/r)+1;
            Ny=ceil(Ymax/r)+1;
            N=max(Nx,Ny);
            
            grid = (0:N) .* r;
        end

        function cat = antitryps_cat(fname)
            if contains(fname, '-M_')
                cat = 'M';
            elseif contains(fname, '-Z_')
                cat = 'Z';
            else
                assert false
            end
        end

        function ivels = inst_vels(tab)
            ivels = [];

            for i=unique(tab(:,1))'
                tr = tab(tab(:,1) == i, :);
                ivels = [ivels; sqrt(sum((tr(2:end, 3:4) - tr(1:(end-1), 3:4)).^2, 2)) ./ ...
                        (tr(2:end, 2) - tr(1:(end-1), 2))];
            end
        end
        
        function ivels_tr = inst_vels_along_traj(tab)
            ivels_tr = {};

            for i=unique(tab(:,1))'
                tr = tab(tab(:,1) == i, :);
                ivels_tr{i+1} = sqrt(sum((tr(2:end, 3:4) - tr(1:(end-1), 3:4)).^2, 2)) ./ ...
                                    (tr(2:end, 2) - tr(1:(end-1), 2));
            end
        end

        function disp_cats = ivels_disps(tab, cats)
            %cats must start at 0 and end at >= max(ivels)
            ivels = [];
            disp_cats = cell(length(cats)-1, 1);

            for i=unique(tab(:,1))'
                tr = tab(tab(:,1) == i, :);
                cur_ivels = sqrt(sum((tr(2:end, 3:4) - tr(1:(end-1), 3:4)).^2, 2)) ./ ...
                    (tr(2:end, 2) - tr(1:(end-1), 2));
                ivels = [ivels; cur_ivels];

                for j=1:length(cur_ivels)
                    cidx = max(find(cur_ivels(j) - cats >= 0));
                    disp_cats{cidx} = [disp_cats{cidx}; tr([j j+1], 3:4); nan nan];
                end
            end
        end

        function vel_cats = ivels_cats_percs(b, o, perc)
            %perc in [0 1]
            vel_cats = [0];

            co = cumsum(o) / sum(o);
            cur_perc = perc;
            for i=1:(length(co)-1)
                if co(i) >= cur_perc
                    vel_cats = [vel_cats b(i)];
                    cur_perc = cur_perc + perc;
                end
            end

            vel_cats = [vel_cats b(end)];
        end

        function tidxs = filter_trajectories_idxs_npts(tab, npts)
            tidxs = unique(tab(:,1))';
            keep = [];
            for i=1:length(tidxs)
                if sum(tab(:,1) == tidxs(i)) > npts
                    keep = [keep tidxs(i)];
                end
            end
            tidxs = keep;
        end

        function tab_new = filter_trajectories_npts(tab, npts)
            tab_new = [];
            for i=unique(tab(:,1))'
                if sum(tab(:,1) == i) > npts
                    tab_new = [tab_new; tab(tab(:,1) == i, :)];
                end
            end
        end

        function [gx, gy] = gen_grid_from_mat(minp, maxp, dx)
            Nx=ceil(single(maxp(1) - minp(1)) / dx) + 1;
            Ny=ceil(single(maxp(2) - minp(2)) / dx) + 1;
            N=max(Nx, Ny);

            gx = minp(1) + (0:N) .* dx;
            gy = minp(2) + (0:N) .* dx;
        end

        function dens = density_map(tab, gx, gy, types)
            assert(any(cellfun(@(x) strcmp(x, 'dens'), types) | cellfun(@(x) strcmp(x, 'npts'), types) | ...
                cellfun(@(x) strcmp(x, 'log'), types)))
            r = gx(2) - gx(1);
            dens = zeros(length(gx), length(gy));

            for k = 1:size(tab, 1)
                cx = floor(single((tab(k,3) - gx(1)) / r)) + 1;
                cy = floor(single((tab(k,4) - gy(1)) / r)) + 1;
                
                if cx > 0 && cy > 0 && cx <= length(gx) && cy <= length(gy)
                    dens(cx, cy) = dens(cx, cy) + 1;
                end
            end

            if any(cellfun(@(x) strcmp(x, 'dens'), types))
                dens = dens / r^2;
            end
            if any(cellfun(@(x) strcmp(x, 'log'), types))
                dens = log(dens);
            end
        end

        function D = diffusion_map(tab, gx, gy, npts_th)
            r = gx(2) - gx(1);
            sigmaxx= zeros(length(gx), length(gy));
            sigmayy= zeros(length(gx), length(gy));
            cpt = zeros(length(gx), length(gy));

            for i=1:length(tab)-1
                line = tab(i,:);
                line2 = tab(i+1,:);

                if line(1)==line2(1)
                    if ~isnan(line(3) + line2(3))
                        x = floor(single((line(3)-gx(1)) / r)) + 1;
                        y = floor(single((line(4)-gy(1)) / r)) + 1;

                        dx = (line2(3) - line(3))^2;
                        dy = (line2(4) - line(4))^2;
                        dt = line2(2) - line(2);

                        if x > 0 && y > 0
                            sigmaxx(x,y) = sigmaxx(x,y) + dx / dt;
                            sigmayy(x,y) = sigmayy(x,y) + dy / dt;
                            cpt(x,y) = cpt(x,y) + 1;
                        end
                    end
                end
            end

            sigmaxx = sigmaxx ./ cpt;
            sigmayy = sigmayy ./ cpt;
            D = (sigmaxx + sigmayy)/4;
            D(cpt < npts_th) = nan;
        end

        function drift = drift_map(tab, gx, gy, npts_th)
            r = gx(2) - gx(1);
            drift = zeros(length(gx), length(gy), 2);
            cpt = zeros(length(gx), length(gy));

            for i=1:(size(tab,1)-1)
                line=tab(i,:);
                line2=tab(i+1,:);

                if line(1)==line2(1)
                    if ~isnan(line(3)+line2(3))
                        x = floor(single(line(3) - gx(1)) / r) + 1;
                        y = floor(single(line(4) - gy(1)) / r) + 1;

                        assert(line2(2) ~= line(2))
                        if x > 0 && y > 0 && x <= size(drift,1) && y <= size(drift,2)
                            drift(x, y, 1) = drift(x, y, 1) + (line2(3)-line(3))  / (line2(2)-line(2));
                            drift(x, y, 2) = drift(x, y, 2) + (line2(4)-line(4)) / (line2(2)-line(2));
                            cpt(x, y) = cpt(x, y) + 1;
                        end
                    end
                end
            end

            drift(:,:,1) = drift(:,:,1) ./ cpt;
            drift(:,:,2) = drift(:,:,2) ./ cpt;

            for i=1:size(drift, 1)
                for j=1:size(drift, 2)
                    if cpt(i,j) < npts_th
                        drift(i,j,:) = [nan, nan];
                    end
                end
            end
        end

        function res = gen_scalar_map_from_mat(gx, gy, s)
            res = zeros(length(gx), length(gy)) * nan;
            ss = strsplit(s, ';');
            
            for i=1:length(ss)
                tmp = strsplit(ss{i}, ':');
                res(str2num(tmp{1})+1, str2num(tmp{2})+1) = str2double(tmp{3});
            end
        end

        function res = gen_vector_map_from_mat(gx, gy, s)
            res = zeros(length(gx), length(gy), 2) * nan;
            ss = strsplit(s, ';');

            for i=1:length(ss)
                tmp = strsplit(ss{i}, ':');
                res(str2num(tmp{1})+1, str2num(tmp{2})+1, :) = str2double(tmp(3:4));
            end
        end

        function show_trajectories(tab, col)
            ih = ishold;
            if ~ih
                hold on
            end

            if strcmp(col, 'rand')
                for i=unique(tab(:,1))'
                    plot(tab(tab(:,1) == i, 3), tab(tab(:,1) == i, 4), 'Color', rand(1,3), 'LineWidth', 0.2)
                end
            else
                tmp = [];
                for i=unique(tab(:,1))'
                    tmp = [tmp; tab(tab(:,1) == i, 3:4); nan nan];
                end
                plot(tmp(:,1), tmp(:,2), 'Color', col, 'LineWidth', 0.2)
            end

            if ~ih
                hold off
            end
        end

        function show_scalar_map(scalmap, gx, gy)
            ih = ishold;
            if ~ih
                hold on
            end

            r = gx(2) - gx(1);

            scalmap = scalmap(floor(single((gx-gx(1))/ r)) + 1, floor(single((gy-gy(1)) / r)) + 1);

            cm = jet(256);
            cm(1,:) = [1, 1, 1];
            colormap(gca, cm);

            M = max(scalmap(:));

            for i=1:size(scalmap, 1)
                for j=1:size(scalmap, 2)
                    if scalmap(i,j) > 0
                        fill(gx(i) + [0 0 1 1] * r, gy(j) + [0 1 1 0] * r, ...
                                ind2rgb(ceil((scalmap(i,j) ./ M).*255)+1, cm), 'EdgeColor', 'none')
                    end
                end
            end

            %imagesc needs the center of the pixel hence the + r/2
            %imagesc(scalmap', 'XData', [gx(1), gx(end)], 'YData', [gy(1), gy(end)]);
            %set(gca,'YDir','normal')
            if ~ih
                hold off
            end
        end

        function show_vector_map(drift, gx, gy, scaling, lw)
            function [] = my_quiver(lx, ly, ldx, ldy, mult)
                alpha = 0.33; % Size of arrow head relative to the length of the vector
                beta = 0.33;  % Width of the base of the arrow head relative to the
                eps=0.0001;

                cm = [1 0 1; 1 0 0; 0 1 0; 0 1 1];

                ih = ishold;
                if ~ih
                    hold on
                end

                for k=1:length(lx)
                    x = lx(k);
                    y = ly(k);
                    dx = ldx(k);
                    dy = ldy(k);

                    DX=dx/sqrt(dx^2+dy^2);
                    DY=dy/sqrt(dx^2+dy^2);

                    th = mod(atan2(DY, DX) + 2*pi, 2*pi);

                    if th >= 7*pi/4 || th < pi/4
                        cidx = 1;
                    elseif th >= pi/4 && th < 3*pi/4
                        cidx = 2;
                    elseif th >= 3*pi/4 && th < 5*pi/4
                        cidx = 3;
                    else
                        cidx = 4;
                    end

                    if mult > 0
                        dx = DX * mult;
                        dy = DY * mult;
                    elseif mult <= 0
                        dx = abs(mult) * dx;
                        dy = abs(mult) * dy;
                    end

                    plot([x x+dx],[y y+dy],'Color',cm(cidx,:), 'LineWidth', lw);
                    plot([x+dx, x+dx-alpha*(dx+beta*(dy+eps))] , ...
                        [y+dy, y+dy-alpha*(dy-beta*(dx+eps))] , ...
                        'Color',cm(cidx,:), 'LineWidth', lw);
                    plot([x+dx, x+dx-alpha*(dx-beta*(dy+eps))] , ...
                        [y+dy, y+dy-alpha*(dy+beta*(dx+eps))] , ...
                        'Color',cm(cidx,:), 'LineWidth',lw);
                end

                if ~ih
                    hold off
                end
            end

            r = gx(2) - gx(1);
            d = [];
            for i=1:length(gx)
                for j=1:length(gy)
                    ix = floor(single(gx(i) - min(gx)) / r) + 1;
                    jx = floor(single(gy(j) - min(gy)) / r) + 1;
                    if all(~isnan(drift(ix, jx, 1:2)))
                        d = [d; [gx(i) + r/2, gy(j) + r/2, ...
                            drift(ix, jx, 1), drift(ix, jx, 2)]];
                    end
                end
            end
            if ~isempty(d)
                my_quiver(d(:, 1), d(:,2), d(:,3), d(:,4), scaling);
            end
        end

        function show_ellipse(e, col)
            ih = ishold;
            if ~ih
                hold on
            end

            %plot(mu(1), mu(2), 'x', 'Color', col);

            theta = linspace(0,2*pi,50);
            theta = [theta 2*pi + (theta(2) - theta(1))];
            plot(e(1) + e(3)*cos(theta)*cos(e(5)) - e(4)*sin(theta)*sin(e(5)), ...
                 e(2) + e(3)*cos(theta)*sin(e(5)) + e(4)*sin(theta)*cos(e(5)), ...
                'Color', col, 'LineWidth', 1)
            
            if ~ih
                hold off
            end
        end

        function idx = val_to_idx(v, vmin, vmax, cm_size)
            idx = floor(single((v - vmin) / vmax) * (cm_size - 1)) + 1;
        end

        function [res, tr_id] = trajs_num_pts(tab)
            res = [];
            tr_id = [];
            for i=unique(tab(:,1))'
                res = [res; sum(tab(:,1) == i)];
                tr_id = [tr_id; i];
            end
        end

        function [res, tr_id] = trajs_max_dist(tab)
            res = [];
            tr_id = [];
            for i=unique(tab(:,1))'
                tr = tab(tab(:,1) == i, 3:4);
                res = [res; max(sqrt(sum((tr - tr(1,:)).^2, 2)))];
                tr_id = [tr_id; i];
            end
        end

        function [res, tr_id] = trajs_max_radius(tab)
            res = [];
            tr_id = [];
            for i=unique(tab(:,1))'
                m = mean(tab(tab(:,1) == i, 3:4), 1);
                res = [res; max(sqrt(sum((tab(tab(:,1) == i, 3:4) - m).^2, 2)))];
                tr_id = [tr_id; i];
            end
        end

        function [res, tr_id] = total_distance(tab)
            res = [];
            tr_id = [];
            for i=unique(tab(:,1))'
                tr = tab(tab(:,1) == i, :);

                if size(tr, 1) < 2
                    continue
                end

                res = [res; sqrt(sum((tr(end, 3:4) - tr(1, 3:4)).^2, 2)) / size(tr,1)];
                tr_id = [tr_id; i];
            end
        end

        function [res, res_trajs, tr_id] = trajs_max_conf_points(tab, r)
            rr = r * r;
            res = [];
            res_trajs = {};
            tr_id = [];
            for i=unique(tab(:,1))'
                tr = tab(tab(:,1) == i, :);
                cur_max = 0;
                res_trajs{length(res_trajs)+1} = [];
                for j=1:size(tr, 1)
                    for k=(j+1):size(tr, 1)
                        m = mean(tr(j:k, 3:4), 1);
                        if any(sum((tr(j:k, 3:4) - m).^2, 2) > rr)
                            break
                        end
                    end
                    if k-j-1 > cur_max
                        cur_max = k - j - 1;
                        res_trajs{end} = zeros(size(tr, 1), 1);
                        res_trajs{end}(j:(k-1)) = 1;
                    end
                end
                res = [res; cur_max];
                tr_id = [tr_id; i];
            end
        end

        function cross_pos = detect_recursion(tr, tau_min)
            cross_pos = [];
            for i=1:(size(tr, 1) - 1)
                for j=(i+tau_min):(size(tr, 1) - 1)
                    if polyxpoly(tr([i i+1], 3), tr([i i+1], 4), ...
                                 tr([j j+1], 3), tr([j j+1], 4))
                        cross_pos = [cross_pos; j];
                    end
                end
            end

            if isempty(cross_pos)
                return;
            end

            cross_pos = sort(cross_pos);
            tmp = cross_pos(1);
            for i=2:length(cross_pos)
                if cross_pos(i) ~= cross_pos(i-1) + 1
                    tmp = [tmp; cross_pos(i)];
                end
            end
            cross_pos = tmp;
        end

        function scalarmap_to_svg(m, max_val, px_size, outf)
            fid = fopen(outf, 'w');
            fprintf(fid, '<?xml version="1.0" standalone="yes"?>\n');
            fprintf(fid, '<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" ');
            fprintf(fid, '"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">\n');
            fprintf(fid, sprintf('<svg viewBox="0 0 %g %g" ', size(m,1)*px_size, size(m,2)*px_size));
            fprintf(fid, 'xmlns="http://www.w3.org/2000/svg" version="1.1" ');
            fprintf(fid, 'xmlns:xlink="http://www.w3.org/1999/xlink">\n');
            fprintf(fid, '<rect x="0" y="0" width="%g" height="%g" fill="rgb(255, 255, 255)"/>\n', (size(m,1)-1)*px_size, (size(m, 2)-1)*px_size);
            fprintf(fid, sprintf("<!--\nmax: %.3f\n-->\n", max_val));
            fprintf(fid, '<g>\n');

            for i = 1:size(m, 2)
                for j=1:size(m, 1)
                    if ~all(m(i,j,:) == 0)
                        fprintf(fid, '<rect x="%g" y="%g" width="%g" height="%g" ', ...
                            (i-1)*px_size, (size(m,2) - (j-1))*px_size, px_size, px_size);
                        fprintf(fid, 'fill="rgb(%g,%g,%g)"/>\n', m(i,j,:) .* 255);
                    end
                end
            end
            fclose(fid);
        end

        function res = ta_MSD_tab(tab)
            idxs = unique(tab(:,1))';
            res = cell(length(idxs), 1);
            
            for k=1:length(idxs)
                t = tab(tab(:,1) == idxs(k), :);

                res{k} = cell(size(t, 1) - 1, 1);
                for tau=1:size(t, 1)-1
                    for j=1:(size(t, 1)-1-tau)
                        res{k}{tau} = [res{k}{tau}; sum((t(j+tau, 3:4) - t(j, 3:4)).^2, 2)];
                    end
                end
            end
        end
        
        function Ds = diff_ta_MSD(taMSD, fit_N, DT)
            Ds = [];
            for u=1:length(taMSD)
                xs = (0:length(taMSD{u})) * DT;
                ys = cellfun(@(x) mean(x), taMSD{u});
                filt = cellfun(@(x) length(x) >= 3, taMSD{u});
                ys(~filt) = nan;
                
                xs = xs(~isnan(ys));
                ys = ys(~isnan(ys));
                
                if length(ys) < fit_N
                    continue
                end
                
                warning('off')
                [fi, gof] = fit(xs(1:fit_N)', ys(1:fit_N), 'poly1');
                warning('on')
                Ds = [Ds fi.p1/4];
            end
        end
        
        function Ds = compute_ta_MSD_D(taMSDs, DT)
            Ds = [];
            for u=1:length(taMSDs)
                xs = (0:length(taMSDs{u})) * DT;
                ys = cellfun(@(x) mean(x), taMSDs{u});
                filt = cellfun(@(x) length(x) >= 3, taMSDs{u});
                ys(~filt) = nan;

                xs = xs(~isnan(ys));
                ys = ys(~isnan(ys));

                if length(ys) < 5
                    continue
                end

                warning('off')
                [fi, gof] = fit(xs(1:5)', ys(1:5), 'a*x');
                warning('on')
                Ds = [Ds; fi.a/4];
            end
        end

        function sds = ensemble_MSD(tab)
            sds = {};
            sds{1} = [0];
            for i=unique(tab(:,1))'
                tr = tab(tab(:,1) == i, :);

                for j=2:size(tr, 1)
                    if j > length(sds)
                        sds{j} = [];
                    end
                    sds{j} = [sds{j} sum((tr(j,3:4) - tr(1,3:4)).^2)];
                end
            end
        end

        function disps = displacements(tab)
            disps = [];
            for i=unique(tab(:,1))'
                tr = tab(tab(:,1) == i, :);
                disps = [disps sqrt(sum((tr(2:end, 3:4) - tr(1:(end-1), 3:4)).^2, 2))'];
            end
        end
        
        function res = mymax(l)
            if isempty(l)
                res = nan;
            else
                res = max(l);
            end
        end
        
        function res = generate_brownian_motion(Ntrajs, Npts, D, dt, Nsampling)
            res = [];
            for k=1:Ntrajs
                X = rand(1,2);
                res = [res; k 0 X];
                for l=1:(Npts*Nsampling)
                    X = X + sqrt(2*D*dt) * randn(1,2);
                    if mod(l, Nsampling) == 0
                        res = [res; k l*dt X];
                    end
                end
            end
        end
    end
end

