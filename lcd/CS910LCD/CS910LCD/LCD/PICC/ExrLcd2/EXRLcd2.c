#include <16F877.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)
#define ESCAP 27
#define ENTER 13
#include "lcd.c"
#define PORTC 0x07
#bit LED_IND1=PORTC.3
#bit LED_IND2=PORTC.4
byte m_LineNumber=1;

void main (void)
{
byte m_RecData=0;
setup_psp(PSP_DISABLED);
setup_spi(FALSE);
lcd_init();
set_tris_c(0x80);
lcd_putc('\f'); //Clear LCD
lcd_gotoxy(1,1);
LED_IND1=1;
LED_IND2=0;

while(1)
{
	m_RecData=getc();
	putc(m_RecData);
	switch(m_RecData)
	{
	case ESCAP:
		 lcd_putc('\f');
		 lcd_gotoxy(1,1); 	
		 break;
	case ENTER:
		  if (m_LineNumber==1)
		 		{
			 	m_LineNumber=2;
			 	lcd_gotoxy(1,m_LineNumber);
			 	}
		  else
		  		{
		  		m_LineNumber=1;
		  		lcd_putc('\f');
		  		lcd_gotoxy(1,m_LineNumber);
		  		
		  		}
		 
		 break;
	default:	
		 lcd_putc(m_RecData);
		 break;
	}
	LED_IND1=~LED_IND1;
	LED_IND2=~LED_IND2;
	
}

while(1);
}
