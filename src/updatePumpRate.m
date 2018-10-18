function updatePumpRate(pump, rate, feedback)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
    status = evalin('base', 'exist(''arduinoSerial'');');
    if status
        serial = evalin('base', 'arduinoSerial');
        update = sprintf('pumps{%d}.flowRate = %.10f;', pump, rate);
        evalin('base', update);
        update = sprintf('pumps{%d}.feedbackSensor = ''%s'';', pump, feedback);
        evalin('base', update);

        message = sprintf('P%d 1 %.10f %s\n', pump, rate, feedback)
        fprintf(serial, message);
%         pause(0.1);
    else
        warning('Cannot update pump rate. Serial channel does not exist.');
    end
    

end

