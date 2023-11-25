////////////////////////////////////////////////////////////////////////////////////
//							VsZeNeR'05		
//						   29/Agosto/05
//						vszener@gmail.com
//	Programa:	VsZeNeR'05 mov pantalla
//	Version:	0.0
//
//	Dispositivo: PIC 16F648A			Compilador:	 CCS vs3.227
//	Entorno IDE: MPLAB IDE v7.21		Simulador:	 Proteus 6.7sp3
//
//	Notas: Se muestra por pantalla de lcd(LM016L) como la palabra VsZeNeR'05 
//		   se va moviendo por pantalla, empieza en la 1�fila y termina en la 2�fila.
//		   Se utiliza variables locales:
//				�x -> indice filas, es de tipo char signed porque necesitamos n� negativos
//				      para que en la 2�fila del lcd aparezca primero el final del mensaje.
//				�y -> indice de columnas: y=1 -> 1�columna
//										  y=2 -> 2�columna
//		   La 'x' se comprende en pantalla desde 1 hasta 16 y la 'y' desde 1 hasta 2.
//		   Tener en cuenta que hay que poner la directiva NOLVP para que el pin B4 sea de 
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
	char y=1;					//indice columnas
	signed char x=1;			//indice filas
	
	lcd_init();					//inicializa lcd
	
	while(TRUE){		//bucle...
		lcd_gotoxy(x,y);		//cursor para escribir mensaje
		lcd_putc("MARCOS");	//muestra por pantalla el mensaje
		delay_ms(150);			
		x++;					//incremento indice de filas
		if(x>16){				//�ya se ha mostrado mensaje entero por 1�fila?
			x=-8;				//SI -> indice fila x=-8
			y++;				//incremento indice columnas
			if(y>2)				//�ya se ha mostrado mensaje por 2� columna?
				y=1;			//SI -> restauro indice columna
			}
		lcd_putc("\f");			//borra pantalla  
		}				//...infinito
}
