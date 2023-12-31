//rs232 al�c�-verici uygulams� pic16f877 - atmproj
//30.06.2009
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F877.h>
#fuses XT,NOWDT,NOPROTECT,NOLVP,NOBROWNOUT,NOPUT,NOWRT,NODEBUG,NOCPD
#use delay(clock=4000000)

#use rs232(baud=1200, xmit=pin_C6, rcv=pin_C7, parity=N, stop=1)
#endif

#byte porta = 0x05
#byte data = 0x30

#int_rda 
void serihaberlesme_kesmesi ()
{
   disable_interrupts(int_rda);
   output_high(pin_c5);
   data = getc();                      // veriyi al data ya �ak
   output_d(data);
   output_low(pin_c5);
}

void main()
{
   set_tris_a(0b00000001);
   
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_CCP1(CCP_OFF);
   setup_CCP2(CCP_OFF);
   
   output_low(pin_c5);
   
   enable_interrupts(GLOBAL);          // aktif edilen t�m kesmelere izin ver
     
   delay_ms(500);
   output_d(0);
   
   while(TRUE)
   {
      enable_interrupts(int_rda);      // veri alma kesmesi aktif   
         
      if(bit_test(porta,0)==0)         // RA0 � kontrol et bas�l� ise  
      {                                // veriyi g�nder
         putc('R');
         putc('E');
         putc('M');
         putc('Z');
         putc('I');        
      }
      delay_ms(100);
   }
}

