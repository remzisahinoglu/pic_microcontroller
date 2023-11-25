//get_long fon kullanýmý verici programý - baþarýlý
//01.05.2011
//yazan = Remzi ÞAHÝNOÐLU

#if defined(__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses INTRC_IO, NOMCLR, PUT, NOWDT, NOLVP, NOBROWNOUT, NOPROTECT
#use rs232 (baud=9600, xmit=PIN_B2, rcv=PIN_B1, parity=N, stop=1)
#endif

//#device PIC16F877

int bilgi=0;


void main()
{
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_CCP1(CCP_OFF);   
   disable_interrupts(GLOBAL);
   
   output_high(PIN_B0);
   delay_ms(1000);
   output_low(PIN_B0);
   
   while(TRUE)
   {
      
      if(input(PIN_A0)==0)
      {
         delay_ms(100);
         bilgi=90;
         putc(bilgi);         
      }
      
      if(input(PIN_A1)==0)
      {
         delay_ms(100);
         bilgi=120;
         putc(bilgi);         
      }
      
      if(input(PIN_A2)==0)
      {
         delay_ms(100);
         bilgi=150;
         putc(bilgi);         
      }
   }
}
