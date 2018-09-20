function out = readSerialInformation()
%readSerialInformation() reads the base workspace 'arduinoSerial' channel
%to receive serial communication from the arduino. Will need to be able to
%read more than just one sensor.
%
% RETURNS:
% out   The character string that is received via the serial communication.
    warning on backtrace
    out = evalin('base', 'fscanf(arduinoSerial)')
end

