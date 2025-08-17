clc; clear; close all;

%% Load CSV File
CSV_FILE = 'Dheeraj13.csv';  % Update with your file
data = readmatrix(CSV_FILE);

% Extract timestamps (convert from microseconds to seconds)
timestamps = data(:, 1) ./ 1e6;  
ppg_signals = data(:, 2:end);  % Extract all sensor readings (any number of columns)

[num_samples, num_sensors] = size(ppg_signals);  % Detect number of sensors

%% Remove Duplicate Timestamps
[unique_timestamps, unique_idx] = unique(timestamps, 'stable');  % Keep first occurrence
ppg_signals = ppg_signals(unique_idx, :);  % Keep only unique timestamps

%% Make Sampling Uniform
min_sampling_period = min(diff(unique_timestamps));  % Minimum time gap
new_time = unique_timestamps(1):min_sampling_period:unique_timestamps(end);  % New uniform time axis

% Interpolate all sensor columns to the new uniform timestamps
ppg_resampled = zeros(length(new_time), num_sensors);
for i = 1:num_sensors
    ppg_resampled(:, i) = interp1(unique_timestamps, ppg_signals(:, i), new_time, 'linear', 'extrap');
end

%% Apply Bandpass Filter (0.5Hz - 5Hz) for Heart Rate Extraction
low_cutoff = 0.5;   % 30 BPM
high_cutoff = 5;    % 300 BPM
fs = 1 / min_sampling_period;  % Estimated sampling frequency
[b, a] = butter(2, [low_cutoff, high_cutoff] / (fs / 2), 'bandpass');

filtered_ppg = filtfilt(b, a, ppg_resampled);  % Apply filter to all sensor signals

%% Peak Detection and Heart Rate Calculation
window_size = 10;  % 30 seconds window
window_samples = round(window_size * fs);  % Convert seconds to samples
heart_rates = NaN(length(new_time), num_sensors);  % Initialize BPM matrix

for i = 1:num_sensors
    % Find Peaks in Filtered Signal (PPG waveform)
    [~, peak_locs] = findpeaks(filtered_ppg(:, i), 'MinPeakDistance', fs / 2);
    
    if length(peak_locs) > 1  % Ensure at least two peaks exist
        peak_intervals = diff(new_time(peak_locs));  % Time between peaks
        bpm = 60 ./ peak_intervals;  % Convert to beats per minute
        
        % Use interpolation only if enough data points exist
        if length(bpm) > 1
            interpolated_bpm = interp1(new_time(peak_locs(2:end)), bpm, new_time, 'linear', 'extrap');
        else
            interpolated_bpm = NaN(size(new_time));  % Not enough peaks, fill with NaN
        end
        
        % Moving average of BPM over 30s window
        heart_rates(:, i) = movmean(interpolated_bpm, window_samples, 'omitnan');
    end
end

%% Plot Heart Rate over Time for All Registers
colors = lines(num_sensors);

figure;
hold on;
for i = 1:num_samples
    plot(new_time, heart_rates(:, i), 'Color', colors(i, :), 'LineWidth', 1, 'DisplayName', ['Sensor ', num2str(i)]);
end
xlabel('Time (s)'); ylabel('Heart Rate (BPM)');
title('Estimated Heart Rate Over Time (30s Moving Window)');
legend show;
grid on;
hold off;
