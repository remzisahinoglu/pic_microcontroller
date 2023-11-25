/*
File Name :EXRLcd.c
Description: explain the method of driving a simple 16X2
Alpha Numeric LCD in Nibble Mode(4 Wire Mode)
Write And Read a character on LCD Display
*****IMPORTANT******
uses standard library available for lcd in PICC
*/

#include <16F877.h>
#device ADC=10
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)
#include "lcd.c"
char  lcd_string[20];
#define PORTC 0x07
#bit LED_IND1=PORTC.3
#bit LED_IND2=PORTC.4



void print_lcd(char *str)
{
	byte cntr=0;
	
	for (cntr=0;cntr<20;cntr++)
			lcd_putc(str[cntr]);		
}

void PrintVoltage(float m_Volts)
{
lcd_gotoxy(1,1);
sprintf(lcd_string,"Voltages= %3.3f    ",m_Volts);
print_lcd(lcd_string);
}
void PrintCounter(byte m_lng)
{
lcd_gotoxy(1,2);
sprintf(lcd_string,"Counter= %d        ",m_lng);
print_lcd(lcd_string);
}

void main (void)
{
byte m_Cntr=0;
long m_Adc_Value=0;
float m_Voltages=0.0;
setup_adc_ports(AN0_AN1_AN2_AN3_AN4);
setup_adc(ADC_CLOCK_INTERNAL );
setup_psp(PSP_DISABLED);
setup_spi(FALSE);
set_tris_c(0x80);
lcd_init();
LED_IND1=1;
LED_IND2=0;

while(1)
{
	set_adc_channel(2);
	m_Adc_Value=read_adc();
	m_Voltages=(m_Adc_Value/1024.0)*5.0;
	PrintVoltage(m_Voltages);
	PrintCounter(m_Cntr++);
    delay_ms(100);
	LED_IND1=~LED_IND1;
	LED_IND2=~LED_IND2;

}
while(1);
}
