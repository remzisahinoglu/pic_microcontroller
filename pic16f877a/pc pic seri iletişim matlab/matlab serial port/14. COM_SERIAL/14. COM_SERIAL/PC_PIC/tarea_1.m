%Programa de ensayo de movimiento de los motores servos HS-311
%ABRIR el puerto COM1
clc; disp('BEGIN')
SerPIC = serial('COM2');
set(SerPIC,'BaudRate',2400);
set(SerPIC,'DataBits',8);
set(SerPIC,'Parity','none');
set(SerPIC,'StopBits',1);
set(SerPIC,'FlowControl','none');
fopen(SerPIC);
%*-*-*-*-*-*-
fprintf(SerPIC,'%s','A');pause(0.2)
%*-*-*-FIN Posición final

%CERRAR el puerto COM1 al finalizar
fclose(SerPIC);
delete(SerPIC)
clear SerPIC
disp('STOP')