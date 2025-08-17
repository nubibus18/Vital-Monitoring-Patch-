% CSV file path
CSV_FILE = 'Dheeraj.csv';

% Read CSV file
data = readmatrix(CSV_FILE);

% Extract timestamps and register values
timestamps = data(:, 1)./1000000;  % Convert timestamps to seconds
values = data(:, 2:end);  % Sensor values (Registers)

% Estimate Sampling Frequency (fs)
time_diffs = diff(timestamps); 
fs = 1 / median(time_diffs, 'omitnan');  % Compute median sampling rate

% Select Register 7 data
reg7_values = values(:, 1);

% Compute Welch’s Power Spectral Density (PSD)
[pxx, f] = pwelch(reg7_values, [], [], [], fs);

% Find peaks in PSD
[peaks, peak_frequencies] = findpeaks(10*log10(pxx), f, 'MinPeakProminence', 3);

% Filter peaks below 10 Hz
valid_idx = peak_frequencies < 7;
filtered_frequencies = peak_frequencies(valid_idx);
filtered_amplitudes = sqrt(10.^(peaks(valid_idx) / 10)); % Convert dB to linear scale

% Time vector for reconstruction
T = max(timestamps) - min(timestamps); % Total duration
t = linspace(0, T, length(reg7_values)); % Time vector with same length as original

% Reconstruct signal using sum of sinusoids
reconstructed_signal = zeros(size(t));

for i = 1:length(filtered_frequencies)
    reconstructed_signal = reconstructed_signal + filtered_amplitudes(i) * sin(2 * pi * filtered_frequencies(i) * t);
end

% Plot reconstructed signal
figure;
plot(t, reconstructed_signal, 'r', 'LineWidth', 1.2);
xlabel('Time (s)');
ylabel('Amplitude');
title('Reconstructed Signal from PSD Peaks (Below 10 Hz)');
grid on;
legend('Reconstructed Signal');

% Display peak frequencies and their power
disp('Reconstructed Signal Uses These Frequencies (Hz) and Corresponding Power:');
disp(table(filtered_frequencies, peaks(valid_idx)));
