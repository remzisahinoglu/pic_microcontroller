// pwm ile dc motor ileri geri hýz kontrol devresi
// 03.06.2011
// yazan = Remzi ÞAHÝNOÐLU

// duyty cycle süresi belli deðerler içinde olmalý

#if defined(__PCM__)
#include <16F877A.h>
#device ADC=10    // dahili adc 10 bit olarak ayarlanmýstýr
#fuses XT, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#use delay(clock=4000000)
#use fast_io(a)
#use fast_io(c)
#use fast_io(e)
#define use_portb_lcd TRUE             // lcd bilgisi için port b’yi kullanýyoruz
#include <lcd416.c>
#endif


unsigned long int bilgi=0, eski_bilgi=0;
//float voltaj;
                                                 
void main()              
{
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_1(T1_DISABLED);
            
   set_tris_a(0x01);
   set_tris_c(0x00);
   set_tris_e(0x03);
   
   setup_CCP1(CCP_PWM);
   setup_CCP2(CCP_OFF);
   
   setup_timer_2(T2_DIV_BY_1,99,1);
   
   output_c(0x00);
   
   setup_adc(adc_clock_div_32);     // adc clock frenkansý fosc/32
   setup_adc_ports(ALL_ANALOG);
   
   enable_interrupts(INT_AD);
   enable_interrupts(GLOBAL);
   
  
   lcd_init();
   printf(lcd_putc,"dadsad");
   delay_ms(500);
      
   while(TRUE)
   {
      set_adc_channel(5);
      delay_us(20);
      bilgi = read_adc();
      //delay_ms(10);
      //voltaj = 0.0048828125*bilgi;  // digitale cevirme iþlemi uygulanan sinyalin 
      // adc için hesaplana adým büyüklüðü: Vref / 2^10 = 5V/1024=0.0048828125
      
      if(bilgi != eski_bilgi)
      {
         eski_bilgi=bilgi;
         set_pwm1_duty(bilgi);
         lcd_send_byte(0,0x01);
         lcd_gotoxy(1,1);
         printf(lcd_putc,"\f%lu",bilgi);
      }
      delay_ms(5);
   }
}
