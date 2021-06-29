classdef Utils
    methods(Static)
        function dt = find_dt(tab)
            dts = [];
            for k=unique(tab(:,1))'
                tr = tab(tab(:,1) == k, 2);
                dts = unique([dts; unique(tr(2:end) - tr(1:(end-1)))]);
            end

            dt = min(dts);
        end

        function res = ta_MSD(t)
            res = cell(size(t, 1) - 1, 1);
            
            for tau=1:size(t, 1)-1
                for j=1:(size(t, 1)-1-tau)
                    res{tau} = [res{tau}; sum((t(j+tau, 3:4) - t(j, 3:4)).^2, 2)];
                end
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

        function res = get_params(dname)
           res = struct();
            tmp = strsplit(dname, '_');
            for i=1:length(tmp)
                tmp2 = strsplit(tmp{i}, ':');
                if strcmp(tmp2{1}, 'id') || strcmp(tmp2{1}, 'Seed')
                    res.(tmp2{1}) = tmp2{2};
                else
                    res.(tmp2{1}) = str2num(tmp2{2});
                end
            end
        end

        function show_trajectories(tab)
            ih = ishold;
            if ~ih
                hold on
            end

            for k=unique(tab(:,1))'
                plot(tab(tab(:,1) == k, 3), tab(tab(:,1) == k, 4), 'Color', rand(1,3))
            end

            if ~ih
                hold off
            end
        end

        function show_ellipse(e, col)
            ih = ishold;
            if ~ih
                hold on
            end

            theta = linspace(0,2*pi,50);
            theta = [theta 2*pi + (theta(2) - theta(1))];
            plot(e(1) + e(3)*cos(theta)*cos(e(5)) - e(4)*sin(theta)*sin(e(5)), ...
                 e(2) + e(3)*cos(theta)*sin(e(5)) + e(4)*sin(theta)*cos(e(5)), ...
                'Color', col, 'LineWidth', 1)

            if ~ih
                hold off
            end
        end

        function in = in_ellipse(p, mu, r, phi)
            in = (cos(phi).*(p(:,1) - mu(1)) + sin(phi).*(p(:,2) - mu(2))).^2 ./ r(1)^2 + ...
                (sin(phi).*(p(:,1) - mu(1)) - cos(phi).*(p(:,2) - mu(2))).^2 ./ r(2)^2 <= 1;
        end

        function tab = gen_brown_trajs(D, dt, npts, ntrajs)
            tab = [];
            for k=1:ntrajs
                tab = [tab; k * ones(npts, 1) (0:(npts-1))'*dt cumsum([10 10; sqrt(2*D*dt) * randn(npts-1, 2)], 1)];
            end
        end
        
        function disps = displacements(tab)
            disps = [];
            for i = unique(tab(:,1))'
                tr = tab(tab(:,1) == i, :);
                disps = [disps; sqrt(sum((tr(2:end, 3:4) - tr(1:(end-1), 3:4)).^2, 2))];
            end
        end
    end
end
