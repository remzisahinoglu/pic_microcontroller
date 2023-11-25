// led yak söndür uygulamsý
// 17.04.2007
// yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F877.h>
#use delay(clock=4000000)
#fuses XT,NOWDT,NOPROTECT,NOLVP,NOBROWNOUT
#endif

void main()
{
   set_tris_b(0);
   while(TRUE)
   {
      output_b(255);
      delay_ms(250);
      output_b(0);
      delay_ms(250);      
   }
}
