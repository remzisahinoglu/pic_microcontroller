///////////////////////////////////////////////////////////////////////////////
////                    rx_tx_kullanımı.C                                  ////
////                                                                       ////
////  Bu porgram her iki pic den rx ve tx pinlerinin kullanılmasıyla       ////
////  ilgilidir.                                                           ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
////  YAZAN: Remzi ŞAHİNOĞLU                                               ////
////  TARİH: 07.05.2011                                                    ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses XT, MCLR, PUT, NOWDT, NOLVP, NOBROWNOUT, NOPROTECT
#use rs232 (baud=9600, xmit=PIN_B2, rcv=PIN_B1, parity=N, stop=1)
#endif

//#device PIC16F628A


void main()
{

   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_CCP1(CCP_OFF);
   

   output_low(PIN_B0);
   output_low(PIN_B3);
   output_low(PIN_B4);
   output_low(PIN_B5);
   output_low(PIN_B6);
   output_low(PIN_B7);
   

  
   enable_interrupts(GLOBAL);
   
   while(TRUE)
   {
      if(input(PIN_A0)==0)
      {
         putc(0x04);
      }
      if(input(PIN_A1)==0)
      {
         putc(0x08);
      }

   }
}
