function getSerialMessage(hObject, eventdata)
%getSerialMessage() reads the base workspace 'arduinoSerial' channel
%to receive serial communication from the arduino. Will need to be able to
%read more than just one sensor.
%
% RETURNS:
% out   The character string that is received via the serial communication.
% tic;
persistent row;
persistent data;
persistent app;
persistent arduinoSerial;
persistent dataHandle;
persistent fileID;
persistent init;

if isempty(app)
    app = evalin('base', 'app');
end
if isempty(row)
    row = 0;
    prevTime = tic;
end
if isempty(arduinoSerial)
   arduinoSerial = evalin('base', 'arduinoSerial'); 
end
if isempty(dataHandle)
   if evalin('base', 'exist(''sensorData'')')
       dataHandle = evalin('base', 'sensorData');
   else
       warning('dataHandle not successfully made.');
   end
end
if isempty(fileID)
    if evalin('base', 'exist(''outFile'')')
        fileID = evalin('base', 'outFile');
    else
        fileID = -1;
    end
end
if isempty(init)
   init = true;
end

    string = fscanf(arduinoSerial);
    row = row + 1;
    data(row,1) = now; % timestamp the first cell.
    data(row,2) = row; % use the row index for plotting.
    if row > 10
        % evaluate the string as three seperate numerical values seperated by a space.
        % Processing time of 0.0004 seconds = 2500Hz.
        data(row, 3:12) = sscanf(string, '%f', [1 10]);
        dataHandle.data(row, :) = data(row, :);

        if fileID ~= -1
            % Immediately write to file (dangerous when message frequency is
            % too high, currently set to 100Hz in arduino.
            % fprintf processing time of 0.0002 seconds = 5000Hz.
            fprintf(fileID, '%d,%d,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\r\n', data(row,:));
        end

        % Theoretical communication speed: 1000Hz (safety 500Hz).
    end
end

