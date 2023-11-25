// led yak s�nd�r uygulams�
// 28.06.2009
// yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F877.h>
#use delay(clock=4000000)
#fuses XT,NOWDT,NOPROTECT,NOBROWNOUT,NOLVP,NOPUT,NOWRT,NODEBUG,NOCPD
#endif

void main()
{
   set_tris_d(0);
   delay_ms(500);
   while(TRUE)
   {
      output_d(1);
      delay_ms(500);
      output_d(0);
      delay_ms(500); 
      output_d(2);
      delay_ms(500);
      output_d(0);
      delay_ms(500);
      output_d(4);
      delay_ms(500);
      output_d(0);
      delay_ms(500);
      output_d(8);
      delay_ms(500);
      output_d(0b00001110);
      delay_ms(1500);
      output_d(0);
      delay_ms(1000);
      output_d(2);
      delay_ms(1000);
   }
}
