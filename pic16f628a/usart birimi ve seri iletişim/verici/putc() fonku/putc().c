//rs232 verici uygulamas�
//01.03.2007
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#use RS232(BAUD=1200, xmit=PIN_B2, rcv=PIN_B1)   // saniyede 1200 bit max g�nder
#endif

#byte PORTA = 0x05
#byte data = 0x30
#bit data1 = 0x30.0
#bit data2 = 0x30.1

void main()
{
   
   set_tris_a(0b00000011);
   delay_ms(500);
   output_b(0);
   data=0;
   
   while(TRUE)
   {                 
      if(bit_test(PORTA,0)==0){data1=1;}      // data reg inin 0. �nc� bitini bir yap
      if(bit_test(PORTA,1)==0){data2=1;}      // data reg inin 1. �nc� bitini bir yap
      
      putc(data);                             // data reg ini byte olarak g�nder
      delay_ms(16);
      data=0;
   }
}
