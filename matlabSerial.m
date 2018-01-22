drawing = 1;

if ~exist('s1','var')
    s1 = serial('/dev/ttyUSB0','baudrate',9600);
end

if strcmp(s1.Status,'closed')
    fopen(s1);
end

% centre
xc = 7.1447;
yc = 8.25;
zc = 10;
r = 21.5;

% columns coords
x1 = 0;
y1 = 0;
x2 = 0;
y2 = 16.5;
x3 = 14.2894;
y3 = 8.25;

% engines positions
z1 = zc + sqrt(r^2 - (x1-xc)^2 - (y1-yc)^2 );
z2 = zc + sqrt(r^2 - (x2-xc)^2 - (y2-yc)^2 );
z3 = zc + sqrt(r^2 - (x3-xc)^2 - (y3-yc)^2 );

if drawing
    figure(1)
    cla;

    % columns
    col1 = line([x1 x1], [y1 y1], [0 60]);
    col2 = line([x2 x2], [y2 y2], [0 60]);
    col3 = line([x3 x3], [y3 y3], [0 60]);

    % arms
    arm1 = line([x1 xc], [y1 yc], [z1 zc]);
    arm2 = line([x2 xc], [y2 yc], [z2 zc]);
    arm3 = line([x3 xc], [y3 yc], [z3 zc]);

    view(18,36)

    hold on
end

% mini animka
for t=0:0.05:40
    % remember last position
    z1l = z1;
    z2l = z2;
    z3l = z3;
    
    % centre
    xc = 5 + sin(t);
    yc = 11 + cos(t);
    zc = 15 + 0.1*t;
    
    if drawing
        plot3(xc,yc,zc,'.')
    end
    
    % engines positions
    z1 = zc + sqrt(r^2 - (x1-xc)^2 - (y1-yc)^2 );
    z2 = zc + sqrt(r^2 - (x2-xc)^2 - (y2-yc)^2 );
    z3 = zc + sqrt(r^2 - (x3-xc)^2 - (y3-yc)^2 );
    
    % calculate difference
    if t>0
       z1dif = z1 - z1l;
       z2dif = z2 - z2l;
       z3dif = z3 - z3l;
       
       % first engine
       if z1dif < 0 
            tekst1 = ['xd' num2str(round(z1dif*-797))];
       else
           tekst1 = ['xu' num2str(round(z1dif*797))];
       end
       
       % second engine
       if z2dif < 0 
            tekst2 = ['yd' num2str(round(z2dif*-797))];
       else
           tekst2 = ['yu' num2str(round(z2dif*797))];
       end
       
       % third engine
       if z3dif < 0 
            tekst3 = ['zd' num2str(round(z3dif*-797))]; 
       else
           tekst3 = ['zu' num2str(round(z3dif*797))]; 
       end
             
       % send commands to printer
       
       while(s1.BytesAvailable == 0)
           ;
       end

       while(s1.BytesAvailable)
           fscanf(s1);
       end
       
       
       fprintf(s1,tekst1)
       fprintf(s1,tekst2)
       fprintf(s1,tekst3)
       
    end   
    
    if drawing
    % update arms
    set(arm1,'xdata',[x1 xc],'ydata',[y1 yc],'zdata',[z1 zc])
    set(arm2,'xdata',[x2 xc],'ydata',[y2 yc],'zdata',[z2 zc])
    set(arm3,'xdata',[x3 xc],'ydata',[y3 yc],'zdata',[z3 zc])
    
    axis([0 60 0 60 0 60])
    axis equal
    pause(.2)
    end

end




