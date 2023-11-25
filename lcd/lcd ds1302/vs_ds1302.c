////////////////////////////////////////////////////////////////////////////////////
//							VsZeNeR'05		
//						   09/Septiembre/05
//						vszener@gmail.com
//	Programa:	Reloj-Calendario DS1302
//	Version:	0.0
//
//	Dispositivo: PIC 16F648A			Compilador:	 CCS vs3.227
//	Entorno IDE: MPLAB IDE v7.21		Simulador:	 Proteus 6.7sp3
//
//	Notas: Se muestra por pantalla de lcd(LM016L) la fecha y hora obtenida mediante la
//		   lectura del DS1302. Pudiendose modificar mediante los botones Conf/Ok y up.
//		   Se utiliza variables globales:
//				·day-> dia ds1302
//				·mth -> mes ds1302
//				·year -> año ds1302
//				·hour -> hora ds1302
//				·min -> minutos ds1302
//				·sec-> segundos ds1302
//				·menu -> variable que muestra opcion del menu configurar
//				·flag -> variable que cuenta hasta 130ms aprox.
//				·var -> ajuste fino para que desborde cada 130ms aprox.
//		   Se carga el TMR0 con 0 por lo tanto se desborda en 65ms aprox, queremos
//		   visualizar y restaurar valores cogidos del ds1302 cada 130ms aprox por lo tanto
//		   utilizamos una variable llamada flag que sera la encargada de dicha tarea.
//		   Al inicio del programa debe ser configurado el reloj, siendo el boton 'up' el
//		   encargado de ir moviendose mediante las opciones del menu:hora,minutos,....
//		   y el boton 'Conf/Ok' el encargado de configurar el reloj(cuando estemos en modo ver
//		   hora y fecha) o el encargado de salir de la configuracion de las opciones(cuando
//		   estemos en formato de configurar fecha y hora).
//		   Tener en cuenta que hay que poner la directiva NOLVP para que el pin B4 sea de 
//		   salida. 
//	Conexiones:		A0 -> RST DS1302
//					A1 -> SCLK DS1302
//					A2 -> I/O DS1302
//					A3 -> Boton 'up'
//					A5 -> Boton 'Conf/Ok'
//					B0 -> E		      
//					B1 -> RS			
//					B2 -> RW		
//					B4 -> D4		  
//					B5 -> D5            
//					B6 -> D6	
//					B7 -> D7
//					B3 -> Zumbador(beep)                   
//////////////////////////////////////////////////////////////////////////////////

#include <16f648a.h>				//pic a utilizar 			
#use delay(CLOCK=4000000)			//Fosc=4Mhz 
#fuses HS,NOPUT,NOPROTECT,NOBROWNOUT,NOLVP,NOWDT,NOMCLR	//comandos para el programador 
#use standard_io(a)
#use standard_io(b)
#define use_portb_lcd TRUE			//utilizar el port b para lcd 
#define RTC_SCLK PIN_A1				//definimos pin's...
#define RTC_IO   PIN_A2
#define RTC_RST  PIN_A0				//...de conexion de la rtc ds1302
#include <ds1302.c>					//libreria de rtc ds1302 
#include <lcd.c> 					//libreria de lcd

///VARIABLES GLOBALES
byte day,mth,year,dow,hour,min,sec;	//variabes para ds1302
byte menu=0,flag=0,var=2;			//variables para menu configurar

///DEFINICION DE FUNCIONES	
void configurar(void);
void horas(void);					
void minutos(void);
void dia(void);
void mes(void);
void anio(void);
void beep(void);

///LLAMADA FUNCION INTERRUPCION
#INT_TIMER0
void interrupcion()
{
	if(flag>var){		//¿ya es 130ms aprox?
		var--;			//SI -> decremento var... 
		if(var==0)
			var=2;		//...ajuste fino de 130ms aprox
		flag=0;			//reset flag para contar 130ms aprox
		rtc_get_date(day,mth,year,dow);		//coge dia,mes,año
      	rtc_get_time(hour,min,sec );		//coge hora,minuto,segundo
		printf(lcd_putc,"\fFecha: %2X/%2X/%2X\nHora: %2X:%2X:%2X",day,mth,year,hour,min,sec);	//lcd 
		}
		
	set_timer0(0);		//reset TMR0
	flag++;				//incremento variable flag
}

///PROGRAMA
void main(void){ 
	
	enable_interrupts(INT_TIMER0);				//interrupcion TIMER0 activada
	setup_counters(RTCC_INTERNAL,RTCC_DIV_256);	//configuracion interrupcion TMR0
	set_timer0(0);								//carga TMR0
	
	lcd_init(); 			//inicializa lcd
	rtc_init(); 			//inicializa rtc
	
	lcd_putc("Reloj Calendario\n   VsZeNeR'05");	//presentacion...
	delay_ms(800);									//...inicial
	
	configurar();								//ve a menu configurar
	enable_interrupts(GLOBAL);					//activadas interrupciones
	
 	for(;;){			//bucle...
		if(input(PIN_A5)==0){				//Si se pulsa Conf....
			while(!input(PIN_A5)){}			//elimina rebotes
			beep();
			configurar();
			}								//ve a menu configurar
		} 				//...infinito	   
} 

///FUNCION CONFIGURAR
void configurar(void){
	disable_interrupts(GLOBAL);					//desactivadas interrupciones
	
	do{
		switch(menu){
			case 0:	lcd_putc("\fConfigurar\nhoras?");		//horas
					if(!input(PIN_A5)){
						while(!input(PIN_A5)){}
						beep();
						horas();
						menu=1;								//apunta siguiente opcion
						}
					break;
			case 1: lcd_putc("\fConfigurar\nminutos?");		//minutos
					if(!input(PIN_A5)){
						while(!input(PIN_A5)){}
						beep();
						minutos();
						menu=2;								//apunta siguiente opcion
						}
					break;
			case 2: lcd_putc("\fConfigurar\ndia?");			//dias
					if(!input(PIN_A5)){
						while(!input(PIN_A5)){}
						beep();
						dia();
						menu=3;								//apunta siguiente opcion
						}
					break;
			case 3: lcd_putc("\fConfigurar\nmes?");			//mes
					if(!input(PIN_A5)){
						while(!input(PIN_A5)){}
						beep();
						mes();
						menu=4;								//apunta siguiente opcion
						}
					break;
			case 4: lcd_putc("\fConfigurar\nanio?");		//años
					if(!input(PIN_A5)){
						while(!input(PIN_A5)){}
						beep();
						anio();
						menu=5;								//apunta siguiente opcion
						}
					break;
			case 5: lcd_putc("\fSalir\nconfigurar?");		//salir configuracion
					if(!input(PIN_A5)){
						menu=6;
						beep();
						}					
			}
			
		if(!input(PIN_A3)){				//controla el boton up...
			while(!input(PIN_A3)){}
			menu++;
			if(menu>5)
				menu=0;
			}							//...para mostrar menu por lcd
			
		delay_ms(130);					//retardo para ver lcd
		}while(menu<6);
		
		menu=0;											//actualiza indices menu
		rtc_set_datetime(day,mth,year,dow,hour,min);	//nueva hora,minuto,...
		enable_interrupts(GLOBAL);						//activadas interrupciones
		set_timer0(0);									//carga TMR0
}
///FUNCION CONFIGURA HORAS
void horas(void){
	printf(lcd_putc,"\fConf.Horas:\nHora: %2X:%2X:%2X",hour,min,sec);	//muestra por lcd
	do{
		if(!input(PIN_A3)){						//¿se ha pulsado up?
			while(!input(PIN_A3)){}				//elimina rebotes
			hour++;								//SI -> incremento hour
			switch(hour){						//limites...
				case 0x0A:	hour=0x10;break;		
				case 0x1A:	hour=0x20;break;
				case 0x24:	hour=0x00;
				}								//...hour
			printf(lcd_putc,"\nHora: %2X:%2X:%2X",hour,min,sec);	//muestra por lcd hour
			}
		}while(input(PIN_A5));
	while(!input(PIN_A5)){}			//elimina rebotes
}
///FUNCION CONFIGURA MINUTOS
void minutos(void){
	printf(lcd_putc,"\fConf.Minutos:\nHora: %2X:%2X:%2X",hour,min,sec);	//muestra por lcd
	do{
		if(!input(PIN_A3)){				//¿se ha pulsado up?
		while(!input(PIN_A3)){}			//elimina rebotes
		min++;							//SI -> incremento min
		switch(min){					//limites...
			case 0x0A:	min=0x10;break;
			case 0x1A:	min=0x20;break;
			case 0x2A:	min=0x30;break;
			case 0x3A:	min=0x40;break;
			case 0x4A:	min=0x50;break;
			case 0x5A:	min=0x00;
			}							//...min
		printf(lcd_putc,"\nHora: %2X:%2X:%2X",hour,min,sec);	//muestra por lcd min
		}
		}while(input(PIN_A5));
	while(!input(PIN_A5)){}			//elimina rebotes
}
///FUNCION CONFIGURA DIAS
void dia(void){
	printf(lcd_putc,"\fConf.Dias:\nFecha: %2X/%2X/%2X",day,mth,year);	//muestra por lcd
	do{
		if(!input(PIN_A3)){					//¿se ha pulsado up?
			while(!input(PIN_A3)){}			//elimina rebotes
			day++;							//SI -> incremento day
			switch(day){					//limites...
				case 0x0A:	day=0x10;break;
				case 0x1A:	day=0x20;break;
				case 0x2A:	day=0x30;break;
				case 0x32:	day=0x01;
				}							//...day
			printf(lcd_putc,"\fConf.Dias:\nFecha: %2X/%2X/%2X",day,mth,year);	//muestra por lcd day
			}
		}while(input(PIN_A5));
	while(!input(PIN_A5)){}			//elimina rebotes
}
///FUNCION CONFIGURA MES
void mes(void){
	printf(lcd_putc,"\fConf.Mes:\nFecha: %2X/%2X/%2X",day,mth,year);	//muestra por lcd
	do{
		if(!input(PIN_A3)){					//¿se ha pulsado up?
			while(!input(PIN_A3)){}			//elimina rebotes
			mth++;							//SI -> incremento mth
			switch(mth){					//limites...
				case 0x0A:	mth=0x10;break;
				case 0x13:	mth=0x01;
				}							//...mth
		printf(lcd_putc,"\fConf.Mes:\nFecha: %2X/%2X/%2X",day,mth,year);	//muestra por lcd
			}
		}while(input(PIN_A5));
	while(!input(PIN_A5)){}			//elimina rebotes
}
///FUNCION CONFIGURA AÑOS
void anio(void){
	printf(lcd_putc,"\fConf.Anio:\nFecha: %2X/%2X/%2X",day,mth,year);	//muestra por lcd
	do{
		if(!input(PIN_A3)){					//¿se ha pulsado up?
			while(!input(PIN_A3)){}			//elimina rebotes
			year++;							//SI -> incremento mth
			switch(year){					//limites...
				case 0x0A:	year=0x10;break;
				case 0x1A:	year=0x20;break;
				case 0x2A:	year=0x30;break;
				case 0x3A:	year=0x40;break;
				case 0x4A:	year=0x50;break;
				case 0x5A:	year=0x60;break;
				case 0x6A:	year=0x70;break;
				case 0x7A:	year=0x80;break;
				case 0x8A:	year=0x90;break;
				case 0x9A:	year=0x00;
				}							//...year
		printf(lcd_putc,"\fConf.Anio:\nFecha: %2X/%2X/%2X",day,mth,year);	//muestra por lcd
			}
		}while(input(PIN_A5));
	while(!input(PIN_A5)){}			//elimina rebotes
}
///FUNCION BEEP
void beep(void){
	output_high(PIN_B3);	//activa zumbador
	delay_ms(50);
	output_low(PIN_B3);		//desactiva zumbador
}
