//butona basılınca binary olarak artan led uygulaması
//04.02.2007
//yazan = Remzi ŞAHİNOĞLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif


void main()
{
char x=0;  
tekrar:

output_b(x);
   if (input(PIN_A0)==1)
   {
      x=x+1;
      delay_ms(200);
      goto tekrar;
   }
      goto tekrar;
}
