function out = readSerialInformation()
%readSerialInformation() reads the base workspace 'arduinoSerial' channel
%to receive serial communication from the arduino. Will need to be able to
%read more than just one sensor.
%
% RETURNS:
% out   The character string that is received via the serial communication.
persistent row;
persistent data;
if isempty(row)
    row = 0;
end
    warning on backtrace
    string = evalin('base', 'fscanf(arduinoSerial)');
    splitted = strsplit(string, ',');
    placeIn = -1;
    row = row + 1;
    data(row,1) = now; % timestamp the first cell.
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
                data(row,placeIn+1) = str2num(splitted{i});
            else
                warning('Missing sensor information.');
            end
        else
            warning('Unknown error occurred.');
        end
    end
    assignin('base', 'sensorData', data);
end

