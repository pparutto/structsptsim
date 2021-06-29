%addpath('/tmp');

%[base_poly, polys] = load_polys();

tab = dlmread('/tmp/sim/trajs_empirical_dt=0.000100_DT=0.006000_lambdaNpts=0.050000_nframes=0_width=128_height=128_dens=0.000123.csv');

po = [
7.223125 0.4996875;
7.2678125 0.5525;
7.206875 0.641875;
7.1459375 0.73125;
7.0890625 0.73125;
7.0321875 0.73125;
6.97125 0.641875;
6.90625 0.5525;
6.90625 0.5240625;
6.90625 0.4915625;
6.979375 0.4753125;
7.0484375 0.455;
7.1134375 0.4509375;
7.1825 0.446875;
7.223125 0.4996875;
];


figure
hold on
Utils.show_trajectories(tab, 'rand');
%plot([4.37108 4.39335], [8.51846, 8.50945], 'r', 'LineWidth', 3);

% 
% plot(base_poly(:,1), base_poly(:,2), 'k')
% for k=1:length(polys)
%     plot(polys{k}(:,1), polys{k}(:,2), 'k') 
% end
hold off
axis square
daspect([1 1 1])
set(gca, 'YDir', 'Reverse')

frames = 0:4999;
cnts = zeros(length(frames), 1);
for i=1:length(frames)
    cnts(i) = sum(round(tab(:,2) ./ 0.004) == frames(i));
end

figure
plot(frames, cnts)

figure
hist(Utils.displacements(tab), 100)

% A = [128.5 14];
% B = [127 14];
% C = [125.5 14.1];
% 
% %C = [128.5 14];
% %B = [127 14];
% %A = [125.5 14.1];
% 
% %C = [135.5 17.7];
% %B = [132 15.9];
% %A = [128.5 14];
% 
% A = [135.5 17.7];
% B = [132 15.9];
% C = [128.5 14];
% 
% for i = 1:(size(poly,1)-2)
%     A = poly(i,:);
%     B = poly(i+1,:);
%     C = poly(i+2, :);
%     display(sprintf('%d', (B(1) - A(1)) * (C(2) - B(2)) - (C(1) - A(1)) * (B(2) - A(2))));
% end