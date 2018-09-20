function success = setPumps(flowrate)
%setPumps(flowrate) saves the flowrate set in the GUI to the base
%workspace.
%
% INPUTS:
%   flowrate    The flowrate that is entered in the GUI.
%
% RETURNS:
%   success     Returns true when the pumprate is valid and saved to the
%   base workspace. Otherwise, false.
    warning on backtrace
    success = true;
    if flowrate >= 0 || flowrate <= 8
        assignin('base', 'Pumprate', flowrate);
    else
        warning('Could not set pump rate.');
        success = false;
    end
end

