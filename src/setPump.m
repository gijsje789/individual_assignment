function success = setPump(pumpNumber, flowrate, feedbackSensor)
%setPumps(flowrate) saves the flowrate set in the GUI to the base
%workspace.
%
% INPUTS:
%   flowrate    The flowrate that is entered in the GUI.
%
% RETURNS:
%   success     Returns true when the pumprate is valid and saved to the
%   base workspace. Otherwise, false.
    persistent Pumps
    success = false;
    if flowrate == 0
        Pumps{pumpNumber}.flowRate = flowrate;
        Pumps{pumpNumber}.feedbackSensor = '';
        success = true;
        assignin('base', 'pumps', Pumps);
    elseif flowrate >= 0 && flowrate <= 8
        switch feedbackSensor
            case {'S1', 'S2', 'S3', 'S4', 'S5',...
                    'D1', 'D2', 'D3', 'D4', 'D5'}
                Pumps{pumpNumber}.flowRate = flowrate;
                Pumps{pumpNumber}.feedbackSensor = feedbackSensor;
                success = true;
                assignin('base', 'pumps', Pumps);
            otherwise
                success = false;
                warning('No proper feedback sensor is selected for pump %d.', pumpNumber);
                Pumps{pumpNumber}.flowRate = 0;
                Pumps{pumpNumber}.feedbackSensor = '';
        end
    else
        warning('Could not set pump %d''s parameters.', pumpNumber);
        success = false;
        Pumps{pumpNumber}.flowRate = 0;
                Pumps{pumpNumber}.feedbackSensor = '';
    end
end

