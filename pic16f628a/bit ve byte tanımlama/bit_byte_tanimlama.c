//bit ve byte tanýmlama uygulamasý
//26.04.2007
//yazan = Remzi ÞAHÝNOÐLU

#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTA = 0x05
#byte OPTION = 0x81
#byte TMR0 = 0x01

#byte veri = 0x25
#bit b_0 = 0x25.0
#bit b_1 = 0x25.1
#bit b_2 = 0x25.2
#bit b_3 = 0x25.3
#bit b_4 = 0x25.4
#bit b_5 = 0x25.5
#bit b_6 = 0x25.6
#bit b_7 = 0x25.7

void main()
{
   veri=0;   
   while(TRUE)
   {
      b_0 = 1;
      b_2 = 1;
      output_b(veri);      
   }
}
