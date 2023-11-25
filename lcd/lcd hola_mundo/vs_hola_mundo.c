////////////////////////////////////////////////////////////////////////////////////
//							VsZeNeR'05		
//						   26/Agosto/05
//							vszener@gmail.com
//	Programa:	Hola mundo
//	Version:	0.0
//
//	Dispositivo: PIC 16F648A			Compilador:	 CCS vs3.227
//	Entorno IDE: MPLAB IDE v7.21		Simulador:	 Proteus 6.7sp3
//
//	Notas: Se muestra por pantalla de lcd(LM016L) en la primera linea la 
//		   frase 'hola mundo ;)' y en la segunda linea 'VsZeNeR'05'. Tener
//		   en cuenta que hay que poner la directiva NOLVP para que el pin B4 sea de 
//		   salida. 
//
//	Conexiones:		B0 -> E		      
//					B1 -> RS			
//					B2 -> RW		
//					B4 -> D4		  
//					B5 -> D5            
//					B6 -> D6	
//					B7 -> D7                   
//////////////////////////////////////////////////////////////////////////////////

#include <16f648a.h>				//pic a utilizar 			
#fuses XT,NOWDT,NOPROTECT,PUT,NOLVP		//ordenes para el programador 
#use delay (clock=4000000)			//Fosc=4Mhz
#define use_portb_lcd TRUE			//definir portb lcd
#include<lcd.c>						//libreria manejo lcd
		
///PROGRAMA
void main(void)
{
	lcd_init();			//inicializa lcd
	printf(lcd_putc,"hola mundo ;)\nVsZeNeR'05");	//muestra por pantalla el mensaje   
}
