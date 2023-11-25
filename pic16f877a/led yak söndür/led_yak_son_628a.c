//iki buton ile portb e baglý ledleri ileri geri yakan program uygulamasý
//11.06.2011
//yazan = Remzi ÞAHÝNOÐLU

#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

char i=0;

void main()
{

   while(TRUE)
   {
      if(input(PIN_A0)==0)
      {
         i=i+1;
         output_b(i);
         delay_ms(100);
      }
      if(input(PIN_A1)==0)
      {
         i=i-1;
         output_b(i);
         delay_ms(100);
      }  
      
   }
   }
