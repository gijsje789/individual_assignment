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
   fileID = evalin('base', 'outFile');
end
if isempty(init)
   init = true;
   counter = 1;
end

    string = fscanf(arduinoSerial);
    if init
        if strfind(string, 'Done.') == 1
            init = false;
            disp('Sensors have been set.');
        end
    else
        row = row + 1;
        data(row,1) = now; % timestamp the first cell.
        data(row,2) = row; % use the row index for plotting.
        if row > 10
            % evaluate the string as three seperate numerical values seperated by a space.
            % Processing time of 0.0004 seconds = 2500Hz.
            data(row, 3:5) = sscanf(string, '%d', [1 3]);
            dataHandle.data(row, :) = data(row, :);

            % Immediately write to file (dangerous when message frequency is
            % too high, currently set to 100Hz in arduino.
            % fprintf processing time of 0.0002 seconds = 5000Hz.
            fprintf(fileID, '%d,%d,%d,%d,%d\r\n', data(row,:));

            % Theoretical communication speed: 1000Hz (safety 500Hz).
        end        
    end
end

