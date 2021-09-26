hight   = [9.0, 9.5, 10.0, 10.5, 11, 11.5, 12, 10.5, 12.5, 13.0];
percent = [72.1, 71.5, 71, 70.5, 69.5, 69.0, 68.5, 70.8, 68.0, 66.7];

cal_hight = percent.*(-1)+79.7;
cal_altitude = 961.68 - (cal_hight.*0.01);
altitude = 961.68 - (hight.*0.01);

figure(1)
hold on
    plot(percent, hight, 'd');
    plot(percent, cal_hight);
hold off

figure(2)
hold on
    plot(percent, altitude, 'd');
    plot(percent, cal_altitude);
hold off
