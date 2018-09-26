function success = setDigitalSensor(sensorNumber, output, type)
%setDigitalSensor Saves the information about the sensors to the base workspace.
%
%   INPUTS:
%   type        The type of sensor, either 'Analogue' or 'Digital'.
%   output      The maximum output of the sensor in pulses per unit. Set output to -1
%   to disable the sensor. 
%
% RETURNS:
%   success     True when the sensor information is correct. False when one of
%   the inputs is wrong.
warning on backtrace
persistent Sensors
    success = false;
    if output == -1
        Sensors{sensorNumber}.type = type;
        Sensors{sensorNumber}.OutputSignal = -1;
        success = true;
        assignin('base', 'digitalSensors', Sensors);
    elseif output <= 0
        warning('Maximum output of digital sensor %d is not > 0.', sensorNumber)
    else % Values are okay.
        Sensors{sensorNumber}.type = type;
        Sensors{sensorNumber}.OutputSignal = output; % Maximum output in pulses per minute.
        success = true;
        assignin('base', 'digitalSensors', Sensors);
    end


end