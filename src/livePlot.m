function livePlot(obj, event, app)
    persistent sensorData
    persistent sensors
    persistent colourTable
    persistent resetAutoscaleFlow
    persistent resetAutoscalePressure
    
    flowMinscale = 0.5; % L/min
    pressureMinscale = 10; % mmHg
    minScale_timeout = 1; % Timeout to set the graphs back to autoscale.
    %% Initialisation
    if isempty(sensorData)
       sensorData = evalin('base', 'sensorData');
       if app.PressureAutoscaleYSwitch.Value == 'yes'
           app.pressureGraph.YLimMode = 'auto';
       else
           app.pressureGraph.YLimMode = 'manual';
       end
       if app.FlowAutoscaleYSwitch.Value == 'yes'
           app.flowGraph.YLimMode = 'auto';
       else
           app.flowGraph.YLimMode = 'manual';
       end
       app.flowGraph.XLimMode = 'manual';
       app.pressureGraph.XLimMode = 'manual';
       
%        app.flowGraph.YLim = [0, 4150];
    end
    if isempty(sensors)
        tempAnalogue = evalin('base', 'analogueSensors');
        tempDigital = evalin('base', 'digitalSensors');
        sensors = [tempAnalogue tempDigital];
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
    
    if isempty(resetAutoscaleFlow)
        resetAutoscaleFlow = tic;
    end
    if isempty(resetAutoscalePressure)
        resetAutoscalePressure = tic;
    end
    %% Main code
    row = size(sensorData.data,1);
    if row > 0
        if row <= 1500
            xdata = sensorData.data(1:row,2); % row for plotting.
            ydata = sensorData.data(1:row,3:end); % S1 & S2 value
        elseif row > 1500
            xdata = sensorData.data(row-1500:row,2); % row for plotting
            ydata = sensorData.data(row-1500:row,3:end); % S1 & S2 value
        end
        
        fSensors = 1;
        pSensors = 1;
        for it = 1:(size(sensorData.data,2)-2) % All sensors
            plotThisLine = sprintf('plotLines(%d)', it);
            if strcmp(sensors{it}.type, 'flow')
                if evalin('base', plotThisLine)
                    set(app.flowGraph.Children(fSensors), 'XData', xdata, 'YData', ydata(:,it));
                    set(app.flowGraph.Children(fSensors), 'Color', colourTable(it,:));
                else
                    set(app.flowGraph.Children(fSensors), 'XData', [], 'YData', []);
                end
                fSensors = fSensors + 1;
            elseif strcmp(sensors{it}.type, 'pressure')
                if evalin('base', plotThisLine)
                    set(app.pressureGraph.Children(pSensors), 'XData', xdata, 'YData', ydata(:,it));
                    set(app.pressureGraph.Children(pSensors), 'Color', colourTable(it,:));
                else
                    set(app.pressureGraph.Children(pSensors), 'XData', [], 'YData', []);
                end
                pSensors = pSensors + 1;
            else 
                % Sensor is turned off.
            end
        end % for

        app.flowGraph.XLim = [min(xdata), max(xdata)+40];
        app.pressureGraph.XLim = [min(xdata), max(xdata)+40];
        
        if strcmp(app.FlowAutoscaleYSwitch.Value, 'Yes')...
                || strcmp(app.PressureAutoscaleYSwitch.Value, 'Yes') % autoscale is turned on for either graphs.
            elapsed_flow = toc(resetAutoscaleFlow);
            if strcmp(app.FlowAutoscaleYSwitch.Value, 'Yes')
                if elapsed_flow > minScale_timeout
                    ylim(app.flowGraph, 'auto');
                    resetAutoscaleFlow = tic;
                elseif app.flowGraph.YLim(2)-app.flowGraph.YLim(1) < flowMinscale
                    app.flowGraph.YLim(1) = app.flowGraph.YLim(1) - 0.5 * flowMinscale;
                    app.flowGraph.YLim(2) = app.flowGraph.YLim(2) + 0.5 * flowMinscale;
                    resetAutoscaleFlow = tic;
                end
            end
            elapsed_pressure = toc(resetAutoscalePressure);
            if strcmp(app.PressureAutoscaleYSwitch.Value, 'Yes')
                if elapsed_pressure > minScale_timeout 
                    ylim(app.pressureGraph, 'auto');
                    resetAutoscalePressure = tic;
                elseif app.pressureGraph.YLim(2)-app.pressureGraph.YLim(1) < pressureMinscale
                    app.pressureGraph.YLim(1) = app.pressureGraph.YLim(1) - 0.5 * pressureMinscale;
                    app.pressureGraph.YLim(2) = app.pressureGraph.YLim(2) + 0.5 * pressureMinscale;
                    resetAutoscalePressure = tic;
                end
            end
        end % either auto scale switch is turned to yes.
        
    end % row > 0
end % function