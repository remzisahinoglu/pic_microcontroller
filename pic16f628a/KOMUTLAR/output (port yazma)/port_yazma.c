// portlara veri gönderme uygulamsý
// 08.04.2007
// yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

void main()
{
tekrar:
   output_b(255);   // b portuna desimal 255 sayýsýný yazar
   delay_ms(250);
   output_b(0);
   delay_ms(250);
   goto tekrar;
}
