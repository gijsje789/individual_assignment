function livePlot(obj, event, app)
    persistent sensorData
    persistent analogueSensors
    persistent digitalSensors
    persistent colourTable

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
    if isempty(colourTable)
        colourTable =     [ 1 0 0;          % A1, Red
                            1 0 1;          % A2, Magenta
                            1 1 0;          % A3, Yellow
                            0 1 0;          % A4, Green
                            0 1 1;          % A5, Cyan
                            0 0 1;          % D1, Blue
                            0 0 0;          % D2, Black
                            0.49 0.18 0.56; % D3, Purple
                            0.93 0.69 0.13; % D4, Orange
                            0.65 0.65 0.65];% D5, Gray
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
        
        fSensors = 1;
        pSensors = 1;
        for it = 1:(size(sensorData.data,2)-2) % All active sensors
            if strcmp(analogueSensors{it}.type, 'flow')
                set(app.flowGraph.Children(fSensors), 'XData', xdata, 'YData', ydata(:,it));
                set(app.flowGraph.Children(fSensors), 'Color', colourTable(it,:));
                fSensors = fSensors + 1;
            elseif strcmp(analogueSensors{it}.type, 'pressure')
                set(app.pressureGraph.Children(pSensors), 'XData', xdata, 'YData', ydata(:,it));
                set(app.flowGraph.Children(fSensors), 'Color', colourTable(it,:));
                pSensors = pSensors + 1;
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