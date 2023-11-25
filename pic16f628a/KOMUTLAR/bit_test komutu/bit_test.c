//port okuma ve test uygulamasý
//05.02.2007
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
x=input_a();
if (bit_test(x,1))      //x in 1. bitini test et. yani RA0 ý
   {   
      bit_set(x,1);       //eger 1 ise, 1 yap
      output_b(x);
      goto tekrar;
   }
   else
   {
      bit_clear(x,1);     //eger 0 ise, 0 yap
      output_b(x);
      goto tekrar;
   }
}
