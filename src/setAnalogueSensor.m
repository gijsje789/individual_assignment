function success = setAnalogueSensor(sensorNumber, output, aVal, bVal)
%setSensor1 Saves the information about the sensors to the base workspace.
%
%   INPUTS:
%   type        The type of sensor, either 'Analogue' or 'Digital'.
%   output      The maximum output voltage of the sensor. Set output to -1
%   to disable the sensor. 
%   aVal        The a value in a*x+b. When sensor is disabled, can be any
%   value.
%   bVal        The b value in a*x+b. When sensor is disabled, can be any
%   value.
%
% RETURNS:
%   success     True when the sensor information is correct. False when one of
%   the inputs is wrong.
warning on backtrace
persistent Sensors
    success = false;
    if output == -1
        Sensors{sensorNumber}.OutputVoltage = -1;
        Sensors{sensorNumber}.a = 0;
        Sensors{sensorNumber}.b = 0;
        success = true;
        assignin('base', 'Sensors', Sensors);
    elseif output <= 0 || output > 3.3
        warning('Maximum output of sensor %d is not within 0 ... 3.3 V.', sensorNumber)
    elseif aVal == 0
        warning('Rico of sensor %d is 0.', sensorNumber)
    elseif bVal < 0 || bVal > 3.3
        warning('B value of sensor %d is not within 0 ... 3.3 V.', sensorNumber)
    else % Values are okay.
        Sensors{sensorNumber}.OutputVoltage = output; % Maximum output voltage
        Sensors{sensorNumber}.a = aVal;  % Rico of 'Rico * x + b'
        Sensors{sensorNumber}.b = bVal;  % b of 'Rico * x + b'
        success = true;
        assignin('base', 'Sensors', Sensors);
    end
end

