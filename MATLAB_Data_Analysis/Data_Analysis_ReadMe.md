Before using the files in this directory, make sure the following is installed in MATLAB:
1. IoT Data Explorer App (https://www.mathworks.com/matlabcentral/fileexchange/77368-iot-data-explorer-for-thingspeak-and-matlab)
2. Piecewise Linear Regression (https://www.mathworks.com/matlabcentral/fileexchange/124190-piecewise-linear-model)

# Using IoT Data Explorer App to extract exactly 12 hours of data
1. After installing the IoT Data Explorer, open MATLAB then open it from the 'Apps' tab. (as shown below)
![image](https://user-images.githubusercontent.com/79315379/218377963-5e463b62-9042-45c0-9193-90758428b899.png)

2. In the IoT Data Explorer window, type in the Channel ID and Start Date. (Read API Key is not required) Set 'Start Hour' to 7am and 'Duration' to 12 hours.
Set 'Compare' to 'None' and hit 'Update'. The following output will be seen.
![image](https://user-images.githubusercontent.com/79315379/218379058-0032685d-1c34-4650-83c8-43e124c80681.png)

3. Select 'Output to Workspace' and 2 variables 'oldData' and 'recentData' will appear in your MATLAB workspace.
![image](https://user-images.githubusercontent.com/79315379/218379913-e503d423-eef7-402d-b80a-453ba305ddcf.png)

4. 'oldData' should be a '720x4 timetable' variable type to signify that there are 720 data points sampled every minute. But if there are less than 720 data points, use insert_data.m to insert 'NaN' data points so that the total data points is 720.

However, insert_data.m may miss out on a few data points so some will have to be keyed in manually.

After this step, you should have a CSV file with 720 data points to use for data analysis.

