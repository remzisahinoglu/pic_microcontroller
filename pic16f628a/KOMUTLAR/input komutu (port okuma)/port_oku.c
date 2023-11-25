//port a okuma uygulamasý
//04.02.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTA = 0x05
 

void main()
{
   char veri=0; 
   
   
   while(TRUE)
   {
      veri = input_a();
      output_b(veri);
   }     
}
