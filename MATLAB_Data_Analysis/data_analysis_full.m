% In this code, it is assumed all CSV files for data analysis are saved in a folder called 'Display Data'. 
% If you are using a different directory name, change it accordingly before using this code.

displaydata = dir("Display Data\");
cd("Display Data\");
analysisfile = readtable(displaydata(17).name, 'NumHeaderLines',1);
cd ..;
voltage = analysisfile(:,3); % 'Extracts' column 3 as a variable
irradiance = analysisfile(:,2); % 'Extracts' column 2 as a variable

% This part of the code 'converts' all 'NaN' values to '0' first
for i = 1 : 720
    for j = 2 : 5
        if isnan(analysisfile{i,j}) % if value is NaN, change it to 0
            analysisfile(i,j) = {0}; 
            voltage(i,:) = {0};
            irradiance(i,:) = {0};
        end
    end
voltage(i,:) = {voltage{i,:} * 6.144 / 2^15}; % Convert ADC to voltage
irradiance(i,:) = {irradiance{i,:} * 6.144 / (2^15 * 0.201e-3)}; % Convert ADC to irradiance
end

s1cal = voltage.Var3 / 0.102e-3; % Irradiance measured by sensor 1,
% calculated using sensitivity value 0.102e-3

% uncomment '%%' if fitBogartz.m is stored in a different directory
subplot(2,2,1); % Piecewise linear analysis
hold on;
%% cd("<Directory Name>") % Enter directory where linear_regression.m is stored
% irradiance.Var2 and voltage.Var3 parses all 720 values in each as an individual number
coefficients = fitBogartz(irradiance.Var2, voltage.Var3, 1);
%% cd ..;
scatter(irradiance.Var2, voltage.Var3, 'r', '.');
refline(coefficients(1),coefficients(2));
refline(coefficients(3),coefficients(4));
title('Sensor Voltage Against Solar Irradiance');
xlabel('Solar Irradiance (W/m^2)');
ylabel('Sensor Voltage (V)');
hold off;

subplot(2,2,2); % Solar irradiance against time
plot(analysisfile.Var1, irradiance.Var2, analysisfile.Var1, s1cal);
title('Solar Irradiance From 7am To 7pm');
xlabel('Time');
ylabel('Solar Irradiance (W/m^2)');
legend('Reference Sensor', 'Sensor 1');

subplot(2,2,3); % Histogram of error between Reference and Sensor 1 measurements
histogram(irradiance.Var2 - s1cal);
title('Histogram of Measurement Errors');

subplot(2,2,4); % Graph of error between Reference and Sensor 1 measurements
plot(analysisfile.Var1, irradiance.Var2 - s1cal);
title('Measurement Errors From 7am To 7pm');
xlabel('Time');
ylabel('Solar Irradiance (W/m^2)');
