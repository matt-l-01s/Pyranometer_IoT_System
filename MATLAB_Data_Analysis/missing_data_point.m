% Created by Dr Rodney
Inserted = oldData;
ReferenceSensorADCValue = nan;
Sensor1ADCValue = nan;
Sensor2ADCValue = nan;
Sensor3ADCValue = nan;

dm = str2num(datestr(oldData.Timestamps, 'MM'));

for m=1:height(oldData)-1
    if dm(m+1)-dm(m) == 2 || dm(m+1)-dm(m) == -58 % 2 data points in a row are missing
        TS=datetime(oldData(m,1).Timestamps + minutes(1));
        MissingTT = timetable(TS,ReferenceSensorADCValue,Sensor1ADCValue,Sensor2ADCValue,Sensor3ADCValue);
        Inserted=[Inserted;MissingTT];
    elseif dm(m+1)-dm(m) == 3 % 3 data points in a row are missing
        TS=datetime(oldData(m,1).Timestamps + minutes(1));
        MissingTT = timetable(TS,ReferenceSensorADCValue,Sensor1ADCValue,Sensor2ADCValue,Sensor3ADCValue);
        Inserted=[Inserted;MissingTT];

        TS=datetime(oldData(m,1).Timestamps + minutes(2));
        MissingTT = timetable(TS,ReferenceSensorADCValue,Sensor1ADCValue,Sensor2ADCValue,Sensor3ADCValue);
        Inserted=[Inserted;MissingTT];
    elseif dm(m+1)-dm(m) > 3   %>3
        m; % if 4 data points in a row are missing, the file prints the row number where this occurs
    end
end
IDP=sortrows(Inserted);
%plot(SortedTT,'Timestamps','ReferenceSensorADCValue');
%table2array(a(:,2));
writetimetable(IDP,'20230212_display.csv')
