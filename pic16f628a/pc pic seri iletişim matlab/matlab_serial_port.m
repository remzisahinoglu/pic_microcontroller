% seri porttan pic 16f877a ya veri g�nderme
disp('BASLA')

SerPIC = serial('COM3');
set(SerPIC, 'BaudRate', 9600);
set(SerPIC, 'DataBits', 8);
set(SerPIC, 'Parity', 'none');
set(SerPIC, 'StopBits', 1);
set(SerPIC, 'FlowControl', 'none');
fopen(SerPIC);

x0 = -30.15;
%VER�Y� G�NDER
fprintf(SerPIC, '%f', x0);

pause(0.2)

%VER�Y� AL

fclose(SerPIC);
delete(SerPIC)
clear SerPIC
disp('DUR')