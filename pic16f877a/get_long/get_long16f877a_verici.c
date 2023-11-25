//get_long fon kullanýmý verici programý - baþarýlý
//01.05.2011
//yazan = Remzi ÞAHÝNOÐLU

#if defined(__PCM__)
#include <16F877.h>
#use delay(clock=4000000)
#fuses XT, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#define use_portb_lcd TRUE             // lcd bilgisi için port b’yi kullanýyoruz
#include <lcd.c>                       // lcd için gerekecek fonksiyonlarýn bulunduðu dosya lcd.c
#use rs232 (baud=9600, xmit=PIN_C6, rcv=PIN_c7, parity=N, stop=1)
#include <stdlib.h>
#include <input.c> 
#endif

//#device PIC16F877

signed long m=0;
signed long t=0;

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

   
   enable_interrupts(GLOBAL);
   
   while(TRUE)
   {
      enable_interrupts(int_rda);
      
      if(input(PIN_A0)==0)
      {
         delay_ms(10);
         m=-32050;
         t=15;
         printf("%ld\r",m);
         printf("%ld\r",t);

         
      }
      
      if(input(PIN_A1)==0)
      {
         delay_ms(10);
         m=20150;
         t=184;
         printf("%ld\r",m);         
         printf("%ld\r",t);

      }
   
   }
}
