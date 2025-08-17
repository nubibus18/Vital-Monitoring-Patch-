function AFEprocess()
    % CSV file path
    CSV_FILE = 'Dheeraj7.csv';
    
    % Read CSV file
    data = readmatrix(CSV_FILE);
    
    % Extract timestamps and register values
    timestamps = data(:, 1) ./ 1e6;  % Convert microseconds to seconds
    values = data(:, 2:end);  % Remaining columns are sensor values (Reg1 to Reg8)
    
    % Estimate Sampling Frequency (fs)
    time_diffs = diff(timestamps); 
    fs = 1 / median(time_diffs, 'omitnan');  % Approximate sampling frequency
    
    N = size(values, 1);  % Number of samples
    
    % Compute FFT of Original Data for Each Register
    frequencies = (0:N-1) * (fs / N);  % Frequency axis
    fft_values = abs(fft(values));  % Magnitude of FFT
    
    % Design Band-Pass Filter (0.5 - 5 Hz)
    d = designfilt('bandpassiir', 'FilterOrder', 4, 'HalfPowerFrequency1', 0.5, 'HalfPowerFrequency2', 5, 'SampleRate', fs);
    filtered_values = filtfilt(d, values);  % Apply zero-phase filtering
    
    % Create GUI Figure
    fig = figure('Name', 'AFE Data Visualization', 'NumberTitle', 'off');
    
    % Create Checkboxes for Register Selection
    num_registers = size(values, 2);
    checkboxes = gobjects(num_registers, 1);
    for i = 1:num_registers
        checkboxes(i) = uicontrol('Style', 'checkbox', 'String', ['Reg' num2str(i)], ...
                                  'Position', [20, 400 - (i-1)*30, 100, 30], 'Value', 1);
    end
    
    % Plot Button
    uicontrol('Style', 'pushbutton', 'String', 'Update Plots', ...
              'Position', [20, 50, 100, 30], 'Callback', @updatePlots);
    
    function updatePlots(~, ~)
        % Get Selected Registers
        selected = find(arrayfun(@(x) get(x, 'Value'), checkboxes));
        
        % Define Colors
        colors = lines(num_registers);
        
        % Plot Raw Sensor Data
        figure;
        hold on;
        for i = selected
            plot(timestamps, values(:, i), 'Color', colors(i, :), 'LineWidth', 1.2);
        end
        xlabel('Time (s)'); ylabel('Amplitude'); title('Raw Sensor Data');
        legend(arrayfun(@(x) ['Reg' num2str(x)], selected, 'UniformOutput', false));
        grid on; hold off;
        
        % Plot FFT
        figure;
        hold on;
        for i = selected
            plot(frequencies(1:floor(N/2)), fft_values(1:floor(N/2), i), 'Color', colors(i, :), 'LineWidth', 1.2);
        end
        xlabel('Frequency (Hz)'); ylabel('Magnitude'); title('FFT of Sensor Data');
        legend(arrayfun(@(x) ['Reg' num2str(x)], selected, 'UniformOutput', false));
        grid on; hold off;
        
        % Compute Welch PSD
        figure;
        hold on;
        for i = selected
            [pxx, f] = pwelch(values(:, i), [], [], [], fs);
            plot(f, 10*log10(pxx), 'Color', colors(i, :), 'LineWidth', 1.2);
        end
        xlabel('Frequency (Hz)'); ylabel('Power/Frequency (dB/Hz)'); title('PSD of Sensor Data');
        legend(arrayfun(@(x) ['Reg' num2str(x)], selected, 'UniformOutput', false));
        grid on; hold off;
        
        % Plot Band-Pass Filtered Data
        figure;
        hold on;
        for i = selected
            plot(timestamps, filtered_values(:, i), 'Color', colors(i, :), 'LineWidth', 1.2);
        end
        xlabel('Time (s)'); ylabel('Amplitude'); title('Band-Pass Filtered Data (0.5-5 Hz)');
        legend(arrayfun(@(x) ['Reg' num2str(x)], selected, 'UniformOutput', false));
        grid on; hold off;
    end
end
