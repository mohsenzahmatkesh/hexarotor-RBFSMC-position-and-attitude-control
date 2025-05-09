close all
figure;
set(gcf, 'Position', [100, 100, 1000, 800]); % Adjust figure size
t = tiledlayout(4, 1, 'TileSpacing', 'compact', 'Padding', 'compact'); % Three plots stacked vertically

% First Plot
nexttile;
hold on;
plot(tout,delta_phi, 'b-', 'LineWidth', 2.5, 'DisplayName', '${\Delta}_\phi$');
hold on
plot(tout,delta_h_phi, 'r--', 'LineWidth', 2.5, 'DisplayName', '$\hat{\Delta}_\phi$');
% title('Plot 1: Variable Comparison', 'Interpreter', 'latex', 'FontSize', 14);
% xlabel('Time (s)', 'Interpreter', 'latex', 'FontSize', 34);
ylabel('$\phi_{dist}$', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Location', 'best', 'Interpreter', 'latex', 'FontSize', 16);
hold off;
set(gca,  'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Second Plot
nexttile;
hold on;
plot(tout,delta_theta, 'b-', 'LineWidth', 2.5, 'DisplayName', '${\Delta}_\theta$');
plot(tout,delta_h_theta, 'r--', 'LineWidth', 2.5, 'DisplayName', '$\hat{\Delta}_\theta$');
% title('Plot 2: Variable Comparison', 'Interpreter', 'latex', 'FontSize', 14);
% xlabel('Time (s)', 'Interpreter', 'latex', 'FontSize', 24);
ylabel('$\theta_{dist}$', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Location', 'best', 'Interpreter', 'latex', 'FontSize', 16);
hold off;
set(gca,  'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Third Plot
nexttile;
hold on;
plot(tout,delta_psi, 'b-', 'LineWidth', 2.5, 'DisplayName', '${\Delta}_\psi$');
plot(tout,delta_h_psi, 'r--', 'LineWidth', 2.5, 'DisplayName', '$\hat{\Delta}_\psi$');
% title('Plot 3: Variable Comparison', 'Interpreter', 'latex', 'FontSize', 14);
% xlabel('Time (s)', 'Interpreter', 'latex', 'FontSize', 22);
ylabel('$\psi_{dist}$', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Location', 'best', 'Interpreter', 'latex', 'FontSize', 16);
hold off;
set(gca,  'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Forth Plot
nexttile;
hold on;
plot(tout,delta_z, 'b-', 'LineWidth', 2.5, 'DisplayName', '${\Delta}_z$');
plot(tout,delta_h_z, 'r--', 'LineWidth', 2.5, 'DisplayName', '$\hat{\Delta}_z$');
% title('Plot 3: Variable Comparison', 'Interpreter', 'latex', 'FontSize', 14);
xlabel('Time (s)', 'Interpreter', 'latex', 'FontSize', 22);
ylabel('$z_{dist}$', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Location', 'best', 'Interpreter', 'latex', 'FontSize', 16);
hold off;
set(gca,  'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);



% Adjust layout title if needed
% t.Title.String = 'Comparison of Variables With and Without NN';
t.Title.FontSize = 16;
t.Title.Interpreter = 'latex';

% Load data from both .mat files
data_with_nn = load('with_nn.mat');
data_no_nn = load('no_nn.mat');

% Extract variables from with_nn.mat
x_with_nn = data_with_nn.x;
y_with_nn = data_with_nn.y;
z_with_nn = data_with_nn.z;

% Extract variables from no_nn.mat
x_no_nn = data_no_nn.x;
y_no_nn = data_no_nn.y;
z_no_nn = data_no_nn.z;

% Extract desired trajectory (assuming it's in with_nn.mat)
xdes = data_with_nn.xdes;
ydes = data_with_nn.ydes;
zdes = data_with_nn.zdes;

% Plot
% Plot
figure;
set(gcf, 'Position', [100, 100, 1000, 800]); % Adjust figure size
t = tiledlayout(1, 1, 'TileSpacing', 'none', 'Padding', 'none'); % Remove all margins


nexttile;
plot3(xdes, ydes, zdes, 'k--', 'LineWidth', 2.5, 'DisplayName', 'Desired'); hold on;
plot3(x_with_nn, y_with_nn, z_with_nn, 'b-', 'LineWidth', 2.5, 'DisplayName', 'NN-SMC Method');
plot3(x_no_nn, y_no_nn, z_no_nn, 'r-', 'LineWidth', 2.5, 'DisplayName', 'SMC Method');

grid on;
xlabel('$x$ [m]', 'Interpreter', 'latex', 'FontSize', 22);
ylabel('$y$ [m]', 'Interpreter', 'latex', 'FontSize', 22);
zlabel('$z$ [m]', 'Interpreter', 'latex', 'FontSize', 22);
legend('Interpreter', 'latex', 'FontSize', 16, 'Location', 'best');
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', 22);
view(3); % 3D view

% Extract time and attitude data from loaded structures
tout = data_with_nn.tout;  % assuming both have the same time vector

phi_with = data_with_nn.phi;
theta_with = data_with_nn.theta;
psi_with = data_with_nn.psi;

phi_no = data_no_nn.phi;
theta_no = data_no_nn.theta;
psi_no = data_no_nn.psi;

% Plot figure
figure;
set(gcf, 'Position', [100, 100, 1000, 800]); % Adjust figure size
t = tiledlayout(3, 1, 'TileSpacing', 'compact', 'Padding', 'compact'); % 3 stacked plots

% First Plot: phi
nexttile;
hold on;
plot(tout, phi_with, 'b-', 'LineWidth', 2.5, 'DisplayName', '$\phi$ NN-SMC Method');
plot(tout, phi_no, 'r--', 'LineWidth', 2.5, 'DisplayName', '$\phi$ without NN');
ylabel('$\phi$ (deg)', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Interpreter', 'latex', 'FontSize', 16, 'Location', 'best');
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Second Plot: theta
nexttile;
hold on;
plot(tout, theta_with, 'b-', 'LineWidth', 2.5, 'DisplayName', '$\theta$ NN-SMC Method');
plot(tout, theta_no, 'r--', 'LineWidth', 2.5, 'DisplayName', '$\theta$ without NN');
ylabel('$\theta$ (deg)', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Interpreter', 'latex', 'FontSize', 16, 'Location', 'best');
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Third Plot: psi
nexttile;
hold on;
plot(tout, psi_with, 'b-', 'LineWidth', 2.5, 'DisplayName', '$\psi$ NN-SMC Method');
plot(tout, psi_no, 'r--', 'LineWidth', 2.5, 'DisplayName', '$\psi$ without NN');
xlabel('Time (s)', 'Interpreter', 'latex', 'FontSize', 22);
ylabel('$\psi$ (deg)', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Interpreter', 'latex', 'FontSize', 16, 'Location', 'best');
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Optional: Title (currently hidden)
% t.Title.String = 'Attitude Angles With and Without NN';
t.Title.FontSize = 16;
t.Title.Interpreter = 'latex';


x_with = data_with_nn.x;
y_with = data_with_nn.y;
z_with = data_with_nn.z;

x_no = data_no_nn.x;
y_no = data_no_nn.y;
z_no = data_no_nn.z;

% Plot figure
figure;
set(gcf, 'Position', [100, 100, 1000, 800]); % Adjust figure size
t = tiledlayout(3, 1, 'TileSpacing', 'compact', 'Padding', 'compact'); % 3 stacked plots

% First Plot: x
nexttile;
hold on;
plot(tout, x_with, 'b-', 'LineWidth', 2.5, 'DisplayName', '$x$ NN-SMC Method');
plot(tout, x_no, 'r--', 'LineWidth', 2.5, 'DisplayName', '$x$ without NN');
ylabel('$x$ (m)', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Interpreter', 'latex', 'FontSize', 16, 'Location', 'best');
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Second Plot: y
nexttile;
hold on;
plot(tout, y_with, 'b-', 'LineWidth', 2.5, 'DisplayName', '$y$ NN-SMC Method');
plot(tout, y_no, 'r--', 'LineWidth', 2.5, 'DisplayName', '$y$ without NN');
ylabel('$y$ (m)', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Interpreter', 'latex', 'FontSize', 16, 'Location', 'best');
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Third Plot: z
nexttile;
hold on;
plot(tout, z_with, 'b-', 'LineWidth', 2.5, 'DisplayName', '$z$ NN-SMC Method');
plot(tout, z_no, 'r--', 'LineWidth', 2.5, 'DisplayName', '$z$ without NN');
xlabel('Time (s)', 'Interpreter', 'latex', 'FontSize', 22);
ylabel('$z$ (m)', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Interpreter', 'latex', 'FontSize', 16, 'Location', 'best');
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Optional: Title (currently hidden)
% t.Title.String = 'Position Coordinates With and Without NN';
t.Title.FontSize = 16;
t.Title.Interpreter = 'latex';


xd_with = data_with_nn.xd;
yd_with = data_with_nn.yd;
zd_with = data_with_nn.zd;

xd_no = data_no_nn.xd;
yd_no = data_no_nn.yd;
zd_no = data_no_nn.zd;

% Plot figure
figure;
set(gcf, 'Position', [100, 100, 1000, 800]); % Adjust figure size
t = tiledlayout(3, 1, 'TileSpacing', 'compact', 'Padding', 'compact'); % 3 stacked plots

% First Plot: xd
nexttile;
hold on;
plot(tout, xd_with, 'b-', 'LineWidth', 2.5, 'DisplayName', '$x_d$ NN-SMC Method');
plot(tout, xd_no, 'r--', 'LineWidth', 2.5, 'DisplayName', '$x_d$ without NN');
ylabel('$x_d$ (m/s)', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Interpreter', 'latex', 'FontSize', 16, 'Location', 'best');
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Second Plot: yd
nexttile;
hold on;
plot(tout, yd_with, 'b-', 'LineWidth', 2.5, 'DisplayName', '$y_d$ NN-SMC Method');
plot(tout, yd_no, 'r--', 'LineWidth', 2.5, 'DisplayName', '$y_d$ without NN');
ylabel('$y_d$ (m/s)', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Interpreter', 'latex', 'FontSize', 16, 'Location', 'best');
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Third Plot: zd
nexttile;
hold on;
plot(tout, zd_with, 'b-', 'LineWidth', 2.5, 'DisplayName', '$z_d$ NN-SMC Method');
plot(tout, zd_no, 'r--', 'LineWidth', 2.5, 'DisplayName', '$z_d$ without NN');
xlabel('Time (s)', 'Interpreter', 'latex', 'FontSize', 22);
ylabel('$z_d$ (m/s)', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Interpreter', 'latex', 'FontSize', 16, 'Location', 'best');
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Optional: Title (currently hidden)
% t.Title.String = 'Desired Positions With and Without NN';
t.Title.FontSize = 16;
t.Title.Interpreter = 'latex';
