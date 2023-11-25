//butona basýlýnca binary olarak artan led uygulamasý
//04.02.2007
//yazan = Remzi ÞAHÝNOÐLU
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
   if (input(PIN_A0)==0)
   {
      x=x+1;
      delay_ms(200);
      goto tekrar;
   }
   else
   {
      goto tekrar;
   }
}
