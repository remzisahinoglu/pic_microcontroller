
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
T1              		EQU	RAM_START + 013h
GOP             		EQU	RAM_START + 015h
RM1             		EQU	RAM_START + 016h
RM2             		EQU	RAM_START + 017h
RR1             		EQU	RAM_START + 018h
RR2             		EQU	RAM_START + 019h
RS1             		EQU	RAM_START + 01Ah
RS2             		EQU	RAM_START + 01Bh
_A               		EQU	RAM_START + 01Ch
_C               		EQU	RAM_START + 01Dh
_D               		EQU	RAM_START + 01Eh
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


	INCLUDE	"KAYAN_~1.MAC"
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

	LABEL?L	_BASLA	
	HIGH?T	_LCD_BACKLIGHT
	MOVE?CB	000h, _A
	LABEL?L	L00002	
	CMPGT?BCL	_A, 026h, L00003
	PAUSE?C	0C8h
	LCDOUT?C	0FEh
	LCDOUT?C	001h
	MOVE?CB	000h, _C
	LABEL?L	L00004	
	CMPGT?BCL	_C, 015h, L00005
	LOOKUP?BCLB	_C, 016h, L00001, _D
	LURET?C	031h
	LURET?C	038h
	LURET?C	046h
	LURET?C	034h
	LURET?C	035h
	LURET?C	035h
	LURET?C	030h
	LURET?C	020h
	LURET?C	055h
	LURET?C	053h
	LURET?C	042h
	LURET?C	020h
	LURET?C	042h
	LURET?C	04Fh
	LURET?C	04Fh
	LURET?C	054h
	LURET?C	04Ch
	LURET?C	04Fh
	LURET?C	041h
	LURET?C	044h
	LURET?C	045h
	LURET?C	052h

	LABEL?L	L00001	
	LCDOUT?C	0FEh
	ADD?CBW	08Fh, _C, T1
	SUB?WBW	T1, _A, T1
	LCDOUT?W	T1
	LCDOUT?B	_D
	NEXT?BCL	_C, 001h, L00004
	LABEL?L	L00005	
	MOVE?CB	000h, _C
	NEXT?BCL	_A, 001h, L00002
	LABEL?L	L00003	
	MOVE?CB	000h, _A
	GOTO?L	_BASLA
	END?	

	END
