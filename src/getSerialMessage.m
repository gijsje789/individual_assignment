function getSerialMessage(hObject, eventdata)
%getSerialMessage() reads the base workspace 'arduinoSerial' channel
%to receive serial communication from the arduino. Will need to be able to
%read more than just one sensor.
%
% RETURNS:
% out   The character string that is received via the serial communication.
persistent row;
persistent data;
persistent app;

if isempty(app)
    app = evalin('base', 'app');
    app.S1Graph.XLimMode = 'manual';
    app.S1Graph.YLimMode = 'manual';
    app.S1Graph.YLim = [0, 4150];
end
if isempty(row)
    row = 0;
end

    warning on backtrace
    string = evalin('base', 'fscanf(arduinoSerial)');
    splitted = strsplit(string, ',')
    placeIn = -1;
    row = row + 1;
    data(row,1) = now; % timestamp the first cell.
    data(row,2) = row; % use the row index for plotting.
    for i=1:size(splitted,2)
        if isempty(str2num(splitted{i}))
            % not a number.
            switch splitted{i}
                case 'S1'
                    placeIn = 1;
                case 'S2'
                    placeIn = 2;
                case 'S3'
                    placeIn = 3;
            end 
        elseif ~isempty(str2num(splitted{i}))
            if placeIn ~= -1
                data(row,placeIn+2) = str2num(splitted{i});
            else
                warning('Missing sensor information.');
            end
        else
            warning('Unknown error occurred.');
        end
    end
    
    if row <= 100 && row >= 10
        xdata = data(1:row,2); % row for plotting.
        ydata = data(1:row,3:4); % S1 & S2 value
        plot(app.S1Graph, xdata, ydata);
        app.S1Graph.XLim = [min(xdata), max(xdata)+40];
    elseif row > 100
        xdata = data(row-100:row,2); % row for plotting
        ydata = data(row-100:row,3:4); % S1 & S2 value
        plot(app.S1Graph, xdata, ydata);
        app.S1Graph.XLim = [min(xdata), max(xdata)+40];
    end


%     This should be faster than using plot but there is no line being
%     shown. Tried animatedLine but that is really slow.
%     set(app.S1Graph.Children, 'XData', xdata, 'YData', ydata);
%     drawnow
       
    assignin('base', 'sensorData', data);
end

