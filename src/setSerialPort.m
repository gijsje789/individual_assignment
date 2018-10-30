function success = setSerialPort(port)
%setSerialPort(port) Sets the serial port that is to be used for the
%communication with the arduino in the base workspace.
%   This way, the value entered in the drop down menu of the GUI, is saved
%   to the base workspace such that functions can access it as well.
% 
%   INPUTS:
%   port    Character vector of the COM port that the arduino is connected
%   to.
%
%   RETURNS:
%   success     Returns true when the COM port is valid and set to the base
%   workspace. Otherwise, false.
    success = true;
    if ~isemtpy(strfind(port, 'COM'))
        [num, status] = str2num(erase(port,'COM'));
        if status
            assignin('base', 'serialPort', port);
        else
            warning('COM port might not be correct.\r\n');
            success = false;
        end
    else
        fprintf(2, 'Could not find COM.\r\n');
        success = false;
    end
end

