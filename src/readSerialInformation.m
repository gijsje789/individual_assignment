function out = readSerialInformation()
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    warning on backtrace
    out = evalin('base', 'fscanf(arduinoSerial)')
end

