//lcd uygulamasý
//22.06.2010
//yazan = Remzi ÞAHÝNOÐLU

#if defined(__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses INTRC_IO,NOWDT,NOPROTECT,NOLVP,NOBROWNOUT
#define use_portb_lcd TRUE             // lcd bilgisi için port b’yi kullanýyoruz
#include <lcd.c>                       // lcd için gerekecek fonksiyonlarýn bulunduðu dosya lcd.c
#end if

void main()
{
   lcd_init();                // ekranýmýzý kuruyoruz
   lcd_gotoxy(1,1);           // 1.satýrýn 1.karakterine gidiyoruz
   
   lcd_putc("deneme");  
   
   for(;;) {} //Sonsuz döngü
   
}
