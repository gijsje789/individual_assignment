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

if isempty(app)
    app = evalin('base', 'app');
    app.S1Graph.XLimMode = 'manual';
    app.S1Graph.YLimMode = 'manual';
    app.S1Graph.YLim = [0, 4150];
end
if isempty(row)
    row = 0;
end
if isempty(arduinoSerial)
   arduinoSerial = evalin('base', 'arduinoSerial'); 
end
    tic;
    warning on backtrace  
    string = fscanf(arduinoSerial);
    row = row + 1;
    data(row,1) = now; % timestamp the first cell.
    data(row,2) = row; % use the row index for plotting.
    if row > 10
        data(row, 3:5) = sscanf(string, '%d', [1 3]);
    end
    
    %% Plotting
%         if row <= 100
%             xdata = data(1:row,2); % row for plotting.
%             ydata = data(1:row,3:5); % S1 & S2 value
%             plot(app.S1Graph, xdata, ydata);
%         elseif row > 100
%             xdata = data(row-100:row,2); % row for plotting
%             ydata = data(row-100:row,3:5); % S1 & S2 value
%             set(app.S1Graph.Children(1), 'XData', xdata, 'YData', ydata(:,1), 'Color', [0, 0.45, 0.74]);
%             set(app.S1Graph.Children(2), 'XData', xdata, 'YData', ydata(:,2), 'Color', [0.85, 0.33, 0.10]);
%             set(app.S1Graph.Children(3), 'XData', xdata, 'YData', ydata(:,3), 'Color', [0.93, 0.69, 0.13]);
%         end
%         app.S1Graph.XLim = [min(xdata), max(xdata)+40];
%     end

%     This should be faster than using plot but there is no line being
%     shown. Tried animatedLine but that is really slow.
%     set(app.S1Graph.Children, 'XData', xdata, 'YData', ydata);
%     drawnow
       
    assignin('base', 'sensorData', data);
    toc;
end

