function success = openSerialChannel(app)
% openSerialChannel() opens the serial channel to which the arduino is
% connected in the base workspace.
%   In order to save the incoming data, the base workspace is used for the
%   serial communication and thus requires the 'evalin' function.
%   The main reason is that the GUI and seperate functions all have their
%   own workspace that are not accessable between functions/GUI. Luckily,
%   all functions can, with a small workaround, access the base workspace.
%
% RETURNS:
%   success     Returns true when the serial channel has been opened
%   successfully. Otherwise, false.
    success = true;
    evalin('base', 'arduinoSerial = serial(serialPort);');
    status = evalin('base', 'arduinoSerial.Status');
    if strcmp(status, 'closed')
        evalin('base', 'arduinoSerial.BytesAvailableFcnMode = ''terminator''; ');
        evalin('base', 'arduinoSerial.BytesAvailableFcn = @getSerialMessage;')
        evalin('base', 'arduinoSerial.BaudRate = 115200;');
            
        evalin('base', 'fopen(arduinoSerial);')
        status = evalin('base', 'arduinoSerial.Status');
        if strcmp(status, 'closed')
            warning('Could not open arduino serial port.');
            success = false;
        elseif strcmp(status, 'open')
            disp('Opened arduino COM port.');
            sendParametersToArduino(evalin('base', 'arduinoSerial'));
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

