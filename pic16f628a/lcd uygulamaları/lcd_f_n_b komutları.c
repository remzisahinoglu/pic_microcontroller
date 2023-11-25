//lcd uygulamas� \n, \f, \b kullan�m�
//22.06.2010
//yazan = Remzi �AH�NO�LU

#if defined(__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses INTRC_IO, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, MCLR
#define use_portb_lcd TRUE             // lcd bilgisi i�in port b�yi kullan�yoruz
#include <lcd.c>                       // lcd i�in gerekecek fonksiyonlar�n bulundu�u dosya lcd.c
#endif

void main()
{
   lcd_init(); //lcd ekran� kurar.
   delay_ms(6);
   
   while(TRUE)
   {
      lcd_putc("ilk sat�r\nikinci sat�r");      // \n sat�r atlat�r
      delay_ms(1000);
      lcd_putc("\f");                           // ekran� temizler
      delay_ms(1000);
      
      lcd_putc("remzi");                        // sat�r atla
      delay_ms(1000);
      lcd_putc("\b");                           // en son yaz�lan karakterin �s�tnden yazmaya baslar
      delay_ms(1000);
      lcd_putc("sah�noglu");                    // sonuc remzsahinoglu olur
      delay_ms(1000);
      
      lcd_putc("\f");
   }
}

