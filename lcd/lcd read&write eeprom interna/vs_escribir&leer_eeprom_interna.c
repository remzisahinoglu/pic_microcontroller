////////////////////////////////////////////////////////////////////////////////////
//							VsZeNeR'05		
//						   30/Agosto/05
//						vszener@gmail.com
//	Programa:	Escribir & leer eeprom interna del PIC y mostrarla por lcd
//	Version:	0.0
//
//	Dispositivo: PIC 16F648A			Compilador:	 CCS vs3.227
//	Entorno IDE: MPLAB IDE v7.21		Simulador:	 Proteus 6.7sp3
//
//	Notas: Se muestra por pantalla de lcd(LM016L) la informacion grabada en la memoria
//		   interna del PIC(eeprom) y luego se muestra por la lcd
//		   Se utiliza variables locales:
//				·LAST_VOLUME -> posicion a grabar en la eeprom del pic
//				·mensaje1[] -> contiene 1º mensaje a escribir en eeprom y mostrar en lcd
//				·mensaje2[] -> contiene 2º mensaje a escribir en eeprom y mostrar en lcd
//				·mensaje3[] -> contiene 3º mensaje a escribir en eeprom y mostrar en lcd
//				·mensaje4[] -> contiene 4º mensaje a escribir en eeprom y mostrar en lcd
//				·derecha() -> funcion que muestra mensaje de izquierda a derecha
//				·izquierda() -> funcion que muestra mensaje de derecha a izquierda
//				·volume -> contiene mensaje leido por la eeprom interna del pic
//		   Tener en cuenta que hay que poner la directiva NOLVP para que el pin B4 sea de 
//		   salida. 
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
	char LAST_VOLUME=0,volume;		//variables de direccion escritura eeprom y mensaje
	char mensaje1[]="VsZeNeR'05";	//mensajes...
	char mensaje2[]="HOLA";
	char mensaje3[]="FORO";
	char mensaje4[]="TODOPIC!";		//a escribir en eeprom interna y mostrar en lcd	
		
	lcd_init();						//inicializa lcd
	
	write_eeprom(LAST_VOLUME,mensaje1);	//escritura de los mensajes...
	LAST_VOLUME++;
	write_eeprom(LAST_VOLUME,mensaje2);
	LAST_VOLUME++;
	write_eeprom(LAST_VOLUME,mensaje3);
	LAST_VOLUME++;
	write_eeprom(LAST_VOLUME,mensaje4);	//...en la eeprom interna(0 hasta la 3)
	
	for(;;){	//bucle...
		for(LAST_VOLUME=0;LAST_VOLUME<4;LAST_VOLUME++){	//bucle que recorre las posiciones de eeprom
		volume = read_EEPROM (LAST_VOLUME);				//almacena mensaje de la direccion eeprom
		printf(lcd_putc,"%s",volume);					//muestra mensaje por pantalla lcd
		delay_ms(500);
		lcd_putc("\f");									//borra pantalla lcd
		}		//...infinito
	}
}
