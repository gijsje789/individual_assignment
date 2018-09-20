function readSerialInformation(serialPort)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    warning on backtrace
    if strcmp(serialPort.Status, 'closed')
        fopen(serialPort);
        if strcmp(serialPort.Status, 'closed')
            warning('Could not open serial port.');
        end
    elseif strcmp(serialPort.Status, 'open')
        out = fscanf(serialPort)
    else
        warning('Unknown error when reading serial port.');
    end
end

