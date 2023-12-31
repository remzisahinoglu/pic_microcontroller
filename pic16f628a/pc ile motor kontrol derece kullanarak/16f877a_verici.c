//get_long fon kullan�m� verici program� - ba�ar�l�
//01.05.2011
//yazan = Remzi �AH�NO�LU

#if defined(__PCM__)
#include <16F877A.h>
#fuses XT, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#use delay(clock=4000000)               
#use rs232 (baud=9600, xmit=PIN_C6, rcv=PIN_c7, parity=N, stop=1)
#endif

//#device PIC16F877

int bilgi=0;


void main()
{
   setup_PSP(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_CCP1(CCP_OFF);
   setup_CCP2(CCP_OFF);
   
   output_low(pin_c5);
   
   disable_interrupts(GLOBAL);
   
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
