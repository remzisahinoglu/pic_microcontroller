// analog dijital dönüþtürücü ile step motoru ileri geri kontrol uygulamasý
// 27.08.2011
// yazan = Remzi ÞAHÝNOÐLU

#if defined(__PCM__)
#include <16F877A.h>
#device ADC=10    // dahili adc 10 bit olarak ayarlanmýstýr
#fuses XT, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#use delay(clock=4000000)
#use fast_io(c)
#use fast_io(d)
#use fast_io(e)
#define use_portb_lcd TRUE             // lcd bilgisi için port b’yi kullanýyoruz
#include <lcd416.c>
#endif

/*
#INT_AD
void ADC_kesmesi()      // adc okumasý bittiginde kesme oluþtur
{
   output_high(PIN_C5);
   delay_ms(200);          // uyarýyý burasý veriyor
   output_low(PIN_C5);
}
*/

unsigned long int bilgi=0, eski_bilgi=0;
float voltaj;
                                                 
void main()              
{
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_CCP1(CCP_OFF);
   setup_CCP2(CCP_OFF);
   
   set_tris_c(0x00);
   set_tris_d(0x00);
   set_tris_e(0x0F);
   
   output_c(0x00);
   output_d(0x00);
   
   setup_adc(adc_clock_div_32);     // adc clock frenkansý fosc/32
   setup_adc_ports(ALL_ANALOG);
   
   //enable_interrupts(INT_AD);
   enable_interrupts(GLOBAL);
   
   lcd_init();
   printf(lcd_putc, "\f ADC UYGULAMASI");
   delay_ms(1000);
      
   while(TRUE)
   {
      set_adc_channel(5);
      delay_us(20);
      
      bilgi = read_adc();
      voltaj = 0.0048828125*bilgi;  // digitale cevirme iþlemi uygulanan sinyalin
      // adc için hesaplana adým büyüklüðü: Vref / 2^10 = 5V/1024=0.0048828125
      
      if(bilgi != eski_bilgi)
      {
         eski_bilgi = bilgi;
         lcd_send_byte(0, 0x01);
         printf(lcd_putc, "\fDigital: %lu" ,bilgi);
         printf(lcd_putc, "\nV: %2.6f V" ,voltaj);
         output_d(bilgi);
      }
      delay_ms(5);
   }
}
