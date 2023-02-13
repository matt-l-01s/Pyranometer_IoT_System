% Using fitbogartz function from https://www.mathworks.com/matlabcentral/fileexchange/124190-piecewise-linear-model
% Download from the link and extract all files to same directory as this file

analysisfile = readmatrix('20230113_analysis.csv'); % Change to name of csv file 
cd("Display Data\");
timestamp = readtable('20230113_display.csv', 'NumHeaderLines',1);
cd .. ;
cd("MATLAB Files from Dr Rodney\")
coefficients = fitBogartz(analysisfile(:,11), analysisfile(:,8), 1);

scatter(analysisfile(:,11), analysisfile(:,8), 'r', '.');
refline(coefficients(1),coefficients(2));
refline(coefficients(3),coefficients(4));
%refline(0.115e-3,0);
%refline(0.095e-3,5e-3);
title('Sensor Voltage Against Solar Irradiance');
xlabel('Solar Irradiance (W/m^2)');
ylabel('Sensor Voltage (V)');
