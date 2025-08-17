clc; clear; close all;

%% Load CSV File
CSV_FILE = 'Dheeraj13.csv';  % Update with your file
data = readmatrix(CSV_FILE);

% Extract timestamps (convert from microseconds to seconds)
timestamps = data(:, 1) ./ 1e6;  
ppg_signals = data(:, 2:end);  % Extract all 8 register readings

%% Make Sampling Uniform
min_sampling_period = min(diff(timestamps));  % Minimum time gap
new_time = timestamps(1):min_sampling_period:timestamps(end);  % New uniform time axis

% Interpolate all 8 registers to the new uniform timestamps
ppg_resampled = zeros(length(new_time), size(ppg_signals, 2));
for i = 1:size(ppg_signals, 2)
    ppg_resampled(:, i) = interp1(timestamps, ppg_signals(:, i), new_time, 'linear', 'extrap');
end

%% Apply Bandpass Filter (0.5Hz - 5Hz) for Heart Rate Extraction
low_cutoff = 0.5;   % 30 BPM
high_cutoff = 5;    % 300 BPM
fs = 1 / min_sampling_period;  % Estimated sampling frequency
[b, a] = butter(2, [low_cutoff, high_cutoff] / (fs / 2), 'bandpass');

filtered_ppg = filtfilt(b, a, ppg_resampled);  % Apply filter to all registers

%% Peak Detection and Heart Rate Calculation
window_size = 30;  % 30 seconds window
window_samples = round(window_size * fs);  % Convert seconds to samples
heart_rates = NaN(length(new_time), size(filtered_ppg, 2));  % Initialize BPM matrix

for i = 1:size(filtered_ppg, 2)
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
colors = lines(8);

figure;
hold on;
for i = 1:8
    plot(new_time, heart_rates(:, i), 'Color', colors(i, :), 'LineWidth', 1);
end
xlabel('Time (s)'); ylabel('Heart Rate (BPM)');
title('Estimated Heart Rate Over Time (30s Moving Window)');
grid on;
hold off;
