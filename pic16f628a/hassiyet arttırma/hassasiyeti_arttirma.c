// delta robot i�in test program�
//20.04.2011
//yazan = Remzi �AH�NO�LU

#if defined(__PCH__)
#include <33FJ32MC204.h>
#include <MATH.h>
#use delay(clock=4000000)
#fuses XT, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#define use_portb_lcd TRUE             // lcd bilgisi i�in port b�yi kullan�yoruz
#include <LCD416.c>                    // lcd i�in gerekecek fonksiyonlar�n bulundu�u dosya lcd416.c
#endif

#device DSPIC33FJ32MC204

float x=0;

void main()
{

   
   x = sin(0.5);
   printf(lcd_putc "%1.7f", x);
   while(TRUE){}

}
