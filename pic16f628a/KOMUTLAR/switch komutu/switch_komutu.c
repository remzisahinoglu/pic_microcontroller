//switch uygulamas� uygulams�
//08.04.2007
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

void main()
{
   char i=0;
tekrar:
   switch(i)                          // i nin ald��� de�ere g�re 
   {                                  // case komtlar� ger�ekle�tirilir.
      case 1:output_b(240); break;    // i=1 ise case 1: i�lenir ve break
      case 2:output_b(15); break;     // komutu ile d�ng� k�rl�r.
      case 3:output_b(85); break;
      case 4:output_b(170); break;
   }
   i=i+1;                             // break komutu ile program ak���
   delay_ms(500);                     // buradan devam eder.
   
   if(bit_test(i,3))
   {
      i=0;
      goto tekrar;
   }
   goto tekrar;
}
