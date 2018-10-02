function sendArduinoResetSignal()
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    status = evalin('base', 'exist(''arduinoSerial'')');
    if status
        serial = evalin('base', 'arduinoSerial');
        if strcmp(serial.Status, 'open')
            disp('Sending arduino reset signal');
            message = sprintf('R\n');
            fprintf(serial, message);
            pause(0.5);
        else
            fprintf(2, 'Error: serial is closed.');
        end
    else
        fprintf(2, 'Error: cannot send reset signal.');
    end
end

