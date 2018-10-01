function sendSensorInformationToArduino(serial)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    analogueSensors = evalin('base', 'analogueSensors');
    digitalSensors = evalin('base', 'digitalSensors');
    
    for i = 1:size(analogueSensors,2)
        if analogueSensors{i}.OutputVoltage > 0
            % Sensor is properly enabled.
            message = sprintf('A%d 1 %.3f %.3f %.3f\n', i, ...
                analogueSensors{i}.OutputVoltage, ...
                analogueSensors{i}.a, ...
                analogueSensors{i}.b);
            disp('Activated analogue');
        else
            % Sensor is disabled or something else is wrong.
            message = sprintf('A%d 0\n', i);
        end
        fprintf(serial, message);
        pause(0.1);
    end
    
    for i = 1:size(digitalSensors,2)
       % Sensor is properly enabled.
       if digitalSensors{i}.OutputSignal > 0
           message = sprintf('D%d 1 %.3f\n', i, digitalSensors{i}.OutputSignal);
           disp('Activated digital');
       else
           % Sensor is disabled or something else is wrong.
           message = sprintf('D%d 0\n', i);
       end
       fprintf(serial, message);
       pause(0.1);
    end
    %pause(0.1); % Give the arduino enough time to process the input.
    fprintf(serial, 'Q'); % Send the "I'm finished sending' signal.
    disp('Sensors are set.');
end

