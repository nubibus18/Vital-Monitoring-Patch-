% CSV file path
CSV_FILE = 'Dheeraj13.csv';

% Read the CSV file completely
data = readmatrix(CSV_FILE);

% Extract timestamps and sensor values
timestamps = data(:, 1) / 1e6; % Convert timestamps from microseconds to seconds
sensor_values = data(:, 2:end); % Extract all sensor columns

% Determine the number of sensors
num_sensors = size(sensor_values, 2);
Fs = 1 / mean(diff(timestamps)); % Estimate sampling frequency

%% Plot Time-Domain Signal
figure;
hold on;
colors = lines(num_sensors);  % Unique colors for each sensor

for i = 1:num_sensors
    plot(timestamps, sensor_values(:, i), 'Color', colors(i, :), 'LineWidth', 1.5, 'DisplayName', ['Sensor ' num2str(i)]);
end

xlabel('Time (s)');
ylabel('Sensor Value');
title('Sensor Data in Time Domain');
grid on;
legend show;
hold off;

%% Compute and Plot FFT (All Sensors in One Figure)
figure;
hold on;

for i = 1:num_sensors
    % Compute FFT
    N = length(sensor_values(:, i));
    Y = fft(sensor_values(:, i));
    f = (0:N-1) * (Fs / N); % Frequency axis

    % Plot FFT
    plot(f(1:N/2), abs(Y(1:N/2)), 'Color', colors(i, :), 'LineWidth', 1.5, 'DisplayName', ['Sensor ' num2str(i)]);
end

xlabel('Frequency (Hz)');
ylabel('Magnitude');
title('FFT of All Sensors');
grid on;
legend show;
hold off;

%% Compute and Plot Welch Power Spectral Density (PSD)
figure;
hold on;

for i = 1:num_sensors
    % Extract sensor data
    sensor_data = sensor_values(:, i);

    % Remove NaN values
    sensor_data = sensor_data(~isnan(sensor_data));

    % Check if the sensor_data is empty after NaN removal
    if isempty(sensor_data)
        warning(['Sensor ' num2str(i) ' has no valid data after NaN removal! Skipping...']);
        continue;
    end

    % Welch PSD estimation
    [pxx, f] = pwelch(sensor_data, [], [], [], Fs);

    % Plot Welch PSD
    plot(f, 10*log10(pxx), 'LineWidth', 1.5, 'DisplayName', ['Sensor ' num2str(i)]);
end

xlabel('Frequency (Hz)');
ylabel('Power/Frequency (dB/Hz)');
title('Welch Power Spectral Density (PSD) of All Sensors');
grid on;
legend show;
hold off;


%% Apply Bandpass Filter (0.5 Hz to 5 Hz)
f_low = 0.5;
f_high = 5;
[b, a] = butter(4, [f_low, f_high] / (Fs / 2), 'bandpass'); % 4th-order Butterworth filter

filtered_values = zeros(size(sensor_values));

for i = 1:num_sensors
    filtered_values(:, i) = filtfilt(b, a, sensor_values(:, i)); % Zero-phase filtering
end

%% Plot Filtered Time-Domain Signal (All Sensors in One Figure)
figure;
hold on;
for i = 1:num_sensors
    plot(timestamps, filtered_values(:, i), 'Color', colors(i, :), 'LineWidth', 1.5, 'DisplayName', ['Filtered Sensor ' num2str(i)]);
end

xlabel('Time (s)');
ylabel('Filtered Sensor Value');
title('Bandpass-Filtered Sensor Data (0.5 Hz to 5 Hz)');
grid on;
legend show;
hold off;


disp('Checking for NaN and Inf values in sensor_values...');
for i = 1:num_sensors
    sensor_data = sensor_values(:, i);
    
    nan_idx = find(isnan(sensor_data));
    inf_idx = find(isinf(sensor_data));

    if ~isempty(nan_idx)
        fprintf('Sensor %d has NaN values at indices: %s\n', i, mat2str(nan_idx));
    end

    if ~isempty(inf_idx)
        fprintf('Sensor %d has Inf values at indices: %s\n', i, mat2str(inf_idx));
    end
end




%% Apply Bandpass Filter (0.5 Hz to 5 Hz)
f_low = 0.5;
f_high = 5;
[b, a] = butter(4, [f_low, f_high] / (Fs / 2), 'bandpass'); % 4th-order Butterworth filter

filtered_values = zeros(size(sensor_values));

for i = 1:num_sensors
    sensor_data = sensor_values(:, i);

    % Replace NaN and Inf values with the mean of valid values
    sensor_data(~isfinite(sensor_data)) = mean(sensor_data(isfinite(sensor_data)), 'omitnan');
    
    % If all values were NaN/Inf, set them to 0
    if all(~isfinite(sensor_data))
        sensor_data(:) = 0;
    end

    % Apply filtering
    filtered_values(:, i) = filtfilt(b, a, sensor_data); % Zero-phase filtering
end

%% Compute and Plot FFT After Filtering (All Sensors in One Figure)
figure;
hold on;

for i = 1:num_sensors
    % Compute FFT of the filtered signal
    N = length(filtered_values(:, i));
    Y_filtered = fft(filtered_values(:, i));
    f = (0:N-1) * (Fs / N); % Frequency axis

    % Plot FFT after filtering
    plot(f(1:N/2), abs(Y_filtered(1:N/2)), 'Color', colors(i, :), 'LineWidth', 1.5, 'DisplayName', ['Filtered Sensor ' num2str(i)]);
end

xlabel('Frequency (Hz)');
ylabel('Magnitude');
title('FFT of Bandpass-Filtered Sensor Data (0.5 Hz to 5 Hz)');
grid on;
legend show;
hold off;

%% Normalize Each Cycle Individually
normalized_values = zeros(size(filtered_values));

for i = 1:num_sensors
    signal = filtered_values(:, i);

    % Find zero-crossings (approximate cycle start points)
    zero_crossings = find(diff(sign(signal)) < 0); % Detect downward zero-crossings

    for j = 1:length(zero_crossings)-1
        % Extract one cycle
        cycle_indices = zero_crossings(j):zero_crossings(j+1);
        cycle_data = signal(cycle_indices);
        
        % Get local min and max
        min_val = min(cycle_data);
        max_val = max(cycle_data);
        
        % Normalize the cycle
        if max_val ~= min_val
            normalized_values(cycle_indices, i) = (cycle_data - min_val) / (max_val - min_val);
        else
            normalized_values(cycle_indices, i) = 0; % Prevent division by zero
        end
    end
end

%% Compute Time Derivative of the Bandpass-Filtered Signal
dt = mean(diff(timestamps)); % Compute the average time step
derivative_values = diff(normalized_values) / dt; % Compute numerical derivative

% Append a zero row to match dimensions
derivative_values = [derivative_values; zeros(1, num_sensors)];

%% Plot Normalized Signal
figure;
hold on;
for i = 3
    plot(timestamps, normalized_values(:, i), 'Color', colors(i, :), 'LineWidth', 1.5, 'DisplayName', ['Normalized Sensor ' num2str(i)]);
end

xlabel('Time (s)');
ylabel('Normalized Sensor Value (0 to 1)');
title('Cycle-Wise Normalized Sensor Data');
grid on;
legend show;
hold off;

%% Plot Time Derivative of Bandpass-Filtered Signal
figure;
hold on;
for i = 3
    plot(timestamps, derivative_values(:, i), 'Color', colors(i, :), 'LineWidth', 1.5, 'DisplayName', ['Derivative Sensor ' num2str(i)]);
end

xlabel('Time (s)');
ylabel('Time Derivative');
title('Time Derivative of Bandpass-Filtered Sensor Data');
grid on;
legend show;
hold off;
