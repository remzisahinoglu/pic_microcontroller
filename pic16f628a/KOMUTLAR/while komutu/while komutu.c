//while komutu uygulamasý
//05.04.2007
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
   while(x>5)         // x küçük 5 dogru mu?
   {                  
      output_b(5);    // evet  
   }
   x=x+1;             // hayýr
   delay_ms(1000);
   goto tekrar;
}
