function success = setPumps(flowrate)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    warning on backtrace
    success = true;
    if flowrate >= 0 || flowrate <= 8
        assignin('base', 'Pumprate', flowrate);
    else
        warning('Could not set pump rate.');
        success = false;
    end
end

