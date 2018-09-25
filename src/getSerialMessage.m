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
persistent prevTime;

if isempty(app)
    app = evalin('base', 'app');
    app.S1Graph.XLimMode = 'manual';
    app.S1Graph.YLimMode = 'manual';
    app.S1Graph.YLim = [0, 4150];
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
       evalin('base', 'sensorData = handleForData');
       dataHandle = evalin('base', 'sensorData');
   end
end
    warning on backtrace  
    string = fscanf(arduinoSerial);
    row = row + 1;
    data(row,1) = now; % timestamp the first cell.
    data(row,2) = row; % use the row index for plotting.
    if row > 10
        % evaluate the string as three seperate numerical values seperated by a space.
        % Processing time of 0.0004 seconds = 2500Hz.
        data(row, 3:5) = sscanf(string, '%d', [1 3]);
        dataHandle.data(row, :) = data(row, :);
%     end
    %% Plotting
        if toc(prevTime) >= 0.01 %10 ms
            livePlot(app, row, data);
            prevTime = tic;
        end
    end

%     This should be faster than using plot but there is no line being
%     shown. Tried animatedLine but that is really slow.
%     set(app.S1Graph.Children, 'XData', xdata, 'YData', ydata);
%     drawnow
       
%     assignin('base', 'sensorData', data);
end

