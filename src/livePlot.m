function livePlot(app, row, data)
        if row <= 100
            xdata = data(1:row,2); % row for plotting.
            ydata = data(1:row,3:5); % S1 & S2 value
            plot(app.S1Graph, xdata, ydata);
        elseif row > 100
            xdata = data(row-100:row,2); % row for plotting
            ydata = data(row-100:row,3:5); % S1 & S2 value
            set(app.S1Graph.Children(1), 'XData', xdata, 'YData', ydata(:,1), 'Color', [0, 0.45, 0.74]);
            set(app.S1Graph.Children(2), 'XData', xdata, 'YData', ydata(:,2), 'Color', [0.85, 0.33, 0.10]);
            set(app.S1Graph.Children(3), 'XData', xdata, 'YData', ydata(:,3), 'Color', [0.93, 0.69, 0.13]);
        end
        app.S1Graph.XLim = [min(xdata), max(xdata)+40];
end