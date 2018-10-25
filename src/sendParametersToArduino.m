function sendParametersToArduino(serial)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    analogueSensors = evalin('base', 'analogueSensors');
    digitalSensors = evalin('base', 'digitalSensors');
    pumps = evalin('base', 'pumps');
    
    flushinput(serial);
    flushoutput(serial);
    pause(1); % delay to ensure that the first message going to the arduino is good.
    
    for i = 1:size(analogueSensors,2)
        if analogueSensors{i}.a > 0
            % Sensor is properly enabled.
            message = sprintf('A%d 1 %.10f %.10f\n', i, ...
                analogueSensors{i}.a, ...
                analogueSensors{i}.b);
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
           message = sprintf('D%d 1 %.10f\n', i, digitalSensors{i}.OutputSignal);
       else
           % Sensor is disabled or something else is wrong.
           message = sprintf('D%d 0\n', i);
       end
       fprintf(serial, message);
       pause(0.1);
    end
    
    for i = 1:size(pumps,2)
       if pumps{i}.flowRate > 0
           message = sprintf('P%d 1 %.10f %s\n', i, pumps{i}.flowRate, pumps{i}.feedbackSensor);
       else
           message = sprintf('P%d 0', i);
       end
       fprintf(serial, message);
       pause(0.1);
    end
    
    for i = 1:4
        if pumps{i}.flowRate > 0
            if pumps{i}.flowRate > 1.5
                Kp = 0.01; Ki = 0.0025; Kd = 1; % Low-pass, high flow controller
            else
                Kp = 0.01; Ki = 0.00125; Kd = 0; % Low-pass, low flow controller
            end
            message = sprintf('C%d 1 %.10f %.10f %.10f\n', i, Kp, Ki, Kd);
        else
            message = sprintf('C%d 0', i);
        end
        fprintf(serial, message);
        pause(0.1);
    end
    
    pause(0.1); % Give the arduino enough time to process the input.
    message = sprintf('Q\n');
    fprintf(serial, message); % Send the "I'm finished sending' signal.
    disp('Sensors are set.');
end

