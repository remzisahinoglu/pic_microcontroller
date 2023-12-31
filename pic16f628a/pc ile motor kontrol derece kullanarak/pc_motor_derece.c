//pc den gelen 8bit deger ile servo motoru kontrol etme - basar�l�
//24.06.2011
//yazan = Remzi �AH�NO�LU

/*
4us lik bir sapma var osilator 20MHz yap�nca sorun kalk�yor.
periyod 20ms, duty 600us ile 2000us aras�nda de�i�ebilir.

duty    konum (derece)  bilgi(pc)
----    -----           -----
600us   -90             60
1500us  0               150
2000us  +90             200
*/

#if defined(__PCM__)
#include <16F877A.h>
#fuses HS, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#use delay(clock=20000000)               
#use rs232 (baud=9600, xmit=PIN_C6, rcv=PIN_c7, parity=N, stop=1)
#endif

//#device PIC16F877

long bilgi=0;
long doluluk=1500, bosluk=18500, periyod=20000;       // servo orta konumda

#int_rda
void serihaberlesme_kesmesi()                         // \r=0D ENTER-SATIR BA�I, \n=0A SATIR ATLA
{
   disable_interrupts(int_rda);                       // rs232 kesmesi pasif
   output_high(pin_c5);

   bilgi=getc();                                      // 60 ile 200 aras�nda olmal�, 8 bitlik bi de�er
   doluluk = bilgi * 10;                              // 600 us ile 2000 us aras�nda olmal�
   bosluk = periyod - doluluk;

   output_low(pin_c5);
   enable_interrupts(int_rda);                        // rs232 kesmesi aktif
}

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
   output_d(0xF0);
   delay_ms(100);
   output_d(0x00);
   delay_ms(100);
   output_d(0x0F);
   delay_ms(100);
   output_d(0x00);
   
   delay_ms(2000);
   
   enable_interrupts(GLOBAL);                         // t�m kesmeler aktif
   enable_interrupts(int_rda);                        // rs232 kesmesi aktif
   
   while(TRUE)
   {
      output_high(PIN_B7);                            // PWM ��k�� PORTB nin 3. bitinden (RB3)
      delay_us(doluluk);
      output_low(PIN_B7);
      delay_us(bosluk);
   }
}
