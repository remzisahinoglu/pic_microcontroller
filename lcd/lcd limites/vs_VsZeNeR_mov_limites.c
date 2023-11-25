////////////////////////////////////////////////////////////////////////////////////
//							VsZeNeR'05		
//						   30/Agosto/05
//						vszener@gmail.com
//	Programa:	VsZeNeR'05 mov limites
//	Version:	0.0
//
//	Dispositivo: PIC 16F648A			Compilador:	 CCS vs3.227
//	Entorno IDE: MPLAB IDE v7.21		Simulador:	 Proteus 6.7sp3
//
//	Notas: Se muestra por pantalla de lcd(LM016L) como la palabra VsZeNeR'05 
//		   se va moviendo por pantalla, empieza en la 1ºfila y termina en la 2ºfila y
//		   empieza a moverse en sentido contrario,2ºfila hasta la 1ºfila.
//		   Se utiliza variables globales:
//				·x -> indice filas, es de tipo char signed porque necesitamos nº negativos
//				      para que en la 2ºfila del lcd aparezca primero el final del mensaje.
//				·y -> indice de columnas: y=1 -> 1ºcolumna
//										  y=2 -> 2ºcolumna
//				·derecha() -> funcion que muestra mensaje de izquierda a derecha
//				·izquierda() -> funcion que muestra mensaje de derecha a izquierda
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

///DEFINICION VARIABLES GLOBALES
char y=1;					//indice columnas
signed char x=1;			//indice filas

///DECLARACION DE FUNCIONES
void derecha(void);			//mensaje hacia la derecha
void izquierda(void);		//mensaje hacia la izquierda
		
///PROGRAMA
void main(void)
{		
	lcd_init();			//inicializa lcd
	
	while(TRUE){		//bucle...
		derecha();
		izquierda();
		}				//...infinito
}

///FUNCION MUESTRA MENSAJE DE IZQUIERDA -> DERECHA
void derecha(void)
{
	do{
		lcd_gotoxy(x,y);		//cursor para escribir mensaje
		lcd_putc("VsZeNeR'05");	//muestra por pantalla el mensaje
		delay_ms(150);			
		x++;					//incremento indice de filas
		if(x>17){				//¿ya se ha mostrado mensaje entero por 1ºfila?
			x=-8;				//SI -> indice fila x=-8
			y++;				//incremento indice columnas
			if(y>2){			//¿ya se ha mostrado mensaje entero por 2ºfila?
				x=16;			//SI -> actualiza indice filas
				y=2;			//actualiza indice columnas
				return;}		//sal de la funcion derecha()
			}
		lcd_putc("\f");			//borra pantalla
		}while(TRUE);
}
///FUNCION MUESTRA MENSAJE DE DERECHA -> IZQUIERDA
void izquierda(void)
{
	do{			
		x--;					//decremento indice de filas
		if(x<-9){				//¿ya se ha mostrado mensaje entero por 2ºfila?
			x=16;				//SI -> indice fila x=16
			y--;				//decremento indice columnas
			if(y<1){			//¿ya se ha mostrado mensaje por 1º fila?
				x=-8;			//SI -> restauro indice filas
				y=1;			//restauro indice columnas
				return;}		//sal de la funcion izquierda()
			}
		lcd_gotoxy(x,y);		//cursor para escribir mensaje
		lcd_putc("VsZeNeR'05");	//muestra por pantalla el mensaje
		delay_ms(150);
		lcd_putc("\f");			//borra pantalla
		}while(TRUE);
}
