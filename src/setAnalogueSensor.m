function success = setAnalogueSensor(sensorNumber, aVal, bVal, type)
%setAnalogueSensor Saves the information about the sensor to the base workspace.
%
%   INPUTS:
%   type        The type of sensor, either 'Analogue' or 'Digital'.
%   aVal        The a value in a*x+b. When sensor is disabled, can be any
%   value. Set value to -1 to disable the sensor.
%   bVal        The b value in a*x+b. When sensor is disabled, can be any
%   value.
%
% RETURNS:
%   success     True when the sensor information is correct. False when one of
%   the inputs is wrong.
persistent Sensors
    success = false;
    if aVal == -1
        Sensors{sensorNumber}.type = type;
        Sensors{sensorNumber}.a = -1;
        Sensors{sensorNumber}.b = -1;
        success = true;
        assignin('base', 'analogueSensors', Sensors);
    elseif aVal == 0
        warning('Rico of analogue sensor %d is 0.', sensorNumber)
    elseif bVal < 0
        warning('B value of analogue sensor %d is not within > 0 V.', sensorNumber)
    else % Values are okay.
        Sensors{sensorNumber}.type = type;
        Sensors{sensorNumber}.a = aVal;  % Rico of 'Rico * x + b'
        Sensors{sensorNumber}.b = bVal;  % b of 'Rico * x + b'
        success = true;
        assignin('base', 'analogueSensors', Sensors);
    end
end

