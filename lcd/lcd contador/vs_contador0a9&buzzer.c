////////////////////////////////////////////////////////////////////////////////////
//							VsZeNeR'05		
//						   27/Agosto/05
//						vszener@gmail.com
//	Programa:	Contador 0a9 por LCD con pulsador y buzzer
//	Version:	0.0
//
//	Dispositivo: PIC 16F648A			Compilador:	 CCS vs3.227
//	Entorno IDE: MPLAB IDE v7.21		Simulador:	 Proteus 6.7sp3
//
//	Notas: Se muestra por pantalla de lcd(LM016L) un mensaje de inicio del programa
//		   y se espera hasta que pulsemos el boton, entonces se borra pantalla y cada vez
//		   que presionemos el boton se ira incrementando en un valor el digito mostrado en
//		   la mitad de la fila 1º del lcd hasta llegar a 9 que entonces volvera a 0 y se 
//	       emitira un beep.
//		   Tener en cuenta que hay que poner la directiva NOLVP para que el pin B4 sea de 
//		   salida. 
//				·tabNum[] -> vector donde se almacenan numeros del contador 0a9
//				·i -> indice para vector tabNum
//	Conexiones:		B0 -> E		      
//					B1 -> RS			
//					B2 -> RW		
//					B4 -> D4		  
//					B5 -> D5            
//					B6 -> D6	
//					B7 -> D7 
//					B3 -> Buzzer(beep)                   
//////////////////////////////////////////////////////////////////////////////////

#include <16f648a.h>				//pic a utilizar 			
#fuses XT,NOWDT,NOPROTECT,PUT,NOLVP		//ordenes para el programador 
#use delay (clock=4000000)			//Fosc=4Mhz
#define use_portb_lcd TRUE			//definir portb lcd
#include<lcd.c>						//libreria manejo lcd
		
///PROGRAMA
void main(void)
{
	char tabNum[10]={'0','1','2','3','4','5','6','7','8','9'},i=0;	//tabla y variable definida
		
	lcd_init();			//inicializa lcd
	printf(lcd_putc,"Presione boton\npara contador0a9");	//muestra mensaje inicio

	do{							//espera hasta que se pulse boton...
	}while(input(PIN_A0)==1);
	printf(lcd_putc,"\f");		//...para borrar pantalla e iniciar contador
	
	printf(lcd_putc,"\n\t\t\tVsZeNeR'05");	//auto publicidad en la 2ºfila lcd
	lcd_gotoxy(8,1);						//vuelve 1ºfila lcd y apunta la mitad de la columna
	
	for(;;){							//bucle...
		if(!input(PIN_A0)){						//¿se ha pulsado el boton?
			do{									//SI -> eliminar...				
			}while(!input(PIN_A0));				//...rebotes del boton
			if(i>9){							//¿se ha mostrado digito 9?
				i=0;							//SI -> restaura valor indice(para mostrar digito 0)
				output_high(PIN_B3);			//activa buzzer(beep)
				delay_ms(50);					//tiempo de escucha del beep
				output_low(PIN_B3);				//desactiva buzzer(beep)
				}
			printf(lcd_putc,"%c",tabNum[i]);	//muestra por pantalla el numero
			printf(lcd_putc,"\b");				//retrasa el cursor una posicion(escribe encima) 
			i++;								//incremento contador indice tabNum
			}			
		}							//...infinito
}
