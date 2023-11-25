/*
File Name :EXRLcd.c
Description: explain the method of driving a simple 16X2
Alpha Numeric LCD in Nibble Mode(4 Wire Mode)
Write And Read a character on LCD Display
*****IMPORTANT******
uses standard library available for lcd in PICC
*/

#include <16F877.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#include "lcd.c"


void main (void)
{
byte x=4,y=2,z=0;
byte ret_z=0;
lcd_init();
lcd_putc('\f');
lcd_putc('Z');
lcd_putc('=');
lcd_putc('x');
lcd_putc('+');
lcd_putc('y');
lcd_putc(' ');
lcd_putc('=');
lcd_putc('>');
lcd_putc('Z');
lcd_putc('=');
lcd_gotoxy(11,1);
z=x+y;
lcd_putc(z+0x30);		//number to ASCII Code
ret_z=lcd_getc(11,1);
lcd_gotoxy(11,2);
lcd_putc(ret_z);
while(1);
}
