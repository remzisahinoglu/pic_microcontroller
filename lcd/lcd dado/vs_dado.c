////////////////////////////////////////////////////////////////////////////////////
//							VsZeNeR'05		
//						   08/Septiembre/05
//						vszener@gmail.com
//	Programa:	Dado digital con lcd y buzzer
//	Version:	0.0
//
//	Dispositivo: PIC 16F648A			Compilador:	 CCS vs3.227
//	Entorno IDE: MPLAB IDE v7.21		Simulador:	 Proteus 6.7sp3
//
//	Notas: Dado digital, que al presionar el boton conectado al pin A0 del porta
//		   genera un numero pseudo-aleatorio mediante la funcion rand() que se
//		   encuentra en la libreria STDLIB.H. El numero pseudo-aleatorio es elegido
//		   mediante la funcion rand() y segun los 'rebotes' producidos por el boton.Tener
//		   en cuenta que hay que poner la directiva NOLVP para que el pin B4 sea de 
//		   salida. Cuando agregamos un boton a nuestro circuito hay que tener en cuenta
//		   que este dispositivo genera 'rebotes' que hay que ser eliminados para
//		   una correcta visualizacion en la lcd del digito seleccionado. Esta vez
//		   la eliminacion de 'los rebotes' se ha realizado mediante software.
//
//	Conexiones:		A0 -> boton		      
//					B0 -> E		      
//					B1 -> RS			
//					B2 -> RW		
//					B4 -> D4		  
//					B5 -> D5            
//					B6 -> D6	
//					B7 -> D7 
//					B3 -> Buzzer(beep)  
//////////////////////////////////////////////////////////////////////////////////

#include <16f648a.h>				//pic a utilizar
#define RAND_MAX  7					//dado solamente 6 numeros
#include <STDLIB.H>					//libreria donde esta la funcion rand(); 			
#fuses XT,NOWDT,NOPROTECT,PUT,NOLVP		//ordenes para el programador 
#use delay (clock=4000000)			//Fosc=4Mhz
#use standard_io(B)
#use fast_io(A)
#define use_portb_lcd TRUE			//definir portb lcd
#include<lcd.c>						//libreria manejo lcd
		
///PROGRAMA
void main(void)
{
	char num=0;						//variable almacena numero aleatorio

	set_tris_a(0xFF);				//porta como entrada	
	disable_interrupts(GLOBAL);		//todas las interrupciones desactivadas
	
	lcd_init();						//inicializa lcd
	
	srand(10);						//maximo hasta 9
	lcd_putc("Dado Electronico\nPulse boton...");
	
	while(input(PIN_A0)){}			//Se mantiene cabecera hasta que se pulse el boton
		
	lcd_putc("\fDado:\n   VsZeNeR'05");
	lcd_gotoxy(7,1);				//Coordenadas dond se muestra el numero
	
	for(;;){						//bucle...
		if(!input(PIN_A0))			//¿se ha pulsado el boton?
			{
				do{							//SI -> elimina...
					num=rand();				//genera numero pseudo-aleatorio			
					}while(!input(PIN_A0));	//...rebotes
					
				printf(lcd_putc,"%d",num);	//muestra por lcd numero
				lcd_putc("\b");				//retrocede un espacio
				output_high(PIN_B3);		//activa buzzer(beep)
				delay_ms(50);				//tiempo de escucha del beep
				output_low(PIN_B3);			//desactiva buzzer(beep)
			}		 							
		}							//...infinito
}
