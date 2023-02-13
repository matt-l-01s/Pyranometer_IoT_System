displaydata = dir("Display Data\");
cd("Display Data\");
for i = 3 : length(displaydata)
  displayfile = readtable(displaydata(i).name, 'NumHeaderLines',1);
   % Converts timestamps in CSV file to 'datetime' variable
  displayfile.Var1 = datetime(displayfile.Var1, 'InputFormat','dd/M/yyyy H:mm:ss');

  subplot(2,7,i - 2);
  plot(displayfile.Var1, displayfile.Var3, displayfile.Var1, displayfile.Var4);
  title('Daily ADC Reading');
  xlabel('');
  ylabel('');
  legend('Reference Sensor', 'Sensor 1');
end

cd ..;
