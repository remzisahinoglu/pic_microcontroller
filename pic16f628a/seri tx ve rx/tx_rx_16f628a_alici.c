///////////////////////////////////////////////////////////////////////////////
////                    rx_tx_kullanýmý.C                                  ////
////                                                                       ////
////  Bu porgram her iki pic den rx ve tx pinlerinin kullanýlmasýyla       ////
////  ilgilidir.                                                           ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
////  YAZAN: Remzi ÞAHÝNOÐLU                                               ////
////  TARÝH: 07.05.2011                                                    ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses XT, MCLR, PUT, NOWDT, NOLVP, NOBROWNOUT, NOPROTECT
#use rs232 (baud=9600, xmit=PIN_B2, rcv=PIN_B1, parity=N, stop=1)
#endif

//#device PIC16F628A

char a=0;


#int_rda
void serihaberlesme_kesmesi()          // \r=0D ENTER-SATIR BAÞI, \n=0A SATIR ATLA
{  
   disable_interrupts(int_rda);
   output_high(PIN_B4);
   
   a = getc();
   
   output_low(PIN_B4);
}

void main()
{

   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_CCP1(CCP_OFF);
   

  
   enable_interrupts(GLOBAL);
   
   while(TRUE)
   {
      enable_interrupts(GLOBAL);
      
      output_b(a);
   }
}
