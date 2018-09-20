function success = setSensor(type, sensorNumber, varargin)
%setSensor1 Saves the information about the sensors to the base workspace.
%
%   INPUTS:
%   type        The type of sensor, either 'Analogue' or 'Digital'.
%   varargin    Expected 3 inputs for analogue sensors: maximum output, a, b
%            (a*x + b)
%            Expected 1 input for digital sensors: number of pulses per
%            unit.
% RETURNS:
%   success     True when the sensor information is correct. False when one of
%   the inputs is wrong.
warning on backtrace
persistent Sensors
    success = false;
    if sensorNumber > 0 && sensorNumber <= 2
        if strcmp(type, 'Analogue') && (nargin-2) == 3
            output = varargin{1};
            aVal = varargin{2};
            bVal = varargin{3};

            if output <= 0 || output > 3.3
                warning('Maximum output of sensor %d is not within 0 ... 3.3 V.', sensorNumber)
            elseif aVal == 0
                warning('Rico of sensor %d is 0.', sensorNumber)
            elseif bVal < 0 || bVal > 3.3
                warning('B value of sensor %d is not within 0 ... 3.3 V.', sensorNumber)
            else % Values are okay.
                Sensors{sensorNumber} = {type; ...
                        varargin{1}; ... % Maximum output voltage
                        varargin{2}; ... % Rico of 'Rico * x + b'
                        varargin{3}  ... % b of 'Rico * x + b'
                        }
                success = true;
                assignin('base', 'Sensors', Sensors);
            end
        elseif strcmp(type, 'Digital') && (nargin-2) == 1
            output = varargin{1};
            if output <= 0
                warning('Output of sensor %d is not valid (>0).', sensorNumber)
            else % values are okay
                Sensors{sensorNumber} = { type; ...
                        varargin{1} ... % Output in pulses
                        }
                success = true;
                assignin('base', 'Sensors', Sensors);
            end
        else % error case
            warning('No correct input given');
        end
    end

end

