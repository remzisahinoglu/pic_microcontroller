//pot ile servo motor kontrol program� 
//22.06.2011
//yazan = Remzi �AH�NO�LU

/*
periyod 20ms, duty 1000us ile 2000us aras�nda de�i�ebilir.

duty    konum (derece)
----    -----
1000us  -90
1500us  0
2000us  +90
*/

#if defined(__PCM__)
#include <16F877A.h>
#device ADC=8    // dahili adc 8 bit olarak ayarlanm�st�r
#fuses XT, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#use delay(clock=4000000)
#use fast_io(c)
#use fast_io(b) 
#endif

//#device PIC16F877

long long bilgi=0, eski_bilgi=0;
long long doluluk, periyod, bosluk;

void main()
{
   setup_PSP(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_CCP1(CCP_OFF);
   setup_CCP2(CCP_OFF);
   
   set_tris_c(0x80);
   set_tris_b(0x00);
      
   output_low(pin_c5);
   output_d(0xF0);
   delay_ms(300);
   output_d(0x00);
   delay_ms(300);
   output_d(0x0F);
   delay_ms(300);
   output_d(0x00);
   
   setup_adc_ports(ALL_ANALOG);
   setup_adc(ADC_CLOCK_DIV_8);
   set_adc_channel(5);
   delay_us(30);
   
   disable_interrupts(GLOBAL);
   
   periyod = 20000;                      // pwm periyodu = 20ms   olarak ayarlad�k
   doluluk = 1500;                       // doluluk oran�n� ilk olarak motoru orta konuma getirmek i�in 1.5ms olarak ayarland�
   bosluk = periyod - doluluk;           // bo�lu�u hesapla ilk enerji verildi�inde sorun ��kmas�n
   
   while(TRUE)
   {
      bilgi = read_adc();
      
      if(bilgi != eski_bilgi)
      {
         eski_bilgi = bilgi;
         // adc bilgisini servo i�in duty cycle s�resini cevir
         doluluk = (1400*bilgi)/255 + 600;
         bosluk = periyod - doluluk;
      }
      
      output_high(PIN_B3);                          // PWM ��k�� PORTB nin 3. bitinden (RB3)
      delay_us(doluluk);                                           
      output_low(PIN_B3);
      delay_us(bosluk);
   }
}

