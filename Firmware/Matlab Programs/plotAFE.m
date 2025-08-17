% Load CSV File
filename = 'fifo_data.csv'; % Change if needed
data = readmatrix(filename);

% Extract individual register columns
Register_0x64 = data(:,1);
Register_0x65 = data(:,2);
Register_0x66 = data(:,3);
Register_0x67 = data(:,4);
Register_0x68 = data(:,5);
Register_0x69 = data(:,6);
Register_0x70 = data(:,7);
Register_0x71 = data(:,8);

% Create a time/sample index
samples = 1:length(Register_0x64);

% Sampling frequency (adjust as per your data)
fs = 100;  

% --- 1. Time-Domain Plot ---
figure;
plot(samples, Register_0x64, 'r', 'LineWidth', 1.5); hold on;
plot(samples, Register_0x65, 'g', 'LineWidth', 1.5);
plot(samples, Register_0x66, 'b', 'LineWidth', 1.5);
plot(samples, Register_0x67, 'c', 'LineWidth', 1.5);
plot(samples, Register_0x68, 'm', 'LineWidth', 1.5);
plot(samples, Register_0x69, 'y', 'LineWidth', 1.5);
plot(samples, Register_0x70, 'k', 'LineWidth', 1.5);
plot(samples, Register_0x71, '--', 'LineWidth', 1.5);

% Customize plot
xlabel('Sample Index');
ylabel('Register Value');
title('FIFO Data Plot (Time Domain)');
legend('0x64', '0x65', '0x66', '0x67', '0x68', '0x69', '0x70', '0x71');
grid on;
hold off;

% --- 2. FFT of Original Data ---
n = length(Register_0x64);
f = (0:n-1) * (fs / n); % Frequency axis

% Compute FFT for each register
fft_64 = abs(fft(Register_0x64));
fft_65 = abs(fft(Register_0x65));
fft_66 = abs(fft(Register_0x66));
fft_67 = abs(fft(Register_0x67));
fft_68 = abs(fft(Register_0x68));
fft_69 = abs(fft(Register_0x69));
fft_70 = abs(fft(Register_0x70));
fft_71 = abs(fft(Register_0x71));

% --- 2. Plot FFT of Original Data ---
figure;
plot(f, fft_64, 'r', 'LineWidth', 1.5); hold on;
plot(f, fft_65, 'g', 'LineWidth', 1.5);
plot(f, fft_66, 'b', 'LineWidth', 1.5);
plot(f, fft_67, 'c', 'LineWidth', 1.5);
plot(f, fft_68, 'm', 'LineWidth', 1.5);
plot(f, fft_69, 'y', 'LineWidth', 1.5);
plot(f, fft_70, 'k', 'LineWidth', 1.5);
plot(f, fft_71, '--', 'LineWidth', 1.5);

% Customize FFT plot
xlabel('Frequency (Hz)');
ylabel('Magnitude');
title('FFT of FIFO Data (Original)');
legend('0x64', '0x65', '0x66', '0x67', '0x68', '0x69', '0x70', '0x71');
grid on;
hold off;

% --- 3. Band-Pass Filtering ---
low_cutoff = 0.05;  % Lower cutoff frequency in Hz
high_cutoff = 1.5; % Upper cutoff frequency in Hz
[b, a] = butter(4, [low_cutoff, high_cutoff] / (fs / 2), 'bandpass'); % 4th-order Butterworth filter

% Apply filter to each register
filtered_64 = filtfilt(b, a, Register_0x64);
filtered_65 = filtfilt(b, a, Register_0x65);
filtered_66 = filtfilt(b, a, Register_0x66);
filtered_67 = filtfilt(b, a, Register_0x67);
filtered_68 = filtfilt(b, a, Register_0x68);
filtered_69 = filtfilt(b, a, Register_0x69);
filtered_70 = filtfilt(b, a, Register_0x70);
filtered_71 = filtfilt(b, a, Register_0x71);

% --- 3. Plot Band-Pass Filtered Data ---
figure;
plot(samples, filtered_64, 'r', 'LineWidth', 1.5); hold on;
plot(samples, filtered_65, 'g', 'LineWidth', 1.5);
plot(samples, filtered_66, 'b', 'LineWidth', 1.5);
plot(samples, filtered_67, 'c', 'LineWidth', 1.5);
plot(samples, filtered_68, 'm', 'LineWidth', 1.5);
plot(samples, filtered_69, 'y', 'LineWidth', 1.5);
plot(samples, filtered_70, 'k', 'LineWidth', 1.5);
plot(samples, filtered_71, '--', 'LineWidth', 1.5);

% Customize Band-Pass plot
xlabel('Sample Index');
ylabel('Filtered Register Value');
title(sprintf('Band-Pass Filtered Data (%d Hz - %d Hz)', low_cutoff, high_cutoff));
legend('0x64', '0x65', '0x66', '0x67', '0x68', '0x69', '0x70', '0x71');
grid on;
hold off;

% --- 4. FFT of Filtered Data ---
fft_filt_64 = abs(fft(filtered_64));
fft_filt_65 = abs(fft(filtered_65));
fft_filt_66 = abs(fft(filtered_66));
fft_filt_67 = abs(fft(filtered_67));
fft_filt_68 = abs(fft(filtered_68));
fft_filt_69 = abs(fft(filtered_69));
fft_filt_70 = abs(fft(filtered_70));
fft_filt_71 = abs(fft(filtered_71));

% --- 4. Plot FFT of Filtered Data ---
figure;
plot(f, fft_filt_64, 'r', 'LineWidth', 1.5); hold on;
plot(f, fft_filt_65, 'g', 'LineWidth', 1.5);
plot(f, fft_filt_66, 'b', 'LineWidth', 1.5);
plot(f, fft_filt_67, 'c', 'LineWidth', 1.5);
plot(f, fft_filt_68, 'm', 'LineWidth', 1.5);
plot(f, fft_filt_69, 'y', 'LineWidth', 1.5);
plot(f, fft_filt_70, 'k', 'LineWidth', 1.5);
plot(f, fft_filt_71, '--', 'LineWidth', 1.5);

% Customize FFT of Filtered Data plot
xlabel('Frequency (Hz)');
ylabel('Magnitude');
title(sprintf('FFT of Band-Pass Filtered Data (%d Hz - %d Hz)', low_cutoff, high_cutoff));
legend('0x64', '0x65', '0x66', '0x67', '0x68', '0x69', '0x70', '0x71');
grid on;
hold off;
