//while komutu uygulamas�
//05.04.2007
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

void main()
{
char x=0;
tekrar:
   while(x>5)         // x k���k 5 dogru mu?
   {                  
      output_b(5);    // evet  
   }
   x=x+1;             // hay�r
   delay_ms(1000);
   goto tekrar;
}
