//lcd uygulamas�
//22.06.2010
//yazan = Remzi �AH�NO�LU

#if defined(__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses INTRC_IO,NOWDT,NOPROTECT,NOLVP,NOBROWNOUT
#define use_portb_lcd TRUE             // lcd bilgisi i�in port b�yi kullan�yoruz
#include <lcd.c>                       // lcd i�in gerekecek fonksiyonlar�n bulundu�u dosya lcd.c
#end if

void main()
{
   lcd_init();                // ekran�m�z� kuruyoruz
   lcd_gotoxy(1,1);           // 1.sat�r�n 1.karakterine gidiyoruz
   
   lcd_putc("deneme");  
   
   for(;;) {} //Sonsuz d�ng�
   
}
