function success = setSerialPort(port)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
    warning on backtrace
    success = true;
    if contains(port, 'COM')
        [num, status] = str2num(erase(port,'COM'));
        if status
            assignin('base', 'serialPort', port);
        else
            warning('COM port might not be correct.');
            success = false;
        end
    else
        warning('Could not find COM.');
        success = false;
    end
end

