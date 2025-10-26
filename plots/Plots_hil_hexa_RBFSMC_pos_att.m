% Load data
load('with_nn_nohil.mat');      % This loads variable 'out'
u1 = out.u1;
u2 = out.u2;
u3 = out.u3;
u4 = out.u4;
tout = out.tout;

load('with_nn_hil.mat');  % This loads variable 'out'
u1_hil = out.u1;
u2_hil = out.u2;
u3_hil = out.u3;
u4_hil = out.u4;
tout_hil = out.tout;

% Plotting
figure;
set(gcf, 'Position', [100, 100, 1000, 1000]); % Adjusted height for 4 plots
t = tiledlayout(4, 1, 'TileSpacing', 'compact', 'Padding', 'compact'); 

% First Plot: u1
nexttile;
hold on;
plot(tout, u1, 'b-', 'LineWidth', 2.5, 'DisplayName', '$u_1$ Simulation');
plot(tout_hil, u1_hil, 'r--', 'LineWidth', 2.5, 'DisplayName', '$u_1$ HIL');
ylabel('$u_1$', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Interpreter', 'latex', 'FontSize', 16, 'Location', 'best');
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Second Plot: u2
nexttile;
hold on;
plot(tout, u2, 'b-', 'LineWidth', 2.5, 'DisplayName', '$u_2$ Simulation');
plot(tout_hil, u2_hil, 'r--', 'LineWidth', 2.5, 'DisplayName', '$u_2$ HIL');
ylabel('$u_2$', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Interpreter', 'latex', 'FontSize', 16, 'Location', 'best');
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Third Plot: u3
nexttile;
hold on;
plot(tout, u3, 'b-', 'LineWidth', 2.5, 'DisplayName', '$u_3$ Simulation');
plot(tout_hil, u3_hil, 'r--', 'LineWidth', 2.5, 'DisplayName', '$u_3$ HIL');
ylabel('$u_3$', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Interpreter', 'latex', 'FontSize', 16, 'Location', 'best');
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Fourth Plot: u4
nexttile;
hold on;
plot(tout, u4, 'b-', 'LineWidth', 2.5, 'DisplayName', '$u_4$ Simulation');
plot(tout_hil, u4_hil, 'r--', 'LineWidth', 2.5, 'DisplayName', '$u_4$ HIL');
xlabel('Time (s)', 'Interpreter', 'latex', 'FontSize', 22);
ylabel('$u_4$', 'Interpreter', 'latex', 'FontSize', 22);
grid on;
legend('Interpreter', 'latex', 'FontSize', 16, 'Location', 'best');
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', 22);
xlim([0 76]);

% Optional Title
t.Title.FontSize = 16;
t.Title.Interpreter = 'latex';
