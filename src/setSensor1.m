function success = setSensor1(type, varargin)
%setSensor1 Saves the information about sensor 1 to the base workspace.
%   type The type of sensor, either 'Analogue' or 'Digital'.
%   varargin Expected 3 inputs for analogue sensors: maximum output, a, b
%            (a*x + b)
%            Expected 1 input for digital sensors: number of pulses per
%            unit.
% Returns:
%   success True when the sensor information is correct. False when one of
%   the inputs is wrong.

    success = false;
    if strcmp(type, 'Analogue') && (nargin-1) == 3
        output = varargin{1};
        aVal = varargin{2};
        bVal = varargin{3};
        
        if output <= 0 || output > 3.3
            warning('Maximum output of sensor 1 is not within 0 ... 3.3 V.')
        elseif aVal == 0
            warning('Rico of sensor 1 is 0.')
        elseif bVal <= 0 || bVal > 3.3
            warning('B value of sensor 1 is not within 0 ... 3.3 V.')
        else % Values are okay.
            S1 = {type,
                    varargin{1}, % Maximum output voltage
                    varargin{2}, % Rico of 'Rico * x + b'
                    varargin{3}, % b of 'Rico * x + b'
                    }
            success = true;
            assignin('base', 'S1', S1);
        end
    elseif strcmp(type, 'Digital') && (nargin-1) == 1
        output = varargin{1};
        if output <= 0
            warning('Output of sensor 1 is not valid (>0).')
        else % values are okay
            S1 = { type,
                    varargin{1} % Output in pulses
                    }
            success = true;
            assignin('base', 'S1', S1);
        end
    else % error case
        warning('No correct input given');
    end

end

