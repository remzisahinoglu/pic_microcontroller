
; PICBASIC PRO(TM) Compiler 2.50b, (c) 1998, 2008 microEngineering Labs, Inc. All Rights Reserved.  
_USED			EQU	1

	INCLUDE	"F:\PBP\18F4550.INC"


; Define statements.
#define		RESET_ORG		 1000h        
#define		OSC		 48                 
#define		LCD_DREG				PORTB	
#define		LCD_DBIT				0		
#define		LCD_EREG				PORTB	
#define		LCD_EBIT				4		
#define		LCD_RSREG			PORTB	
#define		LCD_RSBIT			5		
#define		LCD_BITS				4		
#define		LCD_LINES			2		

RAM_START       		EQU	00000h
RAM_END         		EQU	007FFh
RAM_BANKS       		EQU	00008h
BANK0_START     		EQU	00060h
BANK0_END       		EQU	000FFh
BANK1_START     		EQU	00100h
BANK1_END       		EQU	001FFh
BANK2_START     		EQU	00200h
BANK2_END       		EQU	002FFh
BANK3_START     		EQU	00300h
BANK3_END       		EQU	003FFh
BANK4_START     		EQU	00400h
BANK4_END       		EQU	004FFh
BANK5_START     		EQU	00500h
BANK5_END       		EQU	005FFh
BANK6_START     		EQU	00600h
BANK6_END       		EQU	006FFh
BANK7_START     		EQU	00700h
BANK7_END       		EQU	007FFh
BANKA_START     		EQU	00000h
BANKA_END       		EQU	0005Fh

FLAGS           		EQU	RAM_START + 000h
R0              		EQU	RAM_START + 001h
R1              		EQU	RAM_START + 003h
R2              		EQU	RAM_START + 005h
R3              		EQU	RAM_START + 007h
R4              		EQU	RAM_START + 009h
R5              		EQU	RAM_START + 00Bh
R6              		EQU	RAM_START + 00Dh
R7              		EQU	RAM_START + 00Fh
R8              		EQU	RAM_START + 011h
GOP             		EQU	RAM_START + 013h
RM1             		EQU	RAM_START + 014h
RM2             		EQU	RAM_START + 015h
RR1             		EQU	RAM_START + 016h
RR2             		EQU	RAM_START + 017h
RS1             		EQU	RAM_START + 018h
RS2             		EQU	RAM_START + 019h
_PORTL           		EQU	 PORTB
_PORTH           		EQU	 PORTC
_TRISL           		EQU	 TRISB
_TRISH           		EQU	 TRISC
#define _LED_1           	_PORTC??0
#define _LED_2           	_PORTC??1
#define _LED_3           	_PORTC??2
#define _LED_4           	_PORTD??0
#define _LED_5           	_PORTD??1
#define _LED_6           	_PORTD??2
#define _LED_7           	_PORTD??3
#define _LCD_BACKLIGHT   	_PORTD??7
#define _PORTC??0        	 PORTC, 000h
#define _PORTC??1        	 PORTC, 001h
#define _PORTC??2        	 PORTC, 002h
#define _PORTD??0        	 PORTD, 000h
#define _PORTD??1        	 PORTD, 001h
#define _PORTD??2        	 PORTD, 002h
#define _PORTD??3        	 PORTD, 003h
#define _PORTD??7        	 PORTD, 007h

; EEPROM data.


	INCLUDE	"LED_YAK.MAC"
	INCLUDE	"F:\PBP\PBPPIC18.LIB"

	CLEAR?	
	MOVE?CB	000h, PORTA
	MOVE?CB	000h, PORTB
	MOVE?CB	000h, PORTC
	MOVE?CB	000h, PORTD
	MOVE?CB	000h, PORTE
	MOVE?CB	03Fh, TRISA
	MOVE?CB	000h, TRISB
	MOVE?CB	0F8h, TRISC
	MOVE?CB	000h, TRISD
	MOVE?CB	007h, TRISE
	HIGH?T	_LCD_BACKLIGHT
	LCDOUT?C	0FEh
	LCDOUT?C	001h
	LCDOUT?C	0FEh
	LCDOUT?C	083h
	LCDOUT?C	04Ch
	LCDOUT?C	045h
	LCDOUT?C	044h
	LCDOUT?C	020h
	LCDOUT?C	04Fh
	LCDOUT?C	04Eh
	LCDOUT?C	02Fh
	LCDOUT?C	04Fh
	LCDOUT?C	046h
	LCDOUT?C	046h
	LCDOUT?C	0FEh
	LCDOUT?C	0C3h
	LCDOUT?C	055h
	LCDOUT?C	059h
	LCDOUT?C	047h
	LCDOUT?C	055h
	LCDOUT?C	04Ch
	LCDOUT?C	041h
	LCDOUT?C	04Dh
	LCDOUT?C	041h
	LCDOUT?C	053h
	LCDOUT?C	049h

	LABEL?L	_BASLA	
	HIGH?T	_LED_1
	PAUSE?C	0C8h
	HIGH?T	_LED_2
	PAUSE?C	0C8h
	HIGH?T	_LED_3
	PAUSE?C	0C8h
	HIGH?T	_LED_4
	PAUSE?C	0C8h
	HIGH?T	_LED_5
	PAUSE?C	0C8h
	HIGH?T	_LED_6
	PAUSE?C	0C8h
	HIGH?T	_LED_7
	PAUSE?C	0C8h
	LOW?T	_LED_1
	PAUSE?C	0C8h
	LOW?T	_LED_2
	PAUSE?C	0C8h
	LOW?T	_LED_3
	PAUSE?C	0C8h
	LOW?T	_LED_4
	PAUSE?C	0C8h
	LOW?T	_LED_5
	PAUSE?C	0C8h
	LOW?T	_LED_6
	PAUSE?C	0C8h
	LOW?T	_LED_7
	PAUSE?C	0C8h
	GOTO?L	_BASLA
	END?	

	END
