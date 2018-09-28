function livePlot(obj, event, app)
    persistent sensorData
    persistent analogueSensors
    persistent digitalSensors

    if isempty(sensorData)
       sensorData = evalin('base', 'sensorData');
       app.flowGraph.XLimMode = 'manual';
       app.flowGraph.YLimMode = 'auto';
       app.pressureGraph.XLimMode = 'manual';
       app.pressureGraph.YLimMode = 'auto';
%        app.flowGraph.YLim = [0, 4150];
    end
    if isempty(analogueSensors)
        analogueSensors = evalin('base', 'analogueSensors');
    end
    if isempty(digitalSensors)
        digitalSensors = evalin('base', 'digitalSensors');
    end
    
    row = size(sensorData.data,1);
    if row > 0
        if row <= 1500
            xdata = sensorData.data(1:row,2); % row for plotting.
            ydata = sensorData.data(1:row,3:5); % S1 & S2 value
        elseif row > 1500
            xdata = sensorData.data(row-1500:row,2); % row for plotting
            ydata = sensorData.data(row-1500:row,3:5); % S1 & S2 value
        end
        
        anSensors = 1;
        digSensors = 1;
        for it = 1:(size(sensorData.data,2)-2) % All active sensors
            if strcmp(analogueSensors{it}.type, 'flow')
                set(app.flowGraph.Children(anSensors), 'XData', xdata, 'YData', ydata(:,it));
                anSensors = anSensors + 1;
            elseif strcmp(analogueSensors{it}.type, 'pressure')
                set(app.pressureGraph.Children(digSensors), 'XData', xdata, 'YData', ydata(:,it));
                digSensors = digSensors + 1;
            else 
            end
        end
%         set(app.flowGraph.Children(1), 'XData', xdata, 'YData', ydata(:,1), 'Color', [0, 0.45, 0.74]);
%         set(app.flowGraph.Children(2), 'XData', xdata, 'YData', ydata(:,2), 'Color', [0.85, 0.33, 0.10]);
%         set(app.flowGraph.Children(3), 'XData', xdata, 'YData', ydata(:,3), 'Color', [0.93, 0.69, 0.13]);
        app.flowGraph.XLim = [min(xdata), max(xdata)+40];
        app.pressureGraph.XLim = [min(xdata), max(xdata)+40];
    end
end