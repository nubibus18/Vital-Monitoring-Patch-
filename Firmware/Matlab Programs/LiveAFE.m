% CSV file path
CSV_FILE = 'Dheeraj13.csv';

% Sampling rate for live updates
update_rate = 0.1; % Update every 100ms

% Fixed X-axis window size (in seconds)
time_window = 10;  % Show last 10 seconds of data

% Create figure
fig = figure('Name', 'Live UDP Data Plot', 'NumberTitle', 'off', ...
             'Position', [100, 100, 1200, 600]);

% Set up the axis properties
ax = axes('Parent', fig, 'Position', [0.1, 0.15, 0.8, 0.75]);
hold on;
xlabel('Time (s)');
ylabel('Value');
title('Live UDP Data Plot');
grid on;

% Get number of channels from the CSV file
try
    data = readmatrix(CSV_FILE);
    num_channels = size(data, 2) - 1; % Subtract 1 for timestamp column
catch
    num_channels = 4; % Default if file can't be read yet
end

% Create color palette for channels
colors = lines(num_channels);

% Initialize empty plots
h = gobjects(num_channels, 1);
for i = 1:num_channels
    h(i) = plot(NaN, NaN, 'LineWidth', 1.5, 'Color', colors(i,:), ...
                'DisplayName', sprintf('Channel %d', i));
end

% Add legend
legend('Location', 'northeastoutside');

% Start live plotting
while ishandle(fig)  % Run while figure is open
    try
        % Read CSV file
        data = readmatrix(CSV_FILE);
        
        if size(data, 1) > 1  % Ensure there's data
            timestamps = data(:, 1);  % First column is timestamp (in microseconds)
            values = data(:, 2:end);  % Remaining columns are data channels
            
            % Convert timestamps to seconds (relative to first sample)
            time_elapsed = (timestamps - timestamps(1)) / 1e6;
            
            % Find the latest time window
            if time_elapsed(end) > time_window
                start_time = time_elapsed(end) - time_window;
                end_time = time_elapsed(end);
            else
                start_time = 0;
                end_time = time_window;
            end
            
            % Select only data within the time window
            valid_indices = time_elapsed >= start_time;
            time_plot = time_elapsed(valid_indices);
            values_plot = values(valid_indices, :);
            
            % Update each channel's plot
            for i = 1:num_channels
                if i <= size(values_plot, 2)  % Check if channel exists in data
                    h(i).XData = time_plot;
                    h(i).YData = values_plot(:, i);
                end
            end
            
            % Update X-axis limits
            xlim([start_time end_time]);
            
            % Update Y-axis limits with some padding
            if ~isempty(values_plot)
                y_min = min(values_plot(:));
                y_max = max(values_plot(:));
                y_range = y_max - y_min;
                if y_range == 0  % Handle case where all values are same
                    y_range = 1;
                end
                ylim([y_min - 0.1*y_range, y_max + 0.1*y_range]);
            end
            
            % Update title with latest timestamp
            title(sprintf('Live UDP Data Plot - Last update: %.2f s', time_elapsed(end)));
        end
    catch ME
        % Display error but keep trying
        disp(['Error: ' ME.message]);
    end
    
    % Pause before next update
    pause(update_rate);
end