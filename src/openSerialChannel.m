function success = openSerialChannel()
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
    warning on backtrace
    success = true;
    evalin('base', 'arduinoSerial = serial(serialPort)');
    status = evalin('base', 'arduinoSerial.Status');
    if strcmp(status, 'closed')
        evalin('base', 'fopen(arduinoSerial)')
        status = evalin('base', 'arduinoSerial.Status');
        if strcmp(status, 'closed')
            warning('Could not open arduino serial port.');
            success = false;
        elseif strcmp(status, 'open')
            'Opened arduino COM port.'
            evalin('base', 's.BytesAvailableFcnMode = ''terminator'' ');
            evalin('base', 's.BytesAvailableFcn = @readSerialInformation');
        else
            warning('Unknown error whilst opening COM port.');
            success = false;
        end
    elseif strcmp(status, 'open')
        warning('COM port already open, possible undetermined behaviour.');
        success = false;
    else
        warning('Unknown error checking status of COM port.');
        success = false;
    end
end

