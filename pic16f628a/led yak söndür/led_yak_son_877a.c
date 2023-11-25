// led yak s�nd�r uygulamas�
// 11.06.2011
// yazan = Remzi �AH�NO�LU

#if defined(__PCM__)
#include <16F877A.h>
#fuses XT, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#use delay(clock=4000000)
#endif
                                                 
void main()              
{
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_CCP1(CCP_OFF);
   setup_CCP2(CCP_OFF);

   set_tris_b(0x00);
   
   output_b(0x00);
   delay_ms(1000);
   output_b(0xFF);
   delay_ms(1000);
   output_b(0x00);
      
   while(TRUE)
   {
      
      delay_ms(100);
      output_high(PIN_B1);
          
      delay_ms(100);
      output_low(PIN_B1);
      
   }
}
