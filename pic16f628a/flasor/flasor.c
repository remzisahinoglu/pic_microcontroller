//iki buton ile portb e baglý ledleri ileri geri yakan program uygulamasý
//10.02.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,NOMCLR,INTRC_IO,NOPROTECT
#endif

void main()
{
   while(TRUE)
   {
      output_b(0b00000000);
      delay_ms(75);
      output_b(0b00001000);
      delay_ms(75);
      
      output_b(0b00000000);
      delay_ms(75);
      output_b(0b00001000);
      delay_ms(75);
      
      output_b(0b00000000);
      delay_ms(75);
      output_b(0b00001000);
      delay_ms(75);
      
      output_b(0b00000000);
      delay_ms(75);
      output_b(0b00001000);
      delay_ms(75);;
      
      output_b(0b00000000);
      delay_ms(75);
      output_b(0b00001000);
      delay_ms(75);
      
      
      
      output_b(0b00000000);
      delay_ms(75);
      output_b(0b00010000);
      delay_ms(75);
      
      output_b(0b00000000);
      delay_ms(75);
      output_b(0b00010000);
      delay_ms(75);
      
      output_b(0b00000000);
      delay_ms(75);
      output_b(0b00010000);
      delay_ms(75);
      
      output_b(0b00000000);
      delay_ms(75);
      output_b(0b00010000);
      delay_ms(75);
      
      output_b(0b00000000);
      delay_ms(75);
      output_b(0b00010000);
      delay_ms(75);
   
   }
}
