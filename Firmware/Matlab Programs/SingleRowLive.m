% CSV file path
CSV_FILE = 'Dheeraj13.csv';

% Sampling rate for live updates
update_rate = 0.1; % Adjust as needed

% Fixed X-axis window size
time_window = 10;  % Show last 10 seconds of data (Adjust as needed)

% Create figure
fig = figure;
hold on;

% Read the file once to determine the number of sensor columns
data = readmatrix(CSV_FILE);
num_sensors = size(data, 2) - 1;  % Assuming first column is timestamps

% Generate plot handles dynamically
colors = lines(num_sensors);  % Unique colors for each sensor
sensor_plots = gobjects(1, num_sensors);
for i = 1:num_sensors
    sensor_plots(i) = plot(NaN, NaN, 'Color', colors(i, :), 'LineWidth', 1.5, 'DisplayName', ['Sensor ' num2str(i)]);
end

xlabel('Time (s)');
ylabel('Sensor Value');
title('Live Sensor Data Plot');
grid on;
legend show;
hold off;

% Initialize fixed x-axis limits
xlim([0 time_window]);

% Start live plotting
while ishandle(fig)  % Run while figure is open
    % Read CSV file
    data = readmatrix(CSV_FILE);
    
    if size(data, 1) > 1  % Ensure data is available
        timestamps = data(:, 1) / 1e6; % Convert timestamps from microseconds to seconds
        sensor_values = data(:, 2:end); % Extract all sensor columns

        % Convert timestamps to relative time (seconds since first reading)
        time_elapsed = timestamps - timestamps(1);
        
        % Find the latest time window for plotting
        start_time = max(0, time_elapsed(end) - time_window);
        end_time = start_time + time_window;

        % Select only data within the last time_window seconds
        valid_indices = time_elapsed >= start_time;
        time_plot = time_elapsed(valid_indices);
        sensor_plot_values = sensor_values(valid_indices, :);

        % Update each sensor plot
        for i = 1:num_sensors
            set(sensor_plots(i), 'XData', time_plot, 'YData', sensor_plot_values(:, i));
        end

        % Keep X-axis fixed
        xlim([start_time end_time]);
        
        % Adjust Y-axis dynamically based on all sensors
        all_values = sensor_plot_values(:);  % Flatten to a single array
        if ~isempty(all_values)
            ylim([min(all_values) - 10, max(all_values) + 10]); 
        end
    end

    % Pause before next update
    pause(update_rate);  
end
