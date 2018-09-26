function livePlot(obj, event, app)
    persistent sensorData

    if isempty(sensorData)
       sensorData = evalin('base', 'sensorData'); 
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
        set(app.S1Graph.Children(1), 'XData', xdata, 'YData', ydata(:,1), 'Color', [0, 0.45, 0.74]);
        set(app.S1Graph.Children(2), 'XData', xdata, 'YData', ydata(:,2), 'Color', [0.85, 0.33, 0.10]);
        set(app.S1Graph.Children(3), 'XData', xdata, 'YData', ydata(:,3), 'Color', [0.93, 0.69, 0.13]);
        app.S1Graph.XLim = [min(xdata), max(xdata)+40];
    end
end