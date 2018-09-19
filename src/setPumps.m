function success = setPumps(flowrate)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    success = true;
    if flowrate >= 0 || flowrate <= 8
        assignin('base', 'Pumprate', flowrate);
    else
        success = false;
    end
end

