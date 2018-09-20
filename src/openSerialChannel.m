function success = openSerialChannel()
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
    warning on backtrace
    evalin('base', 'arduinoSerial = serial(serialPort)');
    status = evalin('base', 'arduinoSerial.Status');
    if strcmp(status, 'closed')
        evalin('base', 'fopen(arduinoSerial)')
        status = evalin('base', 'arduinoSerial.Status');
        if strcmp(status, 'closed')
            warning('Could not open arduino serial port.');
        elseif strcmp(status, 'open')
            'Opened arduino COM port.'
            evalin('base', 's.BytesAvailableFcnMode = ''terminator'' ');
            evalin('base', 's.BytesAvailableFcn = @readSerialInformation');
        else
            warning('Unknown error whilst opening COM port.');
        end
    elseif strcmp(status, 'open')
        warning('COM port already open, possible undetermined behaviour.');
    else
        warning('Unknown error checking status of COM port.');
    end
end

